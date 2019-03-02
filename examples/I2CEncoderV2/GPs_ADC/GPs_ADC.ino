#include <Wire.h>
#include <i2cEncoderLibV2.h>

/*In this example a standard encoder is used.
 * The GP pins are configured as analog and 
 * the ADC vale is printed on the serial port
 * as well as the counter value
 * 

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/
unsigned long previousMillis = 0;     
const long interval = 1000;           // interval between ADC reading of the GP pins

const int IntPin = A3; // Change according to your board configuration

//Class initialization with the I2C addresses
i2cEncoderLibV2 STDEncoder(0b0001000); //Normal rotary encoder address, A3 is soldered


uint8_t encoder_status, i;
void setup(void)
{
  // Reset the two encoder
  Wire.begin();
  STDEncoder.reset();

  // Initialize the GPIO and the display
  pinMode(IntPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
 
   //Initialize the Serial port
  Serial.begin(115200);
  Serial.print("Encoder Test!!\n");


  //Configure the Standard Encoder
  STDEncoder.begin(INT_DATA | WRAP_DISABLE | DIRE_LEFT | IPUP_ENABLE | RMOD_X1 | STD_ENCODER);
  STDEncoder.writeGP1conf(GP_AN | GP_PULL_DI | GP_INT_DI);  // Configure the GP pins in analog mode
  STDEncoder.writeGP2conf(GP_AN | GP_PULL_DI | GP_INT_DI); // Configure the GP pins in analog mode
  STDEncoder.writeGP3conf(GP_IN | GP_PULL_DI | GP_INT_DI); // Configure the GP pins in analog mode
  STDEncoder.writeCounter((int32_t) 0);
  STDEncoder.writeMax((int32_t) 10);
  STDEncoder.writeMin((int32_t) 0);
  STDEncoder.writeStep((int32_t) 1);
  STDEncoder.writeInterruptConfig(INT2 | RINC | RDEC | RMAX | RMIN); //Enable all the interrupts
  STDEncoder.writeAntibouncingPeriod(20);  /* Set an anti-bouncing of 200ms */
  STDEncoder.updateStatus();

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("GP1: ");
    Serial.println(STDEncoder.readGP1()); //read the analog value of the GP1
    Serial.print("GP2: ");
    Serial.println(STDEncoder.readGP2()); //read the analog value of the GP2
    Serial.print("GP3: ");
    Serial.println(STDEncoder.readGP3()); //read the analog value of the GP3

  }


  if (digitalRead(IntPin) == LOW) { //Check for the interrupt

    if (STDEncoder.updateStatus()) { //Check if the normal encoder is moved
      
      //Print the counter value of the normal encoder
      Serial.print("Encoder: ");
      Serial.println(STDEncoder.readCounterInt());
    }
  }
}