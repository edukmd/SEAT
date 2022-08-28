#ifndef GLOBAIS_H
#define GLOBAIS_H

#include <Arduino.h>

typedef struct {
  unsigned char *name;
  unsigned char tagNUID[4];
  unsigned int usedTimes;
  
}user_t;

unsigned int debounceCheck;

unsigned char accessState;

enum {
  CHECK_USER,
  ACCESS_CONTROL_GRANTED,
  ACCESS_CONTROL_BLOCKED,
  REGISTER_USER,
};

#endif
