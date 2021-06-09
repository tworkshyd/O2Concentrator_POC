// o2_sensor.c


#include "platform.h"
#include "o2_sensor.h"
#include "ADS1X15.h"



#define NUM_OF_SAMPLES_O2   (3)

float y_samples[NUM_OF_SAMPLES_O2] = {0.0, 20.9, 100.0};
//float x_samples[NUM_OF_SAMPLES_O2] = {28.375, 141.75, 633.75};
float x_samples[NUM_OF_SAMPLES_O2] = {227.0, 1134.0, 5069.0};

volatile int16_t  o2_raw_adc_count;
volatile float    o2_slope;
volatile float    o2_const_val;


ADS1115 ADS(O2_SENSOR_ADS_ADDR);

void ads_init (void)  {

    Serial.print("ADS1X15_LIB_VERSION: ");
    Serial.println(ADS1X15_LIB_VERSION);

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

        Serial.print ("x : "); Serial.print (x); Serial.print (", y : "); Serial.println (y);
        Serial.print ("sigmaX : "); Serial.print (sigmaX); Serial.print (", sigmaY : "); Serial.println (sigmaY);
        Serial.print ("sigmaXX : "); Serial.print (sigmaXX); Serial.print (", sigmaXY : "); Serial.println (sigmaXY);

    }
    
    denominator = (NUM_OF_SAMPLES_O2 * sigmaXX) - (sigmaX * sigmaX);
    Serial.print ("denominator : "); Serial.println (denominator);
    
    if (denominator != 0) {      
        o2_slope = ((NUM_OF_SAMPLES_O2 * sigmaXY) - (sigmaX * sigmaY)) / denominator;
        o2_const_val = ((sigmaY * sigmaXX) - (sigmaX * sigmaXY)) / denominator;
        result = SUCCESS;
        
    } 
    else {      
        o2_slope = 0;
        o2_const_val = 0;
        result =  ERROR_SENSOR_CALIBRATION;
        Serial.println ("Error: O2 calibration failed!!");

    }

    Serial.println ();
    Serial.print   ("o2_slope : ");
    Serial.print   (o2_slope);
    Serial.print   (", o2_const_val : ");
    Serial.println (o2_const_val);

    return result;
}


void o2_sensor_scan (void)  {

    float   m_raw_voltage;

    ADS.readADC(O2_SENSOR_CHANNEL_NO);
    o2_raw_adc_count = ADS.getValue();
    
    Serial.print("o2_raw_adc_count : ");
    Serial.print(o2_raw_adc_count);


    m_raw_voltage = ((float)o2_raw_adc_count * 1000.0) * 0.000125;
    Serial.print(", m_raw_voltage : ");
    Serial.print(m_raw_voltage, 4);

    // temp hard coding till calib menu is ready
    // o2_slope = 0.02065262;  // 48.42; // 0.166;
    // o2_const_val = -4.68815;  //227; //1.3228;
    
    // o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);
    o2_concentration = ((o2_raw_adc_count * o2_slope) + o2_const_val);

    Serial.print(", o2_concentration : ");
    Serial.print(o2_concentration);
    Serial.print(o2_concentration, 4);
    Serial.println();

}




// -- END OF FILE --
