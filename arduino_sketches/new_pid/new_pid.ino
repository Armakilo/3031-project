#include <TimerOne.h>

/* Clock frequency */
#define CLK_FREQUENCY 16000000

/* Setup encoder pins for motor 1 */
#define ENCODER_A_1 2
#define ENCODER_B_1 22

/* Setup encoder pins for motor 2 */
#define ENCODER_A_2 3
#define ENCODER_B_2 24

/* Setup encoder pins for motor 3 */
#define ENCODER_A_3 18
#define ENCODER_B_3 26

/* Setup motor driver pins for motor 1 */
#define PWM_IN1_1 6
#define PWM_IN2_1 7

/* Setup motor driver pins for motor 2 */
#define PWM_IN1_2 8
#define PWM_IN2_2 44

/* Setup motor driver pins for motor 3 */
#define PWM_IN1_3 45
#define PWM_IN2_3 46


#define CPR_1 2100
#define CPR_2 2950
#define CPR_3 2950

#define PID_SAMPLING_TIME 50

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
int32_t encoder_count_1 = 0;
int32_t encoder_count_2 = 0;
int32_t encoder_count_3 = 0;

float angle_1 = 0;
float angle_2 = 0;
float angle_3 = 0;

float previous_angle_1 = 0;
float previous_angle_2 = 0;
float previous_angle_3 = 0;

// Velocity variables for each motor
float velocity_1 = 0;
float velocity_2 = 0;
float velocity_3 = 0;

// Setpoint variables for each motor
float setpoint_1 = 0;
float setpoint_2 = 0;
float setpoint_3 = 0;

