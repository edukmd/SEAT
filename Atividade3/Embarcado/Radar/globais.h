//MOTOR DE PASSO
#define IN1     22
#define IN2     24
#define IN3     26
#define IN4     28

//SENSOR ULTRASSÔNICO
#define ECHO    13
#define TRIGGER 12

//TIMER DEFINES
#define TIMER_CONTROL   1  //ms
#define TIMER_CHECK_ANGLE TIMER_CONTROL * 5  //ms

#define HORARIO               1
#define ANTI_HORARIO          0

#define FULL_STEP_SIMPLE      0
#define FULL_STEP_DOUBLE      1


#define STRIDE_ANGLE  5.625 //each 64 steps
#define TOTAL_ANGLE   180 //DEGREES
#define TOTAL_STEPS (TOTAL_ANGLE * 64) / (STRIDE_ANGLE * 2)

unsigned int flagCheckAngle;
unsigned char StateMachine;
unsigned char checkDistanceState;

unsigned long int actualTime;
unsigned int sentido_motor;
unsigned int tipo;
unsigned int stepsToReverseDirection;

enum steps{
  ROTATE_MOTOR,
  CHECK_DISTANCE
};

enum distanceSteps{
  SEND_TRIGGER,
  WAIT_ECHO,
  CALCULATE_DISTANCE
};
