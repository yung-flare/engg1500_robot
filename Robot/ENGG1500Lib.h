#ifndef ENGG1500LIB_H
#define ENGG1500LIB_H

#include<stdint.h>

void enc_init(void);

uint32_t enc_getLeft(void);
uint32_t enc_getRight(void);
void enc_clear(void);

int getVelEstimate(double &leftVel,double &rightVel);

#endif

