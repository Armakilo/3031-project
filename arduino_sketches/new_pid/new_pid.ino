// Limit PID output if necessary for speed regulation?

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

/* Change these pins to drive in the correct direction */

/* Setup motor driver pins for motor 1 */
#define PWM_IN1_1 7
#define PWM_IN2_1 6

/* Setup motor driver pins for motor 2 */
#define PWM_IN1_2 44
#define PWM_IN2_2 8

/* Setup motor driver pins for motor 3 */
#define PWM_IN1_3 46
#define PWM_IN2_3 45

/* Motor limit switches */
#define LIMIT_SWITCH_1 19
#define LIMIT_SWITCH_2 20
#define LIMIT_SWITCH_3 21

#define CPR_1 4156
#define CPR_2 2554
#define CPR_3 2554

#define PID_SAMPLING_TIME 100

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
int32_t previous_encoder_count_1 = 0;
int32_t previous_encoder_count_2 = 0;
int32_t previous_encoder_count_3 = 0;

int32_t encoder_count_1 = 0;
int32_t encoder_count_2 = 0;
int32_t encoder_count_3 = 0;

float angle_difference_1 = 0;
float angle_difference_2 = 0;
float angle_difference_3 = 0;

float initial_angle_1 = 0;
float initial_angle_2 = 130;
float initial_angle_3 = -34;

float angle_1 = 0;
float angle_2 = 0;
float angle_3 = 0;

float previous_angle_1 = initial_angle_1;
float previous_angle_2 = initial_angle_2;
float previous_angle_3 = initial_angle_3;

// Velocity variables for each motor
float velocity_1 = 0;
float velocity_2 = 0;
float velocity_3 = 0;

// Setpoint variables for each motor
float angle_setpoint_1 = initial_angle_1;
float angle_setpoint_2 = initial_angle_2;
float angle_setpoint_3 = initial_angle_3;

float velocity_setpoint_1 = 0;
float velocity_setpoint_2 = 0;
float velocity_setpoint_3 = 0;

