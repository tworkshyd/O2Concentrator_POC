// o2_sensor.c


#include "platform.h"
#include "o2_sensor.h"
#include "ADS1X15.h"



#define NUM_OF_SAMPLES_O2   (3)

float y_samples[NUM_OF_SAMPLES_O2]   = {0.0, 20.9, 100.0};
//float x_samples[NUM_OF_SAMPLES_O2] = {28.375, 141.75, 633.75};
float x_samples[NUM_OF_SAMPLES_O2]   = {224.0, 1083.0, 4888.0};




ADS1115 ADS(O2_SENSOR_ADS_ADDR);

void ads_init (void)  {

    DBG_PRINT("ADS1X15_LIB_VERSION: ");
    DBG_PRINTLN(ADS1X15_LIB_VERSION);

    // ads1015.setGain(GAIN_TWOTHIRDS); // 2/3x gain +/- 6.144V  1 bit = 3mV (default)
    // ads1015.setGain(GAIN_ONE);       // 1x gain   +/- 4.096V  1 bit = 2mV
    // ads1015.setGain(GAIN_TWO);       // 2x gain   +/- 2.048V  1 bit = 1mV
    // ads1015.setGain(GAIN_FOUR);      // 4x gain   +/- 1.024V  1 bit = 0.5mV
    // ads1015.setGain(GAIN_EIGHT);     // 8x gain   +/- 0.512V  1 bit = 0.25mV
    // ads1015.setGain(GAIN_SIXTEEN);   // 16x gain  +/- 0.256V  1 bit = 0.125mV

    ADS.begin();
    ADS.setGain(1);         // GAIN_ONE (for an input range of +/-4.096V)
    ADS.setDataRate(7);     // slow

    ADS.setMode(0);         // continuous mode
    ADS.readADC(O2_SENSOR_CHANNEL_NO);  

}




/*
    main function to calibrate and get m (slope) and c (constant) after curve fitting
    read y data from eeprom
    convert from byte to float
    use in algo to calc m and c values.
*/
int sensor_zero_calibration (void)
{

    float x = 0, y = 0;
    float sigmaX = 0, sigmaY = 0;
    float sigmaXX = 0, sigmaXY = 0;
    float denominator = 0;    
    int   value = 0;
    int   result;

    result = SUCCESS;

    for (int index = 0; index < NUM_OF_SAMPLES_O2; index++)
    {

        y = y_samples[index];
        x = x_samples[index];
        
        sigmaX  += x;
        sigmaY  += y;
        sigmaXX += x * x;
        sigmaXY += x * y;

        DBG_PRINT ("x : "); DBG_PRINT (x); DBG_PRINT (", y : "); DBG_PRINTLN (y);
        DBG_PRINT ("sigmaX : "); DBG_PRINT (sigmaX); DBG_PRINT (", sigmaY : "); DBG_PRINTLN (sigmaY);
        DBG_PRINT ("sigmaXX : "); DBG_PRINT (sigmaXX); DBG_PRINT (", sigmaXY : "); DBG_PRINTLN (sigmaXY);

    }
    
    denominator = (NUM_OF_SAMPLES_O2 * sigmaXX) - (sigmaX * sigmaX);
    DBG_PRINT ("denominator : "); DBG_PRINTLN (denominator);
    
    if (denominator != 0) {      
        o2_slope = ((NUM_OF_SAMPLES_O2 * sigmaXY) - (sigmaX * sigmaY)) / denominator;
        o2_const_val = ((sigmaY * sigmaXX) - (sigmaX * sigmaXY)) / denominator;
        result = SUCCESS;
        
    } 
    else {      
        o2_slope = 0;
        o2_const_val = 0;
        result =  ERROR_SENSOR_CALIBRATION;
        DBG_PRINTLN ("Error: O2 calibration failed!!");

    }

    DBG_PRINTLN ("");
    DBG_PRINT   ("o2_slope : ");
    DBG_PRINT   (o2_slope);
    DBG_PRINT   (", o2_const_val : ");

    return result;
    
}


void o2_sensor_scan (void)  {

    ADS.readADC(O2_SENSOR_CHANNEL_NO);
    o2_raw_adc_count = ADS.getValue();
    
    DBG_PRINT("o2_raw_adc_count : ");
    DBG_PRINT(o2_raw_adc_count);


    m_raw_voltage = ((float)o2_raw_adc_count * 1000.0) * 0.000125;
    DBG_PRINT(", m_raw_voltage : ");
    DBG_PRINT(m_raw_voltage, 4);

    // temp hard coding till calib menu is ready
    // o2_slope = 0.02065262;  // 48.42; // 0.166;
    // o2_const_val = -4.68815;  //227; //1.3228;
    
    // o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);
    o2_concentration = ((o2_raw_adc_count * o2_slope) + o2_const_val);

    DBG_PRINT(", o2_concentration : ");
    DBG_PRINT(o2_concentration);
    DBG_PRINT(o2_concentration, 4);
    DBG_PRINTLN("");

}




// -- END OF FILE --
