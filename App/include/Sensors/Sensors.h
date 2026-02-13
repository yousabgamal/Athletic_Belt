/**
 * @file Sensors.h
 * @author Yousab Gamal(yous.gmm@gmail.com)
 * @brief A file that includes all the functions declaration for all sensors
 * @version 0.1
 * @date 2025-10-23
 * @copyright Copyright (c) 2025
 */

 #ifndef SENSORS_H
 #define SENSORS_H

/******************Section: Includes**********************/
#include "main.h"
#include "type_def.h"

/******************Section: Macros Declarations***********/
#define ONE_SECOND_CHECK 1000
#define TEN_SECOND_CHECK 10000

/******************Section: Macros Functions Declarations*/

/******************Section: Data Types Declarations*******/

/******************Section: Functions Declarations********/
void Initialize_Communication(uint32 bps);

void Heart_Rate_And_SPo2_Sensor_Update(void);
bool Heart_Rate_And_SPo2_Sensor_Config(void);
void Get_Heart_Rate_And_SPo2_Ratio(float * Heart_Rate , uint8 * SPo2_Ratio);

bool Temperature_Sensor_Config(void);
void Get_Temperature_Sensor(float * Athletic_Temp);

bool Acceleration_Sensor_Config(void);
void Get_Acceleration_Sensor(float * filteredAx , float * filteredAy , float * filteredAz);

void Get_Physical_Effort(uint16 * Physical_Effort);

 #endif
