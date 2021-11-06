// tempr_sensor.c


#include "tempr_sensor.h"


float   tempr_value_1 = 30.5;
float   tempr_value_2 = 25.3;
int     tempr1_raw_adc_count;
int     tempr2_raw_adc_count;

float V;
float temp;
float Rx;

// Variables to convert voltage to resistance
float C = 79.489;
float slope = 14.187;

// Variables to convert resistance to temp
float R0 = 100.0;
float alpha = 3.85; //0.00385;



void tempr_sensor_scan (void)  {

    DBG_PRINTLN ();

    // Senor - 1
    ADS.readADC(TEMPR_SENSOR_1_CHANNEL_NO);
    tempr1_raw_adc_count = ADS.getValue();   
    DBG_PRINT   ("tempr1_raw_adc_count : ");
    DBG_PRINTLN (tempr1_raw_adc_count);

    // bits to voltage
    //m_raw_voltage = ((float)tempr1_raw_adc_count) * 0.000125;
    m_raw_voltage = ((float)tempr1_raw_adc_count) * 0.125;
    DBG_PRINT   ("m_raw_voltage    : ");
    DBG_PRINTLN (m_raw_voltage, 4);

    // Voltage to resistance
    Rx = m_raw_voltage * slope + C;     // y = mx + c
    // Resistance to Temperature
    tempr_value_1 = (Rx / R0 - 1.0) / alpha;     // from Rx = R0 (1 + alpha * X)
    
    // Uncommect to convet celsius to fehrenheit
    // tempr_value_1 = tempr_value_1 * 1.8 + 32; 
    
    // Serial.println(tempr_value_1);
    DBG_PRINT   ("tempr_value_1 : ");
    DBG_PRINTLN (tempr_value_1, 4);
    DBG_PRINTLN ();

    // Senor - 2
    ADS.readADC(TEMPR_SENSOR_2_CHANNEL_NO);
    tempr2_raw_adc_count = ADS.getValue();   
    DBG_PRINT   ("tempr2_raw_adc_count : ");
    DBG_PRINTLN (tempr2_raw_adc_count);

    // bits to voltage
    //m_raw_voltage = ((float)tempr2_raw_adc_count) * 0.000125;
    m_raw_voltage = ((float)tempr2_raw_adc_count) * 0.125;
    DBG_PRINT   ("m_raw_voltage    : ");
    DBG_PRINTLN (m_raw_voltage, 4);

    // Voltage to resistance
    Rx = m_raw_voltage * slope + C;     // y = mx + c
    // Resistance to Temperature
    tempr_value_2 = (Rx / R0 - 1.0) / alpha;     // from Rx = R0 (1 + alpha * X)
    
    // Uncommect to convet celsius to fehrenheit
    // tempr_value_2 = tempr_value_2 * 1.8 + 32; 
    
    // Serial.println(tempr_value_2);
    DBG_PRINT   ("tempr_value_2 : ");
    DBG_PRINTLN (tempr_value_2, 4);

    DBG_PRINTLN ();
    
    
/*
    // temp hard coding till calib menu is ready
    // o2_slope = 0.02065262;  // 48.42; // 0.166;
    // o2_const_val = -4.68815;  //227; //1.3228;
    
    o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);



    #if (CAPP_AT_95_O2 == 1)
    // capping O2C value.. to restrict it below.. 95% of FiO2   
    if (o2_concentration > O2C_CAP_VALUE_MAX) {
        o2_concentration = O2C_CAP_VALUE_MAX;
        DBG_PRINT(", Capping limit hit-->");
    }  
    #endif

    
    DBG_PRINT   ("o2_concentration : ");
    DBG_PRINTLN (o2_concentration, 4);
*/

}





















////// EOF
