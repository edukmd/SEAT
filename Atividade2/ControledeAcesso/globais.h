#ifndef GLOBAIS_H
#define GLOBAIS_H

#include <Arduino.h>

#define __DEBUG__

typedef struct {
  unsigned char *name;
  unsigned char tagNUID[4];
  unsigned int usedTimes;
  
}user_t;

unsigned int debounceCheck;

unsigned char accessState;
unsigned char accessStateOld;

unsigned char userSelected;

#define BUFFER_SIZE 18
unsigned char buffer[BUFFER_SIZE];
unsigned accessOK;

#define BLOCK_ADDR_READTRIES    1
#define TRAILER_ADDR_READTRIES  2

enum {
  CHECK_USER,
  ACCESS_CONTROL_GRANTED,
  ACCESS_CONTROL_BLOCKED,
  REGISTER_USER,
  DELETE_USER
};

#endif
