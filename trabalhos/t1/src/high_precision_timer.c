#include <stdlib.h>

#include "common/high_precision_timer.h"

#ifdef _WIN32
    Timer start_timer()
    {
        Timer timer;
        QueryPerformanceCounter(&timer.start);

        return timer;
    }

    void stop_timer(Timer *timer)
    {
        QueryPerformanceCounter(&timer->end);
        QueryPerformanceFrequency(&timer->frequency);
    }

    double get_timer_units(Timer *timer)
    {
        return timer->end.QuadPart - timer->start.QuadPart;
    }

    double get_timer_seconds(Timer *timer)
    {
        return (double)(timer->end.QuadPart - timer->start.QuadPart) / timer->frequency.QuadPart;
    }
#else
    Timer start_timer()
    {
        Timer timer;
        gettimeofday(&timer.start, NULL);

        return timer;
    }

    void stop_timer(Timer *timer)
    {
        gettimeofday(&timer->end, NULL);
    }

    double get_timer_units(Timer *timer)
    {
        return (timer->end.tv_sec - timer->start.tv_sec) * 1e6 + (timer->end.tv_usec - timer->start.tv_usec);
    }

    double get_timer_seconds(Timer *timer)
    {
        return (timer->end.tv_sec - timer->start.tv_sec) * 1e9 + (timer->end.tv_usec - timer->start.tv_usec) * 1e3;
    }
#endif