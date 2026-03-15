#define ENCODER_A 2
#define ENCODER_B 3
#define PWM_IN1 12
#define PWM_IN2 13

int32_t position = 0;
int32_t previous_time = 0;
float previous_error = 0;
float integral = 0;
float error = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT); 
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoder, RISING);
}

void loop() {
  int32_t setpoint = 1000*sin(previous_time/1e6);

  float kp = 1;
  float ki = 1;
  float kd = 0;

  int32_t current_time = micros();

  float delta_time = (float)((current_time - previous_time)/1.0e6);
  previous_time = current_time;

  int error = setpoint - position;

  // Derivative
  float derivative = (error - previous_error)/delta_time;

  // Integral
  integral += error*delta_time;
  // This would probably be useful for preventing integrator windup
  // Noticed that motor is 'choppy' at the extremes with input sinewave
  if (((previous_error < 0) && (error > 0)) || ((previous_error > 0) && (error < 0)) || (position == setpoint)) {
    integral = 0;
  }

  // Control signal
  float u = kp*error + ki*integral + kd*derivative;

  // Add max PID value and normalize it to duty cycle?

  float pwm_dutycycle = fabs(u);
  if (pwm_dutycycle > 100) {
    pwm_dutycycle = 100;
  }

  int8_t direction = 1;
  if (u < 0) {
    direction = -1; 
  }

  // Makes motor choppy at extremes of input sine
  // if (position == setpoint) {
  //   direction = 0;
  // }

  setMotor(direction, pwm_dutycycle, PWM_IN1, PWM_IN2);

  previous_error = error;

  // Serial.print("Setpoint = ");
  // Serial.println(setpoint);
  // Serial.print("Position = ");
  Serial.println(position);

  // Serial.println(u);
}

void setMotor(int8_t direction, uint8_t pwm_dutycycle, uint8_t pin_in1, uint8_t pin_in2) {
  if (direction == 1) {
    digitalWrite(pin_in1, HIGH);
    analogWrite(pin_in2, 255 - pwm_dutycycle*255/100);
  } else if (direction == -1) {
    digitalWrite(pin_in2, HIGH);
    analogWrite(pin_in1, 255 - pwm_dutycycle*255/100);  
  } else {
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
  }

}

void readEncoder() {
  uint8_t valueRead = digitalRead(ENCODER_B);

  if (valueRead) {
    position++;
  } else {
    position--;
  }
}
