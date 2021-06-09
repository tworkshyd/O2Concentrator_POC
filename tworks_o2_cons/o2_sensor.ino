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

    //pinMode(2, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(2), adsReady, RISING);

    // ads1015.setGain(GAIN_TWOTHIRDS); // 2/3x gain +/- 6.144V  1 bit = 3mV (default)
    // ads1015.setGain(GAIN_ONE);       // 1x gain   +/- 4.096V  1 bit = 2mV
    // ads1015.setGain(GAIN_TWO);       // 2x gain   +/- 2.048V  1 bit = 1mV
    // ads1015.setGain(GAIN_FOUR);      // 4x gain   +/- 1.024V  1 bit = 0.5mV
    // ads1015.setGain(GAIN_EIGHT);     // 8x gain   +/- 0.512V  1 bit = 0.25mV
    // ads1015.setGain(GAIN_SIXTEEN);   // 16x gain  +/- 0.256V  1 bit = 0.125mV

    ADS.begin();
    // ADS.setGain(0);      // 6.144 volt
    ADS.setGain(1);         // GAIN_ONE (for an input range of +/-4.096V)
    //ADS.setGain(GAIN_SIXTEEN);         // GAIN_SIXTEEN (for an input range of +/-4.096V)

    ADS.setDataRate(7);     // slow

    // SET ALERT RDY PIN
    //ADS.setComparatorThresholdHigh(0x8000);
    //ADS.setComparatorThresholdLow(0x0000);
    //ADS.setComparatorQueConvert(0);

    // SET INTERRUPT HANDLER TO CATCH CONVERSION READY
    //pinMode(2, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(ADS_READY_INTR), adsReady, RISING);

    ADS.setMode(0);             // continuous mode
    // ADS.readADC(channel);    // trigger first read
    ADS.readADC(0);


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

/*
    void o2_sensor_scan (void)  {

    float   m_raw_voltage;



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
*/

/*
    main function to calibrate and get m (slope) and c (constant) after curve fitting
    read y data from eeprom
    convert from byte to float
    use in algo to calc m and c values.
*/
#define NUM_OF_SAMPLES_O2   (3)
//float x_samples[NUM_OF_SAMPLES_O2] = {28.375, 141.75, 633.75};
float x_samples[NUM_OF_SAMPLES_O2] = {227.0, 1134.0, 5069.0};
float y_samples[NUM_OF_SAMPLES_O2] = {0.0, 20.9, 100.0};
int sensor_zero_calibration (void)
{

    float x = 0, sigmaX = 0, sigmaY = 0, sigmaXX = 0, sigmaXY = 0, denominator = 0, y = 0;
    int value = 0;
    // int eeprom_addr = EEPROM_O2_CALIB_ADDR;
    int result;

    result = SUCCESS;

    // VENT_DEBUG_FUNC_START();

    for (int index = 0; index < NUM_OF_SAMPLES_O2; index++)
    {

        value = y_samples[index];
        // y = ((float)value) / 1000;
        y = ((float)value);

        // x = (float(x_samples[index])) / 10;
        x = (float(x_samples[index]));
        sigmaX += x;
        sigmaY += y;
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
    } else {
        o2_slope = 0;
        o2_const_val = 0;
        result =  ERROR_SENSOR_CALIBRATION;
        // VENT_DEBUG_ERROR("Error: O2 calibration failed!!", result);
        Serial.println ("Error: O2 calibration failed!!");

    }

    Serial.println();
    Serial.print ("o2_slope : ");
    Serial.print (o2_slope);
    Serial.print (", o2_const_val : ");
    Serial.println (o2_const_val);


    // VENT_DEBUG_FUNC_END();
    return result;
}


void o2_sensor_scan (void)  {

    float   m_raw_voltage;


    handleConversion();
//    Serial.print("o2_raw_adc_count : ");
//    Serial.print(o2_raw_adc_count);


    m_raw_voltage = ((float)o2_raw_adc_count * 1000.0) * 0.000125;
//    Serial.print(", m_raw_voltage : ");
    // Serial.print(m_raw_voltage);
//    Serial.print(m_raw_voltage, 4);

    // temp hard coding till calib menu is ready
    // o2_slope = 0.02065262;  // 48.42; // 0.166;
    // o2_const_val = -4.68815;  //227; //1.3228;
    //    o2_concentration = ((m_raw_voltage * o2_slope) + o2_const_val);
    o2_concentration = ((o2_raw_adc_count * o2_slope) + o2_const_val);

//    Serial.print(", o2_concentration : ");
    // Serial.print(o2_concentration);
//    Serial.print(o2_concentration, 4);
//    Serial.println();

}

// -- END OF FILE --
