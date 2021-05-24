// o2_sensor.c


#include "platform.h"
#include "o2_sensor.h"
#include "ADS1X15.h"


volatile bool RDY = false;
uint8_t channel = 0;
int16_t val[4] = { 0, 0, 0, 0 };

ADS1115 ADS(O2_SENSOR_ADS_ADDR);

void ads_init (void)  {

    Serial.print("ADS1X15_LIB_VERSION: ");
    Serial.println(ADS1X15_LIB_VERSION);

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), adsReady, RISING);

    ADS.begin();
    ADS.setGain(0);        // 6.144 volt
    ADS.setDataRate(7);    // slow

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
    {
        // save the value
        val[channel] = ADS.getValue();
        // request next channel
        channel++;
        if (channel >= 4) channel = 0;
        ADS.readADC(channel);
        RDY = false;
    }
}


void o2_sensor_scan (void)  {

    for (int i = 0; i < 4; i++)
    {
        Serial.print(val[i]);
        Serial.print('\t');
        handleConversion();
    }
    Serial.println();
    //delay(100);

}


// -- END OF FILE --
