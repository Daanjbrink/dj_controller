#ifndef EXP_H_
#define EXP_H_

uint8_t encoderState[3];

void expInit();
uint8_t expReadPin(uint8_t exp, uint8_t pin);

#endif