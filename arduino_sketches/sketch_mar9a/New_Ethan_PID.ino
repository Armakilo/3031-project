/* Setup encoder pins for motor 1 */
#define ENCODER_A_1 18
#define ENCODER_B_1 23

/* Setup encoder pins for motor 2 */
#define ENCODER_A_2 19
#define ENCODER_B_2 25

/* Setup encoder pins for motor 3 */
// #define ENCODER_A_3 20
// #define ENCODER_B_3 27

/* Setup motor driver pins for motor 1 */
#define PWM_IN1_1 2
#define PWM_IN2_1 3

/* Setup motor driver pins for motor 1 */
#define PWM_IN1_2 5
#define PWM_IN2_2 6

/* Setup motor driver pins for motor 1 */
// #define PWM_IN1_3 7
// #define PWM_IN2_3 8

#define CPR 2950

#define PID_SAMPLING_TIME 50

#define Length 0.22

/* Struct definitions */
typedef struct {
  /* Controller gains */
  float Kp;
  float Ki;
  float Kd;

  /* Derivative  low-pass filter time constant */
  float tau;

  /* Output limits */
  float limMin;
  float limMax;

  /* Integrator limits */
  float limMinInt;
  float limMaxInt;

  /* Sample time (in seconds) */
  float T;

  /* Controller memory */
  float integrator;
  float prevError;
  float differentiator;
  float prevMeasurement;

  /* Controller output */
  float out;
} PIDController;

/* PID update time (change to dedicated timer later?) */
uint32_t time_to_execute = PID_SAMPLING_TIME;

// Hold these positions when time to update

/* Position hold variables */
int32_t position_1 = 0;
int32_t position_2 = 0;
int32_t position_3 = 0;

int32_t position_1_hold = 0;
int32_t position_2_hold = 0;
int32_t position_3_hold = 0;

int32_t previous_position_1 = 0;
int32_t previous_position_2 = 0;
int32_t previous_position_3 = 0;

// Velocity variables for each motor
float velocity_1 = 0;
float velocity_2 = 0;
float velocity_3 = 0;

// Setpoint variables for each motor
float setpoint_1 = 0;
float setpoint_2 = 0;
float setpoint_3 = 0;
float height = 0;
float radius = 0;

/* Motor 1 PID declaration */
// PIDController pid_1 = { 8, 4, 0.01,
//                         0,
//                         -255, 255,
//                         -255, 255,
//                         PID_SAMPLING_TIME/1000.0,
//                         0, 0, 0, 0,
//                         0};

PIDController pid_1 = { 0.12, 0.013, 0,
                        0,
                        -255, 255,
                        -10000, 10000,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

PIDController pid_2 = { 0.12, 0.013, 0,
                        0,
                        -255, 255,
                        -10000, 10000,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};
                        
/* Max velocity profile */
// float max_velocity_profile[100] = {0};

void PIDController_Init(PIDController *pid) {
	/* Clear controller variables */
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

}

float PIDController_Update(PIDController *pid, float setpoint, float measurement) {
	/*Error signal */
  float error = setpoint - measurement;

	/* Proportional */
  float proportional = pid->Kp * error;

	/* Integral */
  pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	/* Anti-wind-up via integrator clamping */
  if (pid->integrator > pid->limMaxInt) {
    pid->integrator = pid->limMaxInt;
  } else if (pid->integrator < pid->limMinInt) {
    pid->integrator = pid->limMinInt;
  }

	/* Derivative (band-limited differentiator) */
  pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);


	/* Compute output and apply limits */
  pid->out = proportional + pid->integrator + pid->differentiator;

  if (pid->out > pid->limMax) {
    pid->out = pid->limMax;
  } else if (pid->out < pid->limMin) {
    pid->out = pid->limMin;
  }

	/* Store error and measurement for later use */
  pid->prevError = error;
  pid->prevMeasurement = measurement;

	/* Return controller output */
  return pid->out;
}

// Initial value and final value zero
void trapezoid(float *array, float slope, float max_value, float sampling_time, float time) {
  uint16_t samples = time/sampling_time;
  uint16_t samples_slopes = max_value/(slope*sampling_time);
  for (uint16_t i = 0; i < samples_slopes; i++) {
    array[i] = slope*sampling_time*i;
  }

  for (uint16_t i = samples_slopes; i < samples + samples_slopes; i++) {
    array[i] = max_value;
  }

  for (uint16_t i = samples + samples_slopes; i < samples + samples_slopes*2; i++) {
    //array[i] = max_value + slope*sampling_time*(samples + samples_slopes - 1 - i);
    array[i] = array[i-1] - slope*sampling_time;
  }
  
  for (uint16_t i = 0; i < samples + samples_slopes*2; i++) {
      Serial.print("Index: ");
      Serial.print(i);
      Serial.print(" Value: ");
      Serial.println(array[i]);
  } 
}

// float path[1500] = {0};

