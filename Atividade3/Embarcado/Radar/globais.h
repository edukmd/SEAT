//MOTOR DE PASSO
#define IN1     8
#define IN2     9
#define IN3     10
#define IN4     11

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

unsigned int flagCheckAngle;
unsigned char StateMachine;
unsigned char checkDistanceState;

unsigned long int actualTime;
unsigned int sentido_motor;
unsigned int tipo;

enum steps{
  ROTATE_MOTOR,
  CHECK_DISTANCE
};

enum distanceSteps{
  SEND_TRIGGER,
  WAIT_ECHO,
  CALCULATE_DISTANCE
};
