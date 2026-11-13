#ifndef ZPIANO_H
#define ZPIANO_H

#include <stdint.h>

int zpiano_init(void);

void zpiano_close(void);

// pitch     —  (for example 440)
// volume    — 0–127
// delay_ms  — in miliseconds
void zpiano_play(int pitch, int volume, int delay_ms);

// buffer clearing
void zpiano_clear(void);

#endif
