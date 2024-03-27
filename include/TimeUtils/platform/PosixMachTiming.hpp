/*
  https://github.com/ChisholmKyle/PosixMachTiming

  Copyright (c) 2015, Kyle Chisholm <>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef TIMING_MACH_H
#define TIMING_MACH_H
/* ************* */
/* TIMING_MACH_H */

#include <time.h>

/* scale factors */
#define TIMING_GIGA (1000000000)


/* timespec difference (monotonic) right - left */
inline void timespec_monodiff_rml(struct timespec *ts_out,
                                  const struct timespec *ts_in) {
    /* out = in - out,
       where in > out
     */
    ts_out->tv_sec = ts_in->tv_sec - ts_out->tv_sec;
    ts_out->tv_nsec = ts_in->tv_nsec - ts_out->tv_nsec;
    if (ts_out->tv_sec < 0) {
        ts_out->tv_sec = 0;
        ts_out->tv_nsec = 0;
    } else if (ts_out->tv_nsec < 0) {
        if (ts_out->tv_sec == 0) {
            ts_out->tv_sec = 0;
            ts_out->tv_nsec = 0;
        } else {
            ts_out->tv_sec = ts_out->tv_sec - 1;
            ts_out->tv_nsec = ts_out->tv_nsec + TIMING_GIGA;
        }
    } else {}
}

#ifdef __MACH__
/* **** */
/* MACH */

    /* emulate clock_nanosleep for CLOCK_MONOTONIC and TIMER_ABSTIME */
    inline int clock_nanosleep_abstime ( const struct timespec *req )
    {
        struct timespec ts_delta;
        int retval = clock_gettime ( CLOCK_MONOTONIC, &ts_delta );
        if (retval == 0) {
            timespec_monodiff_rml ( &ts_delta, req );
            retval = nanosleep ( &ts_delta, NULL );
        }
        return retval;
    }

/* MACH */
/* **** */
#else
/* ***** */
/* POSIX */

    /* clock_nanosleep for CLOCK_MONOTONIC and TIMER_ABSTIME */
    #define clock_nanosleep_abstime( req ) \
        clock_nanosleep ( CLOCK_MONOTONIC, TIMER_ABSTIME, (req), NULL )

/* POSIX */
/* ***** */
#endif

/* TIMING_MACH_H */
/* ************* */
#endif
