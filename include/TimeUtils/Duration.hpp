#pragma once

#include <TimeUtils/TimeUtils_export.hpp>

#include <cstdint>

#if defined(__APPLE__) || defined(__MACH__)
  static_assert ( true, "TimeUtils is not compatible with OS X" );

#elif defined(_WIN32)
  #include <windows.h>

  #ifndef TIME_UTILS_WIN
    #define TIME_UTILS_WIN
  #endif

#elif defined(__linux__)
  #include <time.h>

  #ifndef TIME_UTILS_LIN
    #define TIME_UTILS_LIN
  #endif

#else
  static_assert ( true, "TimeUtils is not compatible with your OS" );

#endif


namespace TimeUtils
{

class Duration;

TIMEUTILS_EXPORT Duration Now();
TIMEUTILS_EXPORT bool Sleep( const Duration& duration );
TIMEUTILS_EXPORT bool SleepUntil( const Duration& timeStamp );

TIMEUTILS_EXPORT void TimePeriodInit();
TIMEUTILS_EXPORT void TimePeriodDeinit();

class TIMEUTILS_EXPORT Duration
{
  int64_t mSec {};
  int64_t mNSec {};

public:
  Duration() = default;
  Duration( const double time );
  Duration( const int64_t sec,
            const int64_t nsec );

  bool sleep() const;
  Duration now();

#if defined (TIME_UTILS_WIN)
  Duration( const LARGE_INTEGER& time );
  operator LARGE_INTEGER () const;

#elif defined (TIME_UTILS_LIN)
  Duration( const timespec& time );
  operator timespec () const;

#endif
  explicit operator double () const;
  explicit operator float () const;

  bool operator == ( const Duration& other) const;
  bool operator != ( const Duration& other) const;
  bool operator < ( const Duration& other) const;
  bool operator > ( const Duration& other) const;
  bool operator <= ( const Duration& other) const;
  bool operator >= ( const Duration& other) const;

  Duration operator + ( const Duration& other ) const;
  Duration operator - ( const Duration& other ) const;

  Duration& operator += ( const Duration& other );
  Duration& operator -= ( const Duration& other );
};

} // namespace TimeUtils
