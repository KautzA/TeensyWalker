// provide control for PWM servos via ADAFRUIT pwm servo driver https://www.adafruit.com/product/815
//Code is based off the library provided and directly uses code

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

void PWMServoMap(){
  for(int i = 0; i < NUM_PWM_SERVOS; i++){
    PWMI2C_setPWM(PWMServoLimits[i][3],0,constrain(map(constrain(PWMServoPos[i],PWMServoLimits[i][0],PWMServoLimits[i][1]),0,180,PWM_SERVOS_MIN,PWM_SERVOS_MAX),PWM_SERVOS_MIN,PWM_SERVOS_MAX));
  } 
}

uint8_t _i2caddr = 0x40;

void PWMI2C_Begin(){
  ServoI2C.begin(I2C_MASTER, 0x00, I2C_PINS_37_38, I2C_PULLUP_EXT, 100000);
  PWMI2C_Reset();
  
}

void PWMI2C_Reset(){
  PWMI2C_write8(PCA9685_MODE1,0x0);
}

void PWMI2C_setPWMFreq(float freq){
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  uint8_t prescale = floor(prescaleval + 0.5);
  uint8_t oldmode = PWMI2C_read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  PWMI2C_write8(PCA9685_MODE1, newmode); // go to sleep
  PWMI2C_write8(PCA9685_PRESCALE, prescale); // set the prescaler
  PWMI2C_write8(PCA9685_MODE1, oldmode);
  delay(5);
  PWMI2C_write8(PCA9685_MODE1, oldmode | 0xa1);
}

void PWMI2C_setPWM(uint8_t num, uint16_t on, uint16_t off) {
  //Serial.print("Setting PWM "); Serial.print(num); Serial.print(": "); Serial.print(on); Serial.print("->"); Serial.println(off);

  ServoI2C.beginTransmission(_i2caddr);
  ServoI2C.write(LED0_ON_L+4*num);
  ServoI2C.write(on);
  ServoI2C.write(on>>8);
  ServoI2C.write(off);
  ServoI2C.write(off>>8);
  ServoI2C.endTransmission();
}

void PWMI2C_setPin(uint8_t num, uint16_t val, bool invert)
{
  // Clamp value between 0 and 4095 inclusive.
  val = min(val, 4095);
  if (invert) {
    if (val == 0) {
      // Special value for signal fully on.
      PWMI2C_setPWM(num, 4096, 0);
    }
    else if (val == 4095) {
      // Special value for signal fully off.
      PWMI2C_setPWM(num, 0, 4096);
    }
    else {
      PWMI2C_setPWM(num, 0, 4095-val);
    }
  }
  else {
    if (val == 4095) {
      // Special value for signal fully on.
      PWMI2C_setPWM(num, 4096, 0);
    }
    else if (val == 0) {
      // Special value for signal fully off.
      PWMI2C_setPWM(num, 0, 4096);
    }
    else {
      PWMI2C_setPWM(num, 0, val);
    }
  }
}

uint8_t PWMI2C_read8(uint8_t addr) {
  ServoI2C.beginTransmission(_i2caddr);
  ServoI2C.write(addr);
  ServoI2C.endTransmission();

  ServoI2C.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return ServoI2C.read();
}

void PWMI2C_write8(uint8_t addr, uint8_t d) {
  ServoI2C.beginTransmission(_i2caddr);
  ServoI2C.write(addr);
  ServoI2C.write(d);
  ServoI2C.endTransmission();
}



