//MOTOR DE PASSO
#define IN1     22
#define IN2     24
#define IN3     26
#define IN4     28

//SENSOR ULTRASSÔNICO
#define ECHO    12  //FIO VERDE ECHO
#define TRIGGER 13  //FIO AMARELO TRIGGER

#define TIMEOUT_SENSOR 29000

//TIMER DEFINES
#define TIMER_CONTROL   5  //ms
#define TIMER_CHECK_ANGLE TIMER_CONTROL * 3  //ms

#define HORARIO               1
#define ANTI_HORARIO          0

#define FULL_STEP_SIMPLE      0
#define FULL_STEP_DOUBLE      1


#define STRIDE_ANGLE  5.625 //each 64 steps
#define STEP_STRIDE   69
#define ANGLE_PER_STEP STRIDE_ANGLE / STEP_STRIDE
#define TOTAL_ANGLE   180 //DEGREES
#define TOTAL_STEPS (TOTAL_ANGLE * STEP_STRIDE) / (STRIDE_ANGLE * 2)

unsigned int flagCheckAngle;
unsigned char StateMachine;
unsigned char checkDistanceState;

unsigned long int actualTime;
unsigned int sentido_motor;
unsigned int tipo;
unsigned int stepsToReverseDirection;

unsigned int echoTime;

unsigned long previousMicros;

float actualAngle;
unsigned char plotAngle;

float media_echo;
float totalPoints;
float dist_eq1;
float dist_eq2;

enum steps{
  ROTATE_MOTOR,
  CHECK_DISTANCE,
  SEND_DATA
};

enum distanceSteps{
  SEND_TRIGGER,
  WAIT_ECHO,
  CALCULATE_DISTANCE
};

/*Tabela de distância por tempo 
 * uS   -- cm
 * 415  -- 7
 * 802  -- 13 
 * 974  -- 16
 * 1140 -- 20
 * 1736 -- 30
 * 
 * Equação linear
 * 0.0177 * x - 0.6945
 * Equação polinomial
 * 7e-07 * x^2 + 0.0161 + 0.0089
 * 
 * dist_eq1 = 0.0177 * media_echo - 0.6945;
 * dist_eq2 = 7e-07 * media_echo * media_echo + 0.0161 * media_echo + 0.0089;
*/
