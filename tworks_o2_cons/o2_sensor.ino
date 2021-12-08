// o2_sensor.c


#include "platform.h"
#include "o2_sensor.h"
#include "ADS1X15.h"
#include "config.h"


#define NUM_OF_SAMPLES_O2   (3)


#if  (O2_SENSOR == 1)
    // 1. for Envitec sensor marked as S1 for demo device
    float y_samples[NUM_OF_SAMPLES_O2]     = {  5.0,    21.1,   96.0};      // based on o2c tank purity = 96% & nitrogen tank purity = 95%
    float x_samples[NUM_OF_SAMPLES_O2]     = { 2.3750, 125.125, 661.625};   // default calib values

#elif  (O2_SENSOR == 2) 
    // 2. for Envitec sensor marked as S2 for demo device
    //float y_samples[NUM_OF_SAMPLES_O2]   = {  4.0,    20.0,   99.7};
    //float y_samples[NUM_OF_SAMPLES_O2]   = {  0.0,    21.1,   100.0};
    float y_samples[NUM_OF_SAMPLES_O2]     = {  5.0,    21.1,   96.0};      // based on o2c tank purity = 96% & nitrogen tank purity = 95%
    float x_samples[NUM_OF_SAMPLES_O2]     = {  8.75, 129.75,  601.75};     // default calib values

#elif  (O2_SENSOR == 3) 
    // 3. for Envitec sensor marked as S3 for demo device
    float y_samples[NUM_OF_SAMPLES_O2]     = {  5.0,    21.1,   96.0};      // based on o2c tank purity = 96% & nitrogen tank purity = 95%
    float x_samples[NUM_OF_SAMPLES_O2]     = { 10.375, 129.25,  588.125};   // default calib values

#else 
    float y_samples[NUM_OF_SAMPLES_O2]     = {  5.0,    21.1,   96.0};      // based on o2c tank purity = 96% & nitrogen tank purity = 95%
    float x_samples[NUM_OF_SAMPLES_O2]     = { 10.375, 129.25,  588.125};   // default calib values

#endif



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

void test_ads1115 (void)  {
  
    ADS.setGain(0);
    
    while (1)
    {

        int16_t val_0 = ADS.readADC(0);  
        int16_t val_1 = ADS.readADC(1);  
        int16_t val_2 = ADS.readADC(2);  
        int16_t val_3 = ADS.readADC(3);  
        
        float f = ADS.toVoltage(1);  // voltage factor
        
        Serial.print("\tAnalog0: "); Serial.print(val_0); Serial.print('\t'); Serial.println(val_0 * f, 3);
        Serial.print("\tAnalog1: "); Serial.print(val_1); Serial.print('\t'); Serial.println(val_1 * f, 3);
        Serial.print("\tAnalog2: "); Serial.print(val_2); Serial.print('\t'); Serial.println(val_2 * f, 3);
        Serial.print("\tAnalog3: "); Serial.print(val_3); Serial.print('\t'); Serial.println(val_3 * f, 3);
        Serial.println();
        
        delay(1000);
    }
  
}



/*
    main function to calibrate and get m (slope) and c (constant) after curve fitting
    read y data from eeprom
    convert from byte to float
    use in algo to calc m and c values.
*/
int sensor_zero_calibration (void)  {

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

        DBG_PRINT ("      x : "); DBG_PRINTLN (x);        DBG_PRINT ("      y : "); DBG_PRINTLN (y);
        DBG_PRINT (" sigmaX : "); DBG_PRINTLN (sigmaX);   DBG_PRINT (" sigmaY : "); DBG_PRINTLN (sigmaY);
        DBG_PRINT ("sigmaXX : "); DBG_PRINTLN (sigmaXX);  DBG_PRINT ("sigmaXY : "); DBG_PRINTLN (sigmaXY);
    }
    
    denominator = (NUM_OF_SAMPLES_O2 * sigmaXX) - (sigmaX * sigmaX);
    DBG_PRINT ("dnmnatr : "); DBG_PRINTLN (denominator);

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
    DBG_PRINT   (o2_const_val);

    return result;
    
}


void o2_sensor_scan (void)  {

    ADS.readADC(O2_SENSOR_CHANNEL_NO);
    o2_raw_adc_count = ADS.getValue();   
    // DBG_PRINT   ("o2_raw_adc_count : ");
    // DBG_PRINTLN (o2_raw_adc_count);

    m_raw_voltage = ((float)o2_raw_adc_count * 1000.0) * 0.000125;
    // DBG_PRINT   ("m_raw_voltage    : ");
    // DBG_PRINTLN (m_raw_voltage, 4);

    // temp hard coding till calib menu is ready
    // o2_slope     = 0.02065262;       //48.42;    //0.166;
    // o2_const_val = -4.68815;         //227;      //1.3228;
    
    o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);



    #if (CAPP_AT_95_O2 == 1)
    // capping O2C value.. to restrict it below.. 95% of FiO2   
    if (o2_concentration > O2C_CAP_VALUE_MAX) {
        o2_concentration = O2C_CAP_VALUE_MAX;
        DBG_PRINT(", Capping limit hit-->");
    }  
    #endif

    
    // DBG_PRINT   ("o2_concentration : ");
    // DBG_PRINTLN (o2_concentration, 4);


}




// -- END OF FILE --
