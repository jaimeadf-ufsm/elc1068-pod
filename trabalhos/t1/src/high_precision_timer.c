#include <stdlib.h>

#include "common/high_precision_timer.h"

#ifdef _WIN32
    Timer start_timer()
    {
        Timer timer;
        QueryPerformanceFrequency(&timer.frequency);
        QueryPerformanceCounter(&timer.start);

        return timer;
    }

    void stop_timer(Timer *timer)
    {
        QueryPerformanceCounter(&timer->end);
    }

    double get_timer_nanoseconds(Timer *timer)
    {
        return (timer->end.QuadPart - timer->start.QuadPart) * 1e9 / timer->frequency.QuadPart;
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

    double get_timer_nanoseconds(Timer *timer)
    {
        return (timer->end.tv_sec - timer->start.tv_sec) * 1e9 + (timer->end.tv_usec - timer->start.tv_usec) * 1e3;
    }
#endif