/* Motor 1 PID declaration */
PIDController pid_1 = { 20, 0.5, 0,
                        0,
                        -255, 255,
                        -127, 127,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

/* Motor 2 PID declaration */
PIDController pid_2 = { 20, 0.5, 0,
                        0,
                        -255, 255,
                        -127, 127,
                        PID_SAMPLING_TIME/1000.0,
                        0, 0, 0, 0,
                        0};

/* Motor 3 PID declaration */
PIDController pid_3 = { 20, 0.5, 0,
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

/* PID functions */

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

/* Functions */

// Initial value and final value zero
float motionProfile(float position, float final_position, float velocity, float max_velocity, float acceleration, float update_time, float max_error) {
  float error = final_position - position;
  // Serial.println(error);
  if (fabs(error) <= max_error) {
    return 0;
  }

  float dir = 0;
  if (error > 0) {
    dir = 1.0f;
  } else {
    dir = -1.0f;
  }

  float v = velocity;
  float v_along = v*dir;

  float stopping_distance = v_along*v_along/(2.0f*acceleration);

  if (stopping_distance >= fabs(error)) {
    // no accelerate
    v_along -= acceleration*update_time;
  } else if (v_along < max_velocity) {
    // Accelerate only if below max velocidwty
    v_along += acceleration*update_time;
  } else {
    v_along = max_velocity;
  }

  if (v_along > max_velocity) {
    v_along = max_velocity;
  }
  if (v_along < 0) {
    v_along = 0;
  }
  
  return v_along * dir;
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

/* Zeros motors to microswitch to develop reference */
void home(void) {
  // leaves interrupts disabled until home function finishes executing
  // once home function finishes executing, reenable interrupts and make sure to set all angles to initial angle
  // use setmotor to set pwm manually
  setMotor(75, PWM_IN1_1, PWM_IN2_1);
  setMotor(75, PWM_IN1_2, PWM_IN2_2);
  setMotor(75, PWM_IN1_3, PWM_IN2_3);

  while(!digitalRead(LIMIT_SWITCH_1) || !digitalRead(LIMIT_SWITCH_2) || !digitalRead(LIMIT_SWITCH_3)) {
    if (digitalRead(LIMIT_SWITCH_1)) {
      setMotor(0, PWM_IN1_1, PWM_IN2_1);
    }

    if (digitalRead(LIMIT_SWITCH_2)) {
      setMotor(0, PWM_IN1_2, PWM_IN2_2);
    }

    if (digitalRead(LIMIT_SWITCH_3)) {
      setMotor(0, PWM_IN1_3, PWM_IN2_3);
    }
  }
}

/* Interrupt functions */
void readEncoder_1() {
  // sei();
  uint8_t valueRead = digitalRead(ENCODER_B_1);
  if (valueRead) {
    encoder_count_1++;
  } else {
    encoder_count_1--;
  }
}

void readEncoder_2() {
  // sei();
  uint8_t valueRead = digitalRead(ENCODER_B_2);
  if (valueRead) {
    encoder_count_2++;
  } else {
    encoder_count_2--;
  }
}

void readEncoder_3() {
  // sei();
  uint8_t valueRead = digitalRead(ENCODER_B_3);
  if (valueRead) {
    encoder_count_3++;
  } else {
    encoder_count_3--;
  }
}

uint8_t toggle = 0;
uint8_t pid_enable = 0;

ISR(TIMER1_COMPA_vect) {
  sei();
  // if (pid_enable) {
  digitalWrite(13, toggle);
  toggle = ~toggle;

  /* Hold all angular positions for calculations */
  angle_difference_1 = encoder_count_1*360.0f/CPR_1;
  angle_difference_2 = encoder_count_2*360.0f/CPR_2;
  angle_difference_3 = encoder_count_3*360.0f/CPR_3;

  /* Compute position as an angle */
  angle_1 = initial_angle_1 + angle_difference_1;
  angle_2 = initial_angle_2 + angle_difference_2;
  angle_3 = initial_angle_3 + angle_difference_3;

  /* Calculate angular velocities of all motors */
  velocity_1 = (angle_1 - previous_angle_1)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;
  velocity_2 = (angle_2 - previous_angle_2)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;
  velocity_3 = (angle_3 - previous_angle_3)/(PID_SAMPLING_TIME/1000.0f)*60.0f/360.0f;

  /* Hold the previous angular positions of all motors */
  previous_angle_1 = angle_1;
  previous_angle_2 = angle_2;
  previous_angle_3 = angle_3;

  velocity_setpoint_1 = motionProfile(angle_1, 
                                      angle_setpoint_1, 
                                      velocity_setpoint_1, 
                                      100, 
                                      3, 
                                      PID_SAMPLING_TIME/1000.0f, 
                                      1);

  velocity_setpoint_2 = motionProfile(angle_2, 
                                      angle_setpoint_2, 
                                      velocity_setpoint_2, 
                                      100, 
                                      3, 
                                      PID_SAMPLING_TIME/1000.0f, 
                                      1);

  velocity_setpoint_3 = motionProfile(angle_3, 
                                      angle_setpoint_3, 
                                      velocity_setpoint_3, 
                                      100, 
                                      3, 
                                      PID_SAMPLING_TIME/1000.0f, 
                                      1);

  /* Update all PIDs */
  PIDController_Update(&pid_1, velocity_setpoint_1, velocity_1);
  PIDController_Update(&pid_2, velocity_setpoint_2, velocity_2);
  PIDController_Update(&pid_3, velocity_setpoint_3, velocity_3);

  setMotor(pid_1.out, PWM_IN1_1, PWM_IN2_1);
  setMotor(pid_2.out, PWM_IN1_2, PWM_IN2_2);
  setMotor(pid_3.out, PWM_IN1_3, PWM_IN2_3);
  // }
}

void setup() {
  Serial.begin(115200);

  /* Disables interrupts */
  SREG = 0x7F;

  /* Configure timer 1 for 50 ms interrupts */
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Set compare match register for 50 Hz increments.
  OCR1A = 12499; 
  // Turn on CTC mode.
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 1024 prescaler.
  TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
  // Disable timer compare interrupt.
  TIMSK1 |= (1 << OCIE1A);

  /* Configure PWM pins to be ~3.9 kHz */
  TCCR4B = TCCR4B & 0b11111010;
  TCCR5B = TCCR5B & 0b11111010;

  PIDController_Init(&pid_1);
  PIDController_Init(&pid_2);
  PIDController_Init(&pid_3);

  /* Setup pins for motor 1 */
  pinMode(ENCODER_A_1, INPUT);
  pinMode(ENCODER_B_1, INPUT);
  pinMode(LIMIT_SWITCH_1, INPUT);

  /* Setup pins for motor 2 */
  pinMode(ENCODER_A_2, INPUT);
  pinMode(ENCODER_B_2, INPUT);
  pinMode(LIMIT_SWITCH_2, INPUT);

  /* Setup pins for motor 3 */ 
  pinMode(ENCODER_A_3, INPUT);
  pinMode(ENCODER_B_3, INPUT); 
  pinMode(LIMIT_SWITCH_3, INPUT);

  /* Setup ISR for motor 1 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_1), readEncoder_1, RISING);

  /* Setup ISR for motor 2 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_2), readEncoder_2, RISING);

  /* Setup ISR for motor 3 */
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_3), readEncoder_3, RISING);

  encoder_count_1 = 0;
  encoder_count_2 = 0;
  encoder_count_3 = 0;

  // setMotor(255, PWM_IN1_3, PWM_IN2_3);

  /* Homing function (leave before interrupt enable)*/
  // home();

  /* Enables global interrupts */
  SREG = 0xFF;

  delay(5000);

  // pid_enable = 1;
}

char bytes[50];

float angle_setpoint_1_path[7] = {initial_angle_1, 90, 180, 270, 180, 90, 0};
float angle_setpoint_2_path[7] = {initial_angle_2, 45, 130, 45, 130, 45, 130};
float angle_setpoint_3_path[7] = {initial_angle_3, 45, -30, 45, -30, 45, -30};

uint32_t execute_time = 0;
uint32_t i = 0;

void loop() {
  // angle_setpoint_3 = 180;
  if (millis() >= execute_time) {
    execute_time += 10000;
    Serial.print("Execute time: ");
    Serial.println(execute_time);
    angle_setpoint_1 = angle_setpoint_1_path[i]; 
    angle_setpoint_2 = angle_setpoint_2_path[i]; 
    angle_setpoint_3 = angle_setpoint_3_path[i]; 
    if (i < 6) {
      i++;
    }
  }

  // if (millis() > 40000) {
  //   angle_setpoint_1 = 360;
  //   angle_setpoint_2 = 360;
  //   angle_setpoint_3 = 360;
  // }

  // if (millis() > 50000) {
  //   angle_setpoint_1 = 720;
  //   angle_setpoint_2 = 720;
  //   angle_setpoint_3 = 720;
  // }

  // if (millis() > 60000) {
  //   angle_setpoint_1 = 540;
  //   angle_setpoint_2 = 540;
  //   angle_setpoint_3 = 540;
  // }

  // if (millis() > 70000) {
  //   angle_setpoint_1 = 0;
  //   angle_setpoint_2 = 0;
  //   angle_setpoint_3 = 0;
  // }

  delay(500);
  Serial.print("Time = ");
  Serial.print(millis());
  
  Serial.print(", Setpoint 1 = ");
  Serial.print(angle_setpoint_1);
  Serial.print(", Setpoint 2 = ");
  Serial.print(angle_setpoint_2);
  Serial.print(", Setpoint 3 = ");
  Serial.print(angle_setpoint_3);
  Serial.println();

  Serial.print("Readings: Velocity 1 = ");
  Serial.print(velocity_1);
  Serial.print(" RPM, ");
  Serial.print(angle_1);
  Serial.print(" degrees");
  Serial.println();


  Serial.print("Readings: Velocity 2 = ");
  Serial.print(velocity_2);
  Serial.print(" RPM, ");
  Serial.print("Velocity 2 Setpoint = ");
  Serial.print(velocity_setpoint_2);
  Serial.print(" RPM, ");
  Serial.print(angle_2);
  Serial.print(" degrees, ");
  Serial.print(pid_2.out);
  Serial.print(" PID 2 output");
  Serial.println();

  Serial.print("Readings: Velocity 3 = ");
  Serial.print(velocity_3);
  Serial.print(" RPM, ");
  Serial.print("Velocity 3 Setpoint = ");
  Serial.print(velocity_setpoint_3);
  Serial.print(" RPM, ");
  Serial.print(angle_3);
  Serial.print(" degrees, ");
  Serial.print(pid_3.out);
  Serial.print(" PID 3 output");
  Serial.println("\n");





  // String command = Serial.readString();

  // char awd[5] = "12345";

  // char ccmd[10];
  // command.toCharArray(ccmd, 10);



  // char* pref = strtok(ccmd,":");
  // char* suff = strtok(NULL,":");

  // // Serial.println(awd);
  // // Serial.println();
  
  //   if(pref == "m1s"){
  //       if (suff == "fwd"){
  //       angle_setpoint_1 += 10; 
  //     }
  //     else if (suff == "rev") {
  //       angle_setpoint_1 -= 10;
  //     }
  //   }
    
  //   if(pref == "m2s"){
  //       if (suff == "fwd"){
  //       angle_setpoint_1 += 10; 
  //     }
  //     else if (suff == "rev") {
  //       angle_setpoint_1 -= 10;
  //     }
  //   }

  //   if(pref == "m3s"){
  //       if (suff == "fwd"){
  //       angle_setpoint_1 += 10; 
  //     }
  //     else if (suff == "rev") {
  //       angle_setpoint_1 -= 10;
  //     }
  //   }

  

  
}

