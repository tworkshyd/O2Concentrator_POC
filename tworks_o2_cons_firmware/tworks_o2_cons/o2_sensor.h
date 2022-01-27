// o2_sensor.h

#ifndef o2_sensor_h
#define o2_sensor_h




#define   O2_SENSOR_ADS_ADDR        (0x48)  // for O2 sensor board of Ventilator project


/**
 * @def   Defines 
 * @brief   Defines used in this file
 */
#define SUCCESS                     0
#define ERROR_OFFSET                (-1)
#define ERROR_UNKNOWN               (ERROR_OFFSET-1)
#define ERROR_I2C_TIMEOUT           (ERROR_OFFSET-2)
#define ERROR_TIMER_INIT            (ERROR_OFFSET-3)
#define ERROR_SENSOR_READ           (ERROR_OFFSET-4)
#define ERROR_BAD_PARAM             (ERROR_OFFSET-5)
#define ERROR_SENSOR_UNSUPPORTED    (ERROR_OFFSET-6)
#define ERROR_SENSOR_CALIBRATION    (ERROR_OFFSET-7)


#define O2_SENSOR_CHANNEL_NO        (0)
#define PRESSURE_SENSOR_CHANNL_NO   (1)
#define TEMPR_SENSOR_1_CHANNEL_NO   (2)
#define TEMPR_SENSOR_2_CHANNEL_NO   (3)



#define NUM_OF_SAMPLES_O2   (3)



// Variable declarations
extern volatile int16_t  o2_raw_adc_count;
extern volatile float    o2_m_raw_voltage;

// for single point calibration
extern volatile float    o2_calib_const;
 
// for two / multi point calibration
extern volatile float    o2_slope;
extern volatile float    o2_offset_value;



extern float y_samples[NUM_OF_SAMPLES_O2];
extern float x_samples[NUM_OF_SAMPLES_O2];


// Function declarations -----------------------------------------------------
void multi_point_calibration        (void);
void two_point_calibration			(void);
void single_point_calibration		(void);
void test_ads1115                   (void);



#endif  //  '#ifndef   o2_sensor_h' ends
