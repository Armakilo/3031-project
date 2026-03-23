#define ENCODER_A 2
#define ENCODER_B 3
#define PWM_IN1 12
#define PWM_IN2 13
#define CPR 2100

// Position tracking variables
int32_t previous_position = 0;
int32_t position = 0;

// Velocity tracking variables
float velocity_previous = 0;
float velocity = 0;

// PID time tracking variables
uint32_t previous_time = 0;
uint32_t time = 0;
float dt = 0;

// PID summation variables
float setpoint = 60;

float previous_error = 0;
float error = 0;

// PID gain variables

// Proportional variables
float kp = 5;

// Integral variables
float ki = 5;
float integral = 0;

// Derivative variables
float kd = 0;

// PID control signal variables
float u = 0;

// Motor control variables
float pwm_dutycycle = 0;
int8_t direction = 0;

// Velocity measurement function variables
uint32_t previous_time_func = 0;
uint32_t time_func = 0;
float dt_func = 0;

struct PID {
  float kp;
  float ki;
  float kp;

  uint32_t dt;

  float previous_error;
  float error;
};

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT); 
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoder, RISING);
}

void loop() {
  // Determine time between motor updates
  time = micros();
  dt = ((float) (time - previous_time))/1.0e6;
  previous_time = time;

  // Compute error
  error = setpoint - velocity;

  // Compute integral
  integral += error*dt;
  // if (((previous_error < 0) && (error > 0)) || ((previous_error > 0) && (error < 0)) || (position == setpoint)) {
  //   integral = 0;
  // }

  // Compute control variable
  u = kp*error + ki*integral;

  // Store previous error
  previous_error = error;

  // Set PWM
  pwm_dutycycle = fabs(u);

  if (pwm_dutycycle > 100) {
    pwm_dutycycle = 100;
  }

  direction = 1;
  if (u < 0) {
    direction = -1; 
  }

  // direction = 1;
  // pwm_dutycycle = 50;

  // Set the motor
  setMotor(direction, pwm_dutycycle, PWM_IN1, PWM_IN2);

  // Serial.print("Velocity: ");
  Serial.println(velocity);
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

float readEncoder() {
  uint8_t valueRead = digitalRead(ENCODER_B);

  if (valueRead) {
    position++;
  } else {
    position--;
  }

  time_func = micros();
  dt_func = ((float) (time_func - previous_time_func))/1.0e6;
  previous_time_func = time_func;

  velocity_previous = velocity;
  velocity = valueRead/(dt_func*CPR)*60;
}
