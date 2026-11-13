#define _POSIX_C_SOURCE 200809L // nanosleep

#include "zpiano.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

static const int SAMPLE_RATE = 8000;

int zpiano_init(void) {
    return 0;
}

void zpiano_close(void) {
}

void zpiano_clear(void) {
}

void zpiano_play(int pitch, int volume, int delay_ms) {
    if (pitch <= 0) return;

    if (volume < 0) volume = 0;
    if (volume > 90) volume = 90;

    FILE *aplay = popen("aplay -q -r 8000 -f U8 -c 1", "w"); // quite, hz, -f U8 8 bit format (0-255, 128 is quite), -c 1 mono sound -с 2 = stereo, "w" means ironically "write" no way
    if (!aplay) {
        perror("popen aplay");
        return;
    }

    int duration_ms = 300;
    int total_samples = SAMPLE_RATE * duration_ms / 1000;
    int period = SAMPLE_RATE / pitch;
    if (period < 2) period = 2;

    unsigned char *buf = malloc(total_samples);
    if (!buf) {
        pclose(aplay);
        return;
    }

    for (int i = 0; i < total_samples; i++) {
        // triangle
        int pos = i % period;
        int half = period / 2;
        int value;

        if (pos < half) {
            value = (pos * 2 * volume) / half;
        } else {
            value = (2 * volume) - ((pos - half) * 2 * volume) / half;
        }

        // curve
        int env = volume;
        int attack = 60;
        int release = 120;

        if (i < attack) {
            env = (volume * i) / attack;
        } else if (i > total_samples - release) {
            env = (volume * (total_samples - i)) / release;
        }

        int sample = 128 + ((value - volume) * env) / (volume ? volume : 1);
        if (sample < 0) sample = 0;
        if (sample > 255) sample = 255;

        buf[i] = (unsigned char)sample;
    }

    fwrite(buf, 1, total_samples, aplay);
    fflush(aplay);
    free(buf);
    pclose(aplay);

    // delay after note
    if (delay_ms > 0) {
        struct timespec ts = {
            .tv_sec = delay_ms / 1000,
            .tv_nsec = (delay_ms % 1000) * 1000000L
        };
        nanosleep(&ts, NULL);
    }
}
