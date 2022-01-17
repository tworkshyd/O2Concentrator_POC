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



void read_pressure (void) {


	ADS.readADC(PRESSURE_SENSOR_CHANNL_NO);
	pressure_raw_adc_count = ADS.getValue();
	// 	DBG_PRINT   ("pressure_raw_adc_count    : ");
	// 	DBG_PRINTLN (pressure_raw_adc_count);
	pressure_raw_m_voltage = ((float)pressure_raw_adc_count * 1000.0) * 0.000125;
	// 	DBG_PRINT   ("pressure_raw_m_voltage    : ");
	// 	DBG_PRINTLN (pressure_raw_m_voltage, 4);

	float mv_at_0psi = 490.87;		// in mV,  0 Psi  = 0 MPa
	float mv_at_23psi = 3722.10;	// in mV, 23 Psi  = 0.16 MPa
	output_pressure = ((pressure_raw_m_voltage - mv_at_0psi) * (23.0 - 0.0)) / (mv_at_23psi - mv_at_0psi);

}

