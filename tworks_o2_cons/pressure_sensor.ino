// pressure_sensor.c



#include "pressure_sensor.h"
#include "platform.h"
#include "db.h"



/*
    Darren Lewis calculated pressure readings MPX5010 based on datasheet
    datasheet: https://docs.rs-online.com/9979/0900766b8138443c.pdf
    my website: http://darrenlewismechatronics.co.uk/
*/

/*
const float ADC_mV       = 4.8828125;   // convesion multiplier from Arduino ADC value to voltage in mV
const float SensorOffset = 200.0;       // in mV taken from datasheet
const float sensitivity  = 4.413;       // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O  = 10;          // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa    = 0.00981;     // convesion multiplier from mmH2O to kPa
*/


/*
    calculated pressure readings MPX5100 based on datasheet
    datasheet:https://www.nxp.com/docs/en/data-sheet/MPX5100.pdf
*/

// VOUT = VS*(0.009*P+0.04), for MPX5100

// const float ADC_mV       = 4.8828125;   // convesion multiplier from Arduino ADC value to voltage in mV
// const float SensorOffset = 200.0;       // in mV taken from datasheet
// const float sensitivity  = 4.50;        // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O  = 10;          // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa    = 0.00981;     // convesion multiplier from mmH2O to kPa


void read_pressure (void) {


// volatile int16_t  pressure_raw_adc_count;
// volatile float    pressure_slope;
// volatile float    pressure_const_val;
// volatile float    pressure_raw_m_voltage;

    // ((sensorValue in mV -sensorOffset) / sensitivity in mV/mmH2O)*kPa conversion constant
    // output_pressure = ((analogRead(A0) * ADC_mV - SensorOffset) / sensitivity * mmh2O_kpa);     // result in kPa
//    output_pressure = (analogRead(Prss_sns_1_adc));

    ADS.readADC(PRESSURE_SENSOR_CHANNL_NO);
    pressure_raw_adc_count = ADS.getValue();
    DBG_PRINT   ("pressure_raw_adc_count    : ");
    DBG_PRINTLN (pressure_raw_adc_count);

    pressure_raw_m_voltage = ((float)pressure_raw_adc_count * 1000.0) * 0.000125;
    DBG_PRINT   ("pressure_raw_m_voltage    : ");
    DBG_PRINTLN (pressure_raw_m_voltage, 4);
// const float ADC_mV       = 4.8828125;   // convesion multiplier from Arduino ADC value to voltage in mV
// const float SensorOffset = 200.0;       // in mV taken from datasheet
// const float sensitivity  = 4.50;        // in mV/mmH2O taken from datasheet


    // temp hard coding till calib menu is ready
    pressure_slope     = 4.50;       
    pressure_const_val = 200.0;         //227;      //1.3228;
    
    output_pressure = ((pressure_raw_m_voltage * pressure_slope) + pressure_const_val);


    Serial.print("output_pressure : ");
    Serial.print(output_pressure);
//    output_pressure = (output_pressure * pressure_raw_m_voltage - SensorOffset) / sensitivity / mmh2O_cmH2O;       // result in cmH2O
    output_pressure = (output_pressure * pressure_raw_m_voltage - pressure_const_val) / pressure_slope / mmh2O_cmH2O;       // result in cmH2O

    //Serial.print(millis());
    Serial.print(" , ");
    Serial.println(output_pressure, 2);
    // delay(500);
    // using serial print format above, you can copy your data from the terminal,
    // paste it into NotePad app, than save as a .CSV file which can be opened in Excel
    
}
