#include <Wire.h>                       // Wire library - used for I2C communication
#include <math.h>                       // Math library - used for math calculations

// ------------------------- Declare Constants ------------------------- //
#define STATUS        0x04              // define STATUS as 0x04
#define XDATA3        0x08              // define XDATA3 as 0x08
#define XDATA2        0x09              // define XDATA2 as 0x09
#define XDATA1        0x0A              // define XDATA1 as 0x0A
#define YDATA3        0x0B              // define YDATA3 as 0x0B
#define YDATA2        0x0C              // define YDATA2 as 0x0C
#define YDATA1        0x0D              // define YDATA1 as 0x0D
#define ZDATA3        0x0E              // define ZDATA3 as 0x0E
#define ZDATA2        0x0F              // define ZDATA2 as 0x0F
#define ZDATA1        0x10              // define ZDATA1 as 0x10

#define RANGE         0x2C              // define RANGE as 0x2C
#define POWER_CTL     0x2D              // define POWER_CTL as 0x2D, power_register address
#define RANGE_2G      2.048             // define RANGE_2G as decimal 2.048m
#define RANGE_4G      4.096             // define RANGE_3G as decimal 4.096
#define RANGE_8G      8.192             // define RANGE_8G as decimal 8.192

// ------------------------- Declare Global Variables ------------------------- // 
int ADXL355 = 0x1D;                     // ADXL355 I2C address - 0x53 or 0x1D
float x,y,z;                            // x,y,z initiated as float
float range;                            // range initiated as float
float rollF,pitchF=0;                   // rollF,pitchF initiated as float
uint8_t enable(uint8_t);                // enable initiated as an unidentified integer

void setup() 
{   
  Serial.begin(500000);
  Wire.begin();
  Wire.setClock(1000000); 

  TCA9548A_deselect();

  delayMicroseconds(1000);

  TCA9548A(0);
  enable(0x00);

  TCA9548A(1);
  enable(0x00);

  TCA9548A(2);
  enable(0x00);

  TCA9548A(3);
  enable(0x00);
}

// Main loop
void loop() 
{
  int x1=0, x2=0, x3=0, x4=0, y1=0, y2=0, y3=0, y4=0, z1=0, z2=0, z3=0, z4=0;

  while(true){
    TCA9548A(0);
    x1 = readOrientation(XDATA3);
    y1 = readOrientation(YDATA3);
    z1 = readOrientation(ZDATA3);
  
    TCA9548A(1);
    x2 = readOrientation(XDATA3);
    y2 = readOrientation(YDATA3);
    z2 = readOrientation(ZDATA3);
      
    TCA9548A(2);
    x3 = readOrientation(XDATA3);
    y3 = readOrientation(YDATA3);
    z3 = readOrientation(ZDATA3);
    
    TCA9548A(3);
    x4 = readOrientation(XDATA3);
    y4 = readOrientation(YDATA3);
    z4 = readOrientation(ZDATA3);/**/
    
    Serial.println(String(x1) + "\t" + String(y1) + "\t" + String(z1) + "\t" + String(x2) + "\t" + String(y2) + "\t" + String(z2) + "\t" + String(x3) + "\t" + String(y3) + "\t" + String(z3) + "\t" + String(x4) + "\t" + String(y4) + "\t" + String(z4));
    
    delayMicroseconds(5);
  }
}
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);                     // TCA9548A address is 0x70
  Wire.write(1 << bus);                             // send byte to select bus
  Wire.endTransmission();
}

void TCA9548A_deselect(){
  Wire.beginTransmission(0x70);                     // TCA9548A address is 0x70
  Wire.write(0x00);                                 // send byte to select bus
  Wire.endTransmission();
}

// function readOrientation
int readOrientation(unsigned long orientation) {
  Wire.beginTransmission(ADXL355);                  // begin data transmission from ADXL355
  Wire.write(orientation);                          // write hexadecimal constants XDATA3, YDATA3 or ZDATA3 containing x, y or z
  Wire.endTransmission();                           // end data transmission
  Wire.requestFrom((int)ADXL355,3);                 // request 3 registers from ADXL355
  uint32_t v1 = Wire.available()?Wire.read():0;     // Ternary operator - if wire is available then read value of v1 else write 0
  uint32_t v2 = Wire.available()?Wire.read():0;     // Ternary operator - if wire is available then read value of v2 else write 0
  uint32_t v3 = Wire.available()?Wire.read():0;     // Ternary operator - if wire is available then read value of v3 else write 0 
  uint32_t data = (v1<<16)|(v2<<8)|v3;              // Bit shift v1 16 bits to the left, v2 8 bits to the left
  data = data>>4;                                   // Bit shift data 4 bits to the right
  
  int value = (int)(data > 0X7FFFF ? data - 0XFFFFE : data);
  return value;  
}

