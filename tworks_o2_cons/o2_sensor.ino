// o2_sensor.c


#include "platform.h"
#include "o2_sensor.h"
#include "ADS1X15.h"


volatile bool RDY = false;
uint8_t channel = 0;
//int16_t val[4] = { 0, 0, 0, 0 };
int16_t o2_raw_adc_count;
float o2_slope;
float o2_const_val;


ADS1115 ADS(O2_SENSOR_ADS_ADDR);

void ads_init (void)  {

    Serial.print("ADS1X15_LIB_VERSION: ");
    Serial.println(ADS1X15_LIB_VERSION);

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), adsReady, RISING);

    ADS.begin();
    // ADS.setGain(0);      // 6.144 volt
    ADS.setGain(1);         // GAIN_ONE (for an input range of +/-4.096V) 
    ADS.setDataRate(7);     // slow

    // SET ALERT RDY PIN
    ADS.setComparatorThresholdHigh(0x8000);
    ADS.setComparatorThresholdLow(0x0000);
    ADS.setComparatorQueConvert(0);

    // SET INTERRUPT HANDLER TO CATCH CONVERSION READY
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ADS_READY_INTR), adsReady, RISING);

    ADS.setMode(0);        // continuous mode
    ADS.readADC(channel);     // trigger first read
}


void adsReady()
{
    RDY = true;
}

void handleConversion()
{
    //  if (RDY)
    //{
    // save the value
    // val[channel] = ADS.getValue();
    // request next channel
    // channel++;
    // if (channel >= 4) channel = 0;
    // ADS.readADC(channel);
    // RDY = false;
    //}

    ADS.readADC(0);
    o2_raw_adc_count = ADS.getValue();

}


void o2_sensor_scan (void)  {

    float   m_raw_voltage;
    /*
        for (int i = 0; i < 4; i++)
        {
            Serial.print(val[i]);
            Serial.print('\t');
            handleConversion();
        }
        Serial.println();
    */


    //delay(100);

    handleConversion();
    Serial.print("o2_raw_adc_count : ");
    Serial.print(o2_raw_adc_count);
        
    // from ventilator code for Envitech O2 sensor
    // o2_value = ((m_raw_voltage*0.166) + 1.3228); // Envitech
    // m_raw_voltage = o2_raw_adc_count * 1000;
    // m_raw_voltage = ((o2_raw_adc_count * 5.0) / 1024);
    // m_raw_voltage = ((o2_raw_adc_count * 5.0) / 4095);
    // m_raw_voltage = ((o2_raw_adc_count * 0.000125)
    m_raw_voltage = ((float)o2_raw_adc_count * 1000.0) * 0.000125;
    Serial.print(", m_raw_voltage : ");
    // Serial.print(m_raw_voltage);
    Serial.print(m_raw_voltage, 4);
    
    // temp hard coding till calib menu is ready
    o2_slope = 0.166;
    o2_const_val = 1.3228;
    o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);
    Serial.print(", o2_concentration : ");
    // Serial.print(o2_concentration);
    Serial.print(o2_concentration, 4);
    Serial.println();

}




// -- END OF FILE --
