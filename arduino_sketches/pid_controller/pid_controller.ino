#define ENCODER_A 2
#define ENCODER_B 3
#define PWM_IN1 12
#define PWM_IN2 13
#define NO_SMOOTHING_POINTS 1000

// #define temp 10000

int32_t position = 0;
int32_t previous_time = 0;
float previous_error = 0;
float integral = 0;
float error = 0;

uint16_t index = 0;

int32_t temp = 20000;

int32_t previous_smooth_step = 0;
int32_t smoothStepArray[NO_SMOOTHING_POINTS] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);

  // Calculates smoothed version of setpoint
  smoothStep(temp);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoder, RISING);
}

void loop() {
  // int32_t setpoint = 1000*sin(previous_time/1e6);
  int32_t setpoint = smoothStepArray[index];
  // int32_t setpoint = temp;
  // int32_t setpoint = 1000;

  float kp = 2;
  float ki = 0.1;
  float kd = 0;

  int32_t current_time = micros();

  float delta_time = (float)((current_time - previous_time)/1.0e6);
  previous_time = current_time;

  int32_t error = setpoint - position;

  if (error <= 3*temp/NO_SMOOTHING_POINTS) {
    if (index < NO_SMOOTHING_POINTS - 1) {
      index++;
    }
  }

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
  if (pwm_dutycycle > 50) {
    pwm_dutycycle = 50;
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
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, LOW);
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

void smoothStep(int32_t max_value) {
  for (uint16_t i = 1; i <= NO_SMOOTHING_POINTS; i++) {
    smoothStepArray[i - 1] = (float) (max_value*(3*(1.0/NO_SMOOTHING_POINTS*i)*(1.0/NO_SMOOTHING_POINTS*i) - 2*(1.0/NO_SMOOTHING_POINTS*i)*(1.0/NO_SMOOTHING_POINTS*i)*(1.0/NO_SMOOTHING_POINTS*i)));
    Serial.println(smoothStepArray[i - 1]);
  }
}
