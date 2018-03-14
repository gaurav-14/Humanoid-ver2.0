#include <Herkulex.h>
#include <Wire.h>
#include "Kalman.h" 

const int N = 2; //no of motor connected
int motor_id[N] = {2,3}; //motor ID - verify your ID !!!!


 // Create the Kalman instances
Kalman kalmanY;


/* IMU Data */
double accX, accY, accZ;
double gyroY;
int16_t tempRaw;

double  gyroYangle; // Angle calculate using the gyro only
double  compAngleY; // Calculated angle using a complementary filter
double  kalAngleY; // Calculated angle using a Kalman filter

double pitch;

double gyroYrate;

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

// TODO: Make calibration routine

void setup() {

  /*Herkulex motor setup*/
  Serial.begin(115200);
  Herkulex.beginSerial1(115200); //open serial port1
  for(int i=0;i<N;i++){ 
  Herkulex.reboot(motor_id[i]); //reboot first motor
    
 
  delay(200);
  };
 Herkulex.initialize(); //initialize motors


  Wire.begin();
  TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));
  accX = (i2cData[0] << 8) | i2cData[1];
  accY = (i2cData[2] << 8) | i2cData[3];
  accZ = (i2cData[4] << 8) | i2cData[5];


 
 pitch = atan2(accX,accZ)* RAD_TO_DEG;;
 
  // Set starting angle
  kalmanY.setAngle(pitch);
 
  gyroYangle = pitch;
  compAngleY = pitch;
 
  timer = micros();
}

void loop() {

  /* Update all the values */
  while (i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  
  tempRaw = (i2cData[6] << 8) | i2cData[7];
  
 
  gyroY = (i2cData[10] << 8) | i2cData[11];
 

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  
   pitch = atan2(accX,accZ)* RAD_TO_DEG;
  
  
   gyroYrate = gyroY / 131.0; // Convert to deg/s
   gyroYrate = gyroY / 131.0;



  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
  
 
  gyroYangle += gyroYrate * dt;
 
  
 
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
  
  // Reset the gyro angle when it has drifted too much
  
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;
  


  /* Print Data */
#if 0 // Set to 1 to activate
  Serial.print(accY); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print("\t");
#endif
    
  Serial.print("p") ; Serial.print("\t");
  Serial.print(kalAngleY); Serial.print("\t");
  
/* Sending KalmanAngleY to motor   */
/*Send only safe limit values to herkulex_motor*/
if (-160 < kalAngleY && kalAngleY < 160){
  for (int i = 0; i< N; i++)
  {
     Herkulex.moveOneAngle(motor_id[i],kalAngleY,50, LED_BLUE); //Send angle obtained from IMU to all motors connected
     delay(50); 
  };
  };

 Serial.print("\t");
 Serial.print("\r\n");
 delay(2);
}
