#pragma once


extern uint t;  // time

extern int8_t tadd[5];   // speed


extern void PlasmaInit(uint16_t* buf, int w, int h);

extern void Plasma(int mode);
extern void Plasma0(),Plasma1(),Plasma2(),Plasma3(),Plasma4(),PlasmaC();
