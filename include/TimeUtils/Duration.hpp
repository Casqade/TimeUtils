#pragma once

#include <TimeUtils/TimeUtils_export.hpp>

#include <ctime>


namespace TimeUtils
{

class Duration;

TIMEUTILS_EXPORT Duration Now();
TIMEUTILS_EXPORT int Sleep( const Duration& duration );
TIMEUTILS_EXPORT int SleepUntil( const Duration& timeStamp );

TIMEUTILS_EXPORT void TimePeriodInit();
TIMEUTILS_EXPORT void TimePeriodDeinit();

class TIMEUTILS_EXPORT Duration
{
  timespec mTime {};

public:
  Duration() = default;

  Duration( const timespec& time );
  Duration( const double time );

  Duration( const time_t sec,
            const long nsec );

  bool sleep() const;
  Duration now();

  explicit operator timespec () const;
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