// function enable
uint8_t enable(uint8_t c = 0X00) { 
  Wire.beginTransmission(ADXL355);                  // begin data transmission from ADXL355m
  if (Wire.endTransmission()==0) {                  // if end data transmission from ADXL355 is equal to 0, then execute the next code
    Wire.beginTransmission(ADXL355);                // begin data transmission from ADXL355
    Wire.write(0X00);                               // write address 0X00
    Wire.endTransmission();                         // end data transmission
    Wire.requestFrom((int)ADXL355,1);               // request 1 register from ADXL355
    if (Wire.available()) {                         // if data transmission is available, then execute the next codem
      if (Wire.read()==0XAD) {                      // if the readings from the data is equal to 0XAD, then execute the next code
        Serial.println("Device found");               // print "Device found" on serial monitor
        
        // Set range to +-2g
        Wire.beginTransmission(ADXL355);
        Wire.write(0x2C);                           // RANGE register address
        Wire.write(0x01);                           // Set to +-2g (01 value)
        Wire.endTransmission();
        
        // Set filter to 2000 Hz and 500 Hz (ODR_LPF setting 0001 from the image)
        Wire.beginTransmission(ADXL355);
        Wire.write(0x28);                           // FILTER register address
        Wire.write(0x01);                           // Set to 0001 (2000 Hz and 500 Hz)
        Wire.endTransmission();
      }
      else {
        Serial.println("Device not found");
        return 1;                                   // return 1
      }
    }
  }
  else {
    return 0;                                       // return 0
  }
  Wire.beginTransmission(ADXL355);                  // begin data transmission from ADXL355
  Wire.write(POWER_CTL);                            // writes address 0X2D from the accelerometer to the Arduino
  Wire.write(c);                                    // write 0X00
  Wire.endTransmission();                           // end data transmission
  return 1;                                         // return 1
}

// function reset
void reset() {
  Wire.beginTransmission(ADXL355);                  // begin data transmission from ADXL355
  Wire.write(0x2f);                                 // writes address 0X2f from the accelerometer to the Arduino
  Wire.write(0x52);                                 // writes code 0x52 to reset the device
  Wire.endTransmission();                           // end data transmission
}

float readAxis() {
  float value;                                      // value is a float
  uint32_t d3,d2,d1,data;                           // d3,d2,d1,data are uint32_t - unidentified integers, 32 bits/4 bytes
  
  d3 = Wire.available()?Wire.read():0;              // Ternary operator - if wire is available then read value of d3 else write 0
  d2 = Wire.available()?Wire.read():0;              // Ternary operator - if wire is available then read value of d2 else write 0
  d1 = Wire.available()?Wire.read():0;              // Ternary operator - if wire is available then read value of d1 else write 0
  data = (d3<<16)|(d2<<8)|d1;                       // Bit shift d3 16 bits to the left, d2 8 bits to the left
  data = data>>4;                                   // Bit shift data 4 bits to the right
  value = (float)data;                              // value = data
  value = data>0X7FFFF?value-0XFFFFE:value;         // Ternary operator - if data is greater than 0x7FFFF then value-0XFFFFE else write value
  return value/0X7FFFF*RANGE_2G;                    // returns value divided by 0X7FFFF times RANGE_2G, which is decimal 2.048
}

uint8_t dataReady() {
  Wire.beginTransmission(ADXL355);                  // begin data transmission from ADXL355
  Wire.write(STATUS);                               // writes address 0X04 from the accelerometer to the Arduino
  Wire.endTransmission();                           // end data transmission
  Wire.requestFrom(ADXL355,1);                      // request 1 register from ADXL355
  return Wire.available()?Wire.read()&0X01:0X00;    // returns value from STATUS and write 0X01 if wire is available, else write 0X00
}
