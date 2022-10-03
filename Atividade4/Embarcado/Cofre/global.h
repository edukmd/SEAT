#ifndef __GLOBAL_H__

#define __GLOBAL_H__

#define CHANNEL_A       19
#define CHANNEL_B       18
#define CHANNEL_C       22
#define GREEN_LED       24
#define MOTOR_PWM       5
#define melodyPin       3


#define DOOR_CHECK      A0

#define OPEN            0
#define CLOSE          90


#define DEBOUNCE_TICKS        6000
#define CHECK_ROTATION_TICKS  1500
#define DEBOUNCE_TO_CLOSE     5000
#define DEBOUNCE_TO_OPEN      DEBOUNCE_TO_CLOSE

typedef enum _direction{
  CW,
  CCW
}direction_t;

int flagIRQ_A = 0;
int flagIRQ_B = 0;

int actualCHANNEL_A = 0;
int actualCHANNEL_B = 0;

int lastCHANNEL_A = 0;
int lastCHANNEL_B = 0;

direction_t actualDirection = CW;

int checkDirection = 0;
int lastRotation = 0;

int totalRotation = 0;

int debounceButton = DEBOUNCE_TICKS;
int pressedFlag = 0;
int flagToCheck = 0;

int timeToCheck = CHECK_ROTATION_TICKS;
int doorValue = 0;
int checkDoorOpen = 0;
int timeToOpen = DEBOUNCE_TO_OPEN;
int timeToClose = DEBOUNCE_TO_CLOSE;




//Password variables
int password[4] = {0,0,0,0};
int passwordDefault[4] = {2,-5,3,8};
unsigned char passwordPos = 0;
unsigned char passwordPassed = 0;


//Functions
unsigned char checkChannelC(void);
void checkRotation(void);
unsigned char checkPassword(void);

#endif
