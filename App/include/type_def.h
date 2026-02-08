/**
 * @file type_def.h
 * @author Yousab Gamal(yous.gmm@gmail.com)
 * @brief A file that includes all data types declarations
 * @version 0.1
 * @date 2025-10-14
 * @copyright Copyright (c) 2025
 */
#ifndef type_def
#define	type_def

/******************Section: Includes**********************/

/******************Section: Macros Functions Declarations*/

/******************Section: Data Types Declarations*******/
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int   uint32;

typedef signed char     sint8;
typedef signed short    sint16;
typedef signed int     sint32;

/*typedef enum
{
    GPIO0  = 0,  //RESERVED
    GPIO1  = 1,  //RESERVED
    GPIO2  = 2,  //RESERVED
    GPIO3  = 3,  //RESERVED
    GPIO4  = 4,  //GSR_INPUT_SENSOR_PIN
    GPIO5  = 5,
    GPIO6  = 6,  //SDA_MPU6050_SENSOR_PIN
    GPIO7  = 7,  //SCL_MPU6050_SENSOR_PIN
    GPIO8  = 8,  //SDA_MAX30100_SENSOR_PIN
    GPIO9  = 9,  //SCL_MAX30100_SENSOR_PIN
    GPIO10 = 10,
    GPIO11 = 11, //SDA_MLX90614_AND_MPU6050_SENSORS_PIN
    GPIO12 = 12, //SCL_MLX90614_AND_MPU6050_SENSORS_PIN
    GPIO13 = 13,
    GPIO14 = 14,
    GPIO15 = 15,
    GPIO16 = 16,
    GPIO17 = 17,
    GPIO18 = 18,
    GPIO19 = 19,
    GPIO20 = 20,
    GPIO21 = 21,
    GPIO35 = 35,
    GPIO36 = 36,
    GPIO37 = 37,
    GPIO38 = 38,
    GPIO39 = 39,
    GPIO40 = 40,
    GPIO41 = 41,
    GPIO42 = 42,
    GPIO43 = 43,
    GPIO44 = 44,
    GPIO45 = 45, //RESERVED
    GPIO46 = 46, //RESERVED
    GPIO47 = 47,
    GPIO48 = 48,
} GPIOx;*/


/******************Section: Macros Declarations***********/
#define ZERO    0x00
#define ONE     0x01

#define TRUE    0x01
#define FALSE   0x00

#define SDA_MAX30100_AND_MPU650_SENSOR_PIN   0X08
#define SCL_MAX30100_AND_MPU650_SENSOR_PIN   0X09
#define SDA_MLX90614_SENSOR_PIN   0X06
#define SCL_MLX90614_SENSOR_PIN   0X07
#define GSR_INPUT_SENSOR_PIN     0X04

/******************Section: Functions Declarations********/

#endif	

