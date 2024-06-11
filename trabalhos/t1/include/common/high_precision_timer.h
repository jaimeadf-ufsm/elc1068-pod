#ifndef HIGH_PRECISION_TIMER_H
#define HIGH_PRECISION_TIMER_H

typedef struct timer Timer;

#ifdef _WIN32
    #include <windows.h>

    struct timer {
        LARGE_INTEGER frequency;
        LARGE_INTEGER start;
        LARGE_INTEGER end;
    };
#else
    #include <sys/time.h>

    struct timer {
        struct timeval start;
        struct timeval end;
    };
#endif

Timer start_timer();
void stop_timer(Timer *timer);

double get_timer_units(Timer *timer);
double get_timer_seconds(Timer *timer);

#endif