//Pressure Sensors
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;  /* ADS1015 */

const float sensorRange = 4.5; // sensor Vout (0-4,5V)

int16_t adc0, adc1, adc2, adc3;
float volts0, volts1, volts2, volts3;
float pressure1, pressure2, pressure3;

float baselineVoltage1, baselineVoltage2, baselineVoltage3;

void setup(void) {
  Serial.begin(115200);
 
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
 
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  //ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
    yield();
  }
}

void loop() {
  pressureSensor();
}

void pressureSensor() {
  adc0 = ads.readADC_SingleEnded(0); // Read ADC
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

   
  // ADC to Voltage
  //volts0 = adc0 * 0.000125; (sensorRange / 32767.0); // ADS1115 memiliki resolusi 16-bit (32767)
  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
  volts2 = ads.computeVolts(adc2); //adc2 * (sensorRange / 32767.0);
  volts3 = ads.computeVolts(adc3);

  // 1740.45 adalah konversi max tekanan sensor = 1.2 MPA
  pressure1 = (volts1 - baselineVoltage1) * (174.00 / (4.5 - baselineVoltage1)) - 1.50;
  pressure2 = (volts2 - baselineVoltage2) * (174.00 / (4.5 - baselineVoltage2)) - 0.50;
  pressure3 = (volts3 - baselineVoltage3) * (174.00 / (4.5 - baselineVoltage3)) - 1.70;

  baselineVoltage1 = 0.515; //at zero pressure
  baselineVoltage2 = 0.525;
  baselineVoltage3 = 0.505;

  if (pressure1 < 0) {
        pressure1 = 0; // Set nilai P menjadi 0 jika negatif
    } 

  if (pressure2 < 0) {
        pressure2 = 0; // Set nilai P menjadi 0 jika negatif
    } 

  if (pressure3 < 0) {
        pressure3 = 0; // Set nilai P menjadi 0 jika negatif
    } 
  
  delay(8000);
}
