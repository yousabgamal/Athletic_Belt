/**
 * @file Sensors.cpp
 * @author Yousab Gamal(yous.gmm@gmail.com.com)
 * @brief A file that include the functions definition to all the sensors, also
 *        include the sensors pins configuration.
 * @version 0.1
 * @date 2025-10-23
 * @copyright Copyright (c) 2025
 * 
 */

#include "Sensors/Sensors.h"
static PulseOximeter HeartRate_And_SPo2_Sensor_Obj = PulseOximeter();
static Adafruit_MLX90614 Temperature_Sensor_Obj = Adafruit_MLX90614();
static Adafruit_MPU6050 Acceleration_Sensor_Obj;

static const uint16 CAL_SAMPLES = 500;           
static const float ALPHA = (float)0.15;             
static const float DEADZONE = (float)0.08; 
static float biasX = 0.0f, biasY = 0.0f, biasZ = 0.0f;
static float filtX = 0.0f, filtY = 0.0f, filtZ = 0.0f;

static float HR_MAX = 198.0f , HR_Normal = 0.0f , Temp_Normal = 0.0f;
static uint16 GSR_Normal , MAX_GSR , MIN_GSR;

static void calibrateSensor(void);
static float deadzoneZero(float Value);

/**
 * @brief A function to define the speed of transition and initialize the I2C communication
 * @param bps: Bits per seconds
*/
void Initialize_Communication(uint32 bps)
{
    Serial.begin(bps);
    Wire.begin(SDA_MAX30100_AND_MPU650_SENSOR_PIN , SCL_MAX30100_AND_MPU650_SENSOR_PIN);
    Wire.setClock(100000); 
    while(!(Wire.begin(SDA_MAX30100_AND_MPU650_SENSOR_PIN , SCL_MAX30100_AND_MPU650_SENSOR_PIN)))
    {
        //Bloking the code until the I2C communication is initialized successfully
    }
    Wire1.begin(SDA_MLX90614_SENSOR_PIN , SCL_MLX90614_SENSOR_PIN);
    while(!(Wire1.begin(SDA_MLX90614_SENSOR_PIN , SCL_MLX90614_SENSOR_PIN)))
    {
        //Bloking the code until the I2C communication is initialized successfully
    }
    Wire1.setClock(50000);
    //delay(100);
}

/**
 * @brief A function to update the heart rate and SPo2 sensor readings
 */
void Heart_Rate_And_SPo2_Sensor_Update(void)
{
    HeartRate_And_SPo2_Sensor_Obj.update();
}

/**
 * @brief A function for heart rate and SPo2 sensor pins configrations and communication protcol(I2C)
 * @return true 
 * @return false 
 */
bool Heart_Rate_And_SPo2_Sensor_Config(void)
{
    static bool ret_val = true;
    if(!(HeartRate_And_SPo2_Sensor_Obj.begin())) 
    {
        ret_val = false;
    } 
    else
    {
        HeartRate_And_SPo2_Sensor_Obj.setIRLedCurrent(MAX30100_LED_CURR_27_1MA); 
    }
    return ret_val;
};

/**
 * @brief A function to get the SPo2 ratio and the heart rate from the athletic
 *        and store the two results in two different pointers
 * @param Heart_Rate: Pointer holds the heart rate data retrieved from the athletic 
 * @param SPo2_Ratio: Pointer holds the SPo2 data retrieve from the athletic 
 * @return Nothing
 */
void Get_Heart_Rate_And_SPo2_Ratio(float * Heart_Rate , uint8 * SPo2_Ratio)
{
    if(HeartRate_And_SPo2_Sensor_Obj.getHeartRate() < 40)
    {
        *(Heart_Rate) = 0;
    }
    else
    {
        *(Heart_Rate) = HeartRate_And_SPo2_Sensor_Obj.getHeartRate();
        if(*(Heart_Rate) > 118.8)
        {
            if(HR_MAX == 198.0f)
            {
                HR_MAX = *(Heart_Rate);
            }
            else if(*(Heart_Rate) > HR_MAX)
            {
                HR_MAX = *(Heart_Rate);
            }
            else
            {
                /*Nothing*/
            }
        }
        HR_Normal = *(Heart_Rate) / HR_MAX;
    }
    if(0 == (*(Heart_Rate)))
    {
        *(SPo2_Ratio) = HeartRate_And_SPo2_Sensor_Obj.getSpO2();
    }
    else
    {
        *(SPo2_Ratio) = (HeartRate_And_SPo2_Sensor_Obj.getSpO2() + 3);
        if(0 == HeartRate_And_SPo2_Sensor_Obj.getSpO2())
        {
            *(SPo2_Ratio) = 0; 
        }
        else if(*(SPo2_Ratio) > 100)
        {
            *(SPo2_Ratio) = 100;
        }
        else
        {
            /*Nothing*/
        }
    }
}

/**
 * @brief  A function for temperature sensor pins configrations and communication protcol(I2C)
 * @return true or false
 */
bool Temperature_Sensor_Config()
{
    bool ret_val = true;
    if(!Temperature_Sensor_Obj.begin(0x5A, &Wire1))
    {
        ret_val = false;
    }
    else
    {
        /*Nothing*/
    }
    return ret_val;
}

/**
 * @brief A function to get the temperature of the athletic in celsius degree from 
 *        the sensor and store it in the pointer
 * @param Athletic_Temp: Pointer holds the temperature data retrieved from the athletic 
 * @return Nothing
 */
