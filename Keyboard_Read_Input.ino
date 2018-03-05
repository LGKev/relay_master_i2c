
/*
  Take user input from keyboard. When q is sent Turn on RELAY, when p is sent Turn off relay.


*/

#include <TinyWire.h>
#ifdef UNO
#define DEBUG_OUTPUT
#endif

#define REGISTER_MAP_SIZE    4
#define BIT0              0b00000001
#define I2C_ALIVE_INDICATOR   13  //LED13

int incomingByte = 0;   // for incoming serial data
int LATEST_ADDRESS = 0x01;     //global so address can be changed by user.
byte x = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("master awake");
<<<<<<< HEAD

  pinMode(13, OUTPUT);
  TinyWire.begin(); // join i2c bus (address optional for master)
=======
  pinMode(I2C_ALIVE_INDICATOR, OUTPUT);
  Wire.begin(); // join i2c bus (address optional for master)
>>>>>>> kjk_simple_reply
}

void loop() {

  // what if serial is affecting i2c when reading keyboard input
  //ltes just toggle the relay via i2c 2 seconds on 2 seconds off
  //~~~~~~ RELAY ON
  Wire.beginTransmission(0x01);
  digitalWrite(I2C_ALIVE_INDICATOR, HIGH);
  Wire.write(0x01);   //send to the on register
  Wire.write(1);      //write a 1 to the ON REGISTER @ 0x01.
  Wire.endTransmission();   //stop transmit condition.
  delay(110);
  
  //~~~~~~ RELAY OFF
  Wire.beginTransmission(0x01);
  digitalWrite(I2C_ALIVE_INDICATOR, LOW);
  Wire.write(0x01);   //send to the on register
  Wire.write(0);      //write a 1 to the ON REGISTER @ 0x01.
  Wire.endTransmission();   //stop transmit condition.
  delay(110);
    
}


/*   ==================     ==================     ==================  */
/*   ==================     ==================     ==================  */

/*========================================================*/
//         Helper Functions
/*========================================================*/
/*
    @brief: Starts I2C transmission with a LATEST_ADDRESS, writing to 0x01 register
      wirte a 1 to 0x01 turns on relay
      write a 0 to 0x01 turns off relay
    @input:  none
    @returns: none
    @flags:  none
*/
void relayON() {
  TinyWire.beginTransmission(LATEST_ADDRESS); // transmit to device #1
  TinyWire.send(0x01);        // sends five bytes
  TinyWire.send(1);              // sends one byte
  TinyWire.endTransmission();    // stop transmitting
}
/*
    @brief: Starts I2C transmission with a LATEST_ADDRESS, writing to 0x01 register
      wirte a 1 to 0x01 turns on relay
      write a 0 to 0x01 turns off relay
    @input:  none
    @returns: none
    @flags:  none
*/
void relayOFF() {
  TinyWire.beginTransmission(LATEST_ADDRESS); // transmit to device #1
  TinyWire.send(0x01);        // sends five bytes to the ON register
  TinyWire.send(0);              // sends one byte, set the bit in the ON register on (1) or off (0)
  TinyWire.endTransmission();    // stop transmitting
}

/*
    @brief: Starts I2C transmission with a LATEST_ADDRESS, writing to 0x03 register
        write to the 0x03 register,
        send _address to set slave a new slave address.
        restarts i2c communication with the latest address.
    @input:  _address, the new address for the slave.
    @returns: none
    @flags:  none
*/
void changeAddress(int _address) {
  TinyWire.beginTransmission(LATEST_ADDRESS); // transmit to device #1
  TinyWire.send(0x03);        // sends five bytes to the 0x00 addredss
  LATEST_ADDRESS = _address;
#ifdef DEBUG_OUTPUT
  Serial.print("the current address is: ");
  Serial.println(LATEST_ADDRESS);
#endif
  TinyWire.send(LATEST_ADDRESS);              // sends new address
  TinyWire.endTransmission();    // stop transmitting
  TinyWire.begin(LATEST_ADDRESS);// start wtih the new address.
}
/*
    @brief: Requests data from the slave by writing to the
    @input:  none
    @returns: none
    @flags:  none
*/
void getStatus() {
  TinyWire.requestFrom(LATEST_ADDRESS, REGISTER_MAP_SIZE);    // request 4 bytes from slave device #1
  byte byteCount = 0; //way to know what bytes to throw away.
  while (TinyWire.available()) { // slave may send less than requested
    char c = TinyWire.read(); // receive a byte as character.
    byteCount++; 
    if (byteCount == 3) { //we know that the byteCount needs to be 3 because 
      //we know the status Register is the 3rd element in the array
      if (!(c & 0b00000000)) {
        Serial.println("relay is off");
      }
    else if (c & BIT0) {
        Serial.println("relay is on");
      }
      else{
        Serial.print("???  ");
        Serial.println(c);
      }
    }
  }
#ifdef DEBUG_OUTPUT
  Serial.println();
#endif
}
