// o2_sensor.h

#ifndef o2_sensor_h
#define o2_sensor_h




#define   O2_SENSOR_ADS_ADDR  (0x48)  // for O2 sensor board of Ventilator project


/**
 * @def   Defines 
 * @brief   Defines used in this file
 */
#define SUCCESS                   0
#define ERROR_OFFSET              (-1)
#define ERROR_UNKNOWN             (ERROR_OFFSET-1)
#define ERROR_I2C_TIMEOUT         (ERROR_OFFSET-2)
#define ERROR_TIMER_INIT          (ERROR_OFFSET-3)
#define ERROR_SENSOR_READ         (ERROR_OFFSET-4)
#define ERROR_BAD_PARAM           (ERROR_OFFSET-5)
#define ERROR_SENSOR_UNSUPPORTED  (ERROR_OFFSET-6)
#define ERROR_SENSOR_CALIBRATION  (ERROR_OFFSET-7)


#define O2_SENSOR_CHANNEL_NO      (0)
#define PRESSURE_SENSOR_CHANNL_NO (1)
#define TEMPR_SENSOR_1_CHANNEL_NO (2)
#define TEMPR_SENSOR_2_CHANNEL_NO (3)



int  sensor_zero_calibration  (void);
void test_ads1115             (void);



#endif  //  '#ifndef   o2_sensor_h' ends