void Get_Temperature_Sensor(float * Athletic_Temp)
{
    float ambient_temp = Temperature_Sensor_Obj.readAmbientTempC();
    float object_temp  = Temperature_Sensor_Obj.readObjectTempC();
    if((object_temp - ambient_temp) > 2.0)  
    {
        *(Athletic_Temp) = object_temp;
        Temp_Normal = (*(Athletic_Temp) - 36) / 36; /*36 is the normal body temperature and 3.6 is the difference between the normal body temperature and the maximum body temperature*/
    }
    else
    {
        *(Athletic_Temp) = 0.0;
    }
}

/**
 * @brief A function for acceleration sensor pins configrations and communication protcol(I2C) 
 * @return true 
 * @return false 
 */
bool Acceleration_Sensor_Config(void)
{
    bool ret_val = true;
    if(!Acceleration_Sensor_Obj.begin(0x68, &Wire))
    {  
        ret_val = false;
    }
    else
    {
        /*Nothing*/
    }
    Acceleration_Sensor_Obj.setAccelerometerRange(MPU6050_RANGE_8_G);
    Acceleration_Sensor_Obj.setGyroRange(MPU6050_RANGE_500_DEG);
    Acceleration_Sensor_Obj.setFilterBandwidth(MPU6050_BAND_44_HZ);
    delay(100);
    calibrateSensor();
    return ret_val;
}

/**
 * @brief A function to get the acceleration(x, y and z) from the sensor while 
 *        the athletic runing in the field
 * @param Object_Speed: Pointer holds the acceleration(x, y and z) data retrieved from the the sensor 
 */
void Get_Acceleration_Sensor(float * Final_x , float * Final_y , float * Final_z)
{
    sensors_event_t a, g, temp;
    Acceleration_Sensor_Obj.getEvent(&a, &g, &temp);
    float ax = a.acceleration.x - biasX;
    float ay = a.acceleration.y - biasY;
    float az = a.acceleration.z - biasZ;

    filtX = ALPHA * ax + (1.0f - ALPHA) * filtX;
    filtY = ALPHA * ay + (1.0f - ALPHA) * filtY;
    filtZ = ALPHA * az + (1.0f - ALPHA) * filtZ;

    float outX = deadzoneZero(filtX);
    float outY = deadzoneZero(filtY);
    float outZ = deadzoneZero(filtZ);

    *(Final_x) = outX;
    *(Final_y) = outY;
    *(Final_z) = outZ;
}

/**
 * @brief A function to get the effort of the athletic in the field
 *        (Notice that: it measures in voltage)
 * @param Physical_Effort: Pointer holds the physical effort of the athletic 
 */
void Get_Physical_Effort(uint16 * Physical_Effort)
{
    static uint8 First_Time_Check = 0;
    static uint16 GSR_Value = 0;
    static uint32 start_Time = 0;
    static float Effort = 0.0f;
    GSR_Value = analogRead(GSR_INPUT_SENSOR_PIN);
    if(First_Time_Check == 0 && GSR_Value != 0)
    {
        MIN_GSR = GSR_Value;
        MAX_GSR = GSR_Value;
        start_Time = millis();    
        while(millis() - start_Time < TEN_SECOND_CHECK)   
        {
            GSR_Value = analogRead(GSR_INPUT_SENSOR_PIN);
            if(GSR_Value > MAX_GSR)
            {
                MAX_GSR = GSR_Value;
            }
            if(GSR_Value < MIN_GSR)
            {
                MIN_GSR = GSR_Value;
            }
        }
        First_Time_Check = 1;
    }
    else
    {
        /*Nothing*/
    }
    if(GSR_Value > MAX_GSR)
    {
        MAX_GSR = GSR_Value;
    }
    else 
    {
        /*Nothing*/
    }
    if(MAX_GSR > MIN_GSR)
    {
        GSR_Normal = (float)(GSR_Value - MIN_GSR) / (float)(MAX_GSR - MIN_GSR);
        if(GSR_Normal > 1)
        {
            GSR_Normal = 1;
        }
        else if(GSR_Normal < 0)
        {
            GSR_Normal = 0;
        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {
        GSR_Normal = 0;
    }
    Effort = (0.5f * HR_Normal) + (0.3f * GSR_Normal) + (0.2f * Temp_Normal);
    *(Physical_Effort) = (uint16)(Effort * 100);
}

/**
 * @brief A helper fun to get the bais to be subtracted in the following code
 */
static void calibrateSensor(void)
{
    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    for(int i = 0; i < CAL_SAMPLES; ++i)
    {
        sensors_event_t a, g, temp;
        Acceleration_Sensor_Obj.getEvent(&a, &g, &temp);
        sumX += a.acceleration.x;
        sumY += a.acceleration.y;
        sumZ += a.acceleration.z;
        delay(5); 
    }
    biasX = (float)(sumX / CAL_SAMPLES);
    biasY = (float)(sumY / CAL_SAMPLES);
    biasZ = (float)(sumZ / CAL_SAMPLES);
    filtX = filtY = filtZ = (float)0.0;
}

/**
 * @brief A helper fun to clean the small noise(Filtration) from the sensor
 * @param Value Any value under it will be ignored
 * @return float 
 */
static float deadzoneZero(float Value)
{
    if(fabs(Value) < DEADZONE)
    {
        Value = 0.0;
    }
    else
    {
        /*Nothing*/
    }
    return Value;
}