void setup() {
  Serial.begin(115200);
  // trapezoid(path, 40, 80, 0.05, 10);
  delay(5000);

  TCCR3B = (TCCR3B & 0b11111010); // 976 Hz
  TCCR4B = (TCCR4B & 0b11111010); // 976 Hz

  PIDController_Init(&pid_1);
  PIDController_Init(&pid_2);

  /* Setup pins for motor 1 */
  pinMode(ENCODER_A_1, INPUT);
  pinMode(ENCODER_B_1, INPUT);

  /* Setup pins for motor 2 */
  pinMode(ENCODER_A_2, INPUT);
  pinMode(ENCODER_B_2, INPUT);

  /* Setup pins for motor 3 */ 
  // pinMode(ENCODER_A_3, INPUT);
  // pinMode(ENCODER_B_3, INPUT); 

  /* Setup ISR for motor 1 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_1), readEncoder_1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), readEncoder_2, RISING);

  /* Setup ISR for motor 2 */
  // attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), readEncoder_2, RISING);

  /* Setup ISR for motor 3 */
  // attachInterrupt(digitalPinToInterrupt(ENCODER_A_3), readEncoder_3, RISING);
  setpoint_1 = 10000;
  setpoint_2 = 10000;
  setpoint_3 = 0;

  height = 0.1;
  radius = 0.2;

  cartisian(height, radius, &setpoint_1, &setpoint_2);
  setpoint_1 = setpoint_1 * CPR / 360;

  time_to_execute = millis() + PID_SAMPLING_TIME;
}

int i = 0;

float int_position = 0;

void loop() {
  if (millis() > time_to_execute) {
    time_to_execute += PID_SAMPLING_TIME;



    // if (i < 1500) {
    //   setpoint_1 = path[i];
    //   i++;
    // }

    /* Hold all angular positions for calculations */
    position_1_hold = position_1;
    position_2_hold = position_2;
    position_3_hold = position_3;

    /* Calculate angular velocities of all motors */
    velocity_1 = (position_1_hold - previous_position_1)/((PID_SAMPLING_TIME/1000.0)*CPR)*60.0;
    velocity_2 = (position_2_hold - previous_position_2)/((PID_SAMPLING_TIME/1000.0)*CPR)*60.0;
    velocity_3 = (position_3_hold - previous_position_3)/((PID_SAMPLING_TIME/1000.0)*CPR)*60.0;

    /* Hold the previous angular positions of all motors */
    previous_position_1 = position_1_hold;
    previous_position_2 = position_2_hold;
    previous_position_3 = position_3_hold;

    int_position += velocity_1*((PID_SAMPLING_TIME/1000.0)/60.0);

    /* Update all PIDs */
    // PIDController_Update(&pid_1, setpoint_1, velocity_1);
    PIDController_Update(&pid_1, setpoint_1, position_1_hold);
    PIDController_Update(&pid_2, setpoint_2, position_2_hold);
    // add others later

    /* Update all motor PWM signals */
    // if (i == 440) {
    //   setMotor(0, PWM_IN1_1, PWM_IN2_1);
    // } else {
    setMotor(pid_1.out, PWM_IN1_1, PWM_IN2_1);
    setMotor(pid_2.out, PWM_IN1_2, PWM_IN2_2);
    // }
    // add others later

    Serial.print(setpoint_1);
    Serial.print("  ");
    Serial.print(position_1_hold);
    Serial.print("  ");
    Serial.print(position_2_hold);
    Serial.println();
    // Serial.print("Update: ");
    // Serial.print(millis());
    // Serial.print(", ");
    // Serial.print(setpoint_1);
    // Serial.print(" setpoint, ");
    // Serial.println(velocity_1);
    // Serial.print(" RPM, ");
    // Serial.print(position_1_hold);
    // Serial.print(" Encoder ticks, ");
    // Serial.print(int_position);
    // Serial.println(" Integrated encoder ticks");
    // Serial.println(pid_1.out);
  }
}

void cartisian(float H, float R, float *setpoint_1, float *setpoint_2)
{
  float A = sqrt(H^2 + R^2);
  float angleL = arctan(H/R);

  float angleA = arccos(((2*(L^2))-A^2)/(4*L));
  float angleC = arcsin((L*sin(a))/A);

  *setpoint_1 = angleC + angleL;
  *setpoint_2 = 180 - a - angleC - angleL;
}

void setMotor(int16_t pwm_dutycycle, uint8_t pin_in1, uint8_t pin_in2) {
  if (pwm_dutycycle >= 0) {
    digitalWrite(pin_in1, HIGH);
    analogWrite(pin_in2, 255 - pwm_dutycycle);
  } else if (pwm_dutycycle < 0) {
    digitalWrite(pin_in2, HIGH);
    analogWrite(pin_in1, 255 + pwm_dutycycle);  
  } else {
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
  }
}

void readEncoder_1() {
  uint8_t valueRead = digitalRead(ENCODER_B_1);
  if (valueRead) {
    position_1++;
  } else {
    position_1--;
  }
}

void readEncoder_2() {
  uint8_t valueRead = digitalRead(ENCODER_B_2);
  if (valueRead) {
    position_2++;
  } else {
    position_2--;
  }
}

// void readEncoder_3() {
//   uint8_t valueRead = digitalRead(ENCODER_B_3);
//   if (valueRead) {
//     position_3++;
//   } else {
//     position_3--;
//   }
// }