/* Motor 1 PID declaration */
PIDController pid_1 = { 0.7, 0.05, 0,
                        0,
                        -255, 255,
                        -127, 127,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

/* Motor 2 PID declaration */
PIDController pid_2 = { 0.25, 0.1, 0,
                        0,
                        -255, 255,
                        -127, 127,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

/* Motor 3 PID declaration */
PIDController pid_3 = { 0.2, 0.1, 0,
                        0,
                        -255, 255,
                        -127, 127,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

  /* Motor 1 PID declaration */
// PIDController pid_1 = { 5, 1, 0,
//                         0,
//                         -255, 255,
//                         -127, 127,
//                         PID_SAMPLING_TIME/1000.0,
//                         0, 0, 0, 0,
//                         0};

// /* Motor 2 PID declaration */
// PIDController pid_2 = { 0.1, 0.01, 0,
//                         0,
//                         -255, 255,
//                         -127, 127,
//                         PID_SAMPLING_TIME/1000.0,
//                         0, 0, 0, 0,
//                         0};

// /* Motor 3 PID declaration */
// PIDController pid_3 = { 0.1, 0.01, 0,
//                         0,
//                         -255, 255,
//                         -127, 127,
//                         PID_SAMPLING_TIME/1000.0,
//                         0, 0, 0, 0,
//                         0};

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

/* Zeros motors to microswitch to develop reference */
void home(void) {

}

// float path[1500] = {0};

void setup() {
  Serial.begin(115200);

  /* Disables interrupts */
  noInterrupts();
  /* Configure PWM pins to be ~3.9 kHz */
  TCCR4B = TCCR4B & 0b11111010;
  TCCR5B = TCCR4B & 0b11111010;
  /* Enables interrupts */
  interrupts();

  // trapezoid(path, 40, 80, 0.05, 10);
  delay(5000);


  PIDController_Init(&pid_1);
  PIDController_Init(&pid_2);
  PIDController_Init(&pid_3);

  /* Setup pins for motor 1 */
  pinMode(ENCODER_A_1, INPUT);
  pinMode(ENCODER_B_1, INPUT);

  /* Setup pins for motor 2 */
  pinMode(ENCODER_A_2, INPUT);
  pinMode(ENCODER_B_2, INPUT);

  /* Setup pins for motor 3 */ 
  pinMode(ENCODER_A_3, INPUT);
  pinMode(ENCODER_B_3, INPUT); 

  /* Setup ISR for motor 1 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_1), readEncoder_1, RISING);

  /* Setup ISR for motor 2 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), readEncoder_2, RISING);

  /* Setup ISR for motor 3 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_3), readEncoder_3, RISING);

  setpoint_1 = 0;
  setpoint_2 = 0;
  setpoint_3 = 0;

  // home();

  time_to_execute = millis() + PID_SAMPLING_TIME;
}

int i = 0;

float int_position = 0;

void loop() {
  if (millis() > time_to_execute) {
    time_to_execute += PID_SAMPLING_TIME;

    if (millis() > 7000) {
      setpoint_1 = 360;
      setpoint_2 = 360;
      setpoint_3 = 360;
    }

    // if (millis() > 30000) {
    //   setpoint_1 = 0;
    //   setpoint_2 = 0;
    //   setpoint_3 = 0;
    // }


    // if (i < 1500) {
    //   setpoint_1 = path[i];
    //   i++;
    // }

    /* Hold all angular positions for calculations */
    angle_1 = encoder_count_1;
    angle_2 = encoder_count_2;
    angle_3 = encoder_count_3;

    /* Compute position as an angle */
    angle_1 = angle_1*360.0f/CPR_1;
    angle_2 = angle_2*360.0f/CPR_2;
    angle_3 = angle_3*360.0f/CPR_3;


    /* Calculate angular velocities of all motors */
    velocity_1 = (angle_1 - previous_angle_1)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;
    velocity_2 = (angle_2 - previous_angle_2)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;
    velocity_3 = (angle_3 - previous_angle_3)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;

    /* Hold the previous angular positions of all motors */
    previous_angle_1 = angle_1;
    previous_angle_2 = angle_2;
    previous_angle_3 = angle_3;

    // int_position += velocity_1*((PID_SAMPLING_TIME/1000.0)/60.0);

    /* Update all PIDs */
    PIDController_Update(&pid_1, setpoint_1, angle_1);
    PIDController_Update(&pid_2, setpoint_2, angle_2);
    PIDController_Update(&pid_3, setpoint_3, angle_3);

    // PIDController_Update(&pid_1, setpoint_1, velocity_1);
    // PIDController_Update(&pid_2, setpoint_2, velocity_2);
    // PIDController_Update(&pid_3, setpoint_3, velocity_3);

    /* Update all motor PWM signals */
    // if (i == 440) {
    //   setMotor(0, PWM_IN1_1, PWM_IN2_1);
    // } else {
    setMotor(pid_1.out, PWM_IN1_1, PWM_IN2_1);
    setMotor(pid_2.out, PWM_IN1_2, PWM_IN2_2);
    setMotor(pid_3.out, PWM_IN1_3, PWM_IN2_3);
    // }
    // add others later

    Serial.print("Time = ");
    Serial.print(millis());
    
    Serial.print(", Setpoint 1 = ");
    Serial.print(setpoint_1);
    Serial.print(", Setpoint 2 = ");
    Serial.print(setpoint_2);
    Serial.print(", Setpoint 3 = ");
    Serial.print(setpoint_3);
    Serial.println();

    Serial.print("Readings: Velocity 1 = ");
    Serial.print(velocity_1);
    Serial.print(" RPM, ");
    Serial.print(angle_1);
    Serial.print(" degrees, ");
    Serial.println();


    Serial.print("Readings: Velocity 2 = ");
    Serial.print(velocity_2);
    Serial.print(" RPM, ");
    Serial.print(angle_2);
    Serial.print(" degrees");
    Serial.println();

    Serial.print("Readings: Velocity 3 = ");
    Serial.print(velocity_3);
    Serial.print(" RPM, ");
    Serial.print(angle_3);
    Serial.print(" degrees");
    Serial.println("\n");

  }
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
    encoder_count_1++;
  } else {
    encoder_count_1--;
  }
}

void readEncoder_2() {
  uint8_t valueRead = digitalRead(ENCODER_B_2);
  if (valueRead) {
    encoder_count_2++;
  } else {
    encoder_count_2--;
  }
}

void readEncoder_3() {
  uint8_t valueRead = digitalRead(ENCODER_B_3);
  if (valueRead) {
    encoder_count_3++;
  } else {
    encoder_count_3--;
  }
}