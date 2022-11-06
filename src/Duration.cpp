#include <TimeUtils/Duration.hpp>

#include <cmath>

#if defined(_WIN32)
  #include <windows.h>
#endif


namespace TimeUtils
{

const static long NSEC_IN_SEC = 1'000'000'000;

Duration
Now()
{
  timespec currentTime;
  clock_gettime( CLOCK_MONOTONIC, &currentTime );

  return currentTime;
}

int
Sleep( const Duration& duration )
{
  const auto timeToSleep = static_cast <timespec> (duration);

  return clock_nanosleep( CLOCK_MONOTONIC, 0,
                          &timeToSleep, NULL );
}

int
SleepUntil( const Duration& timestamp )
{
  const auto timeToSleep = static_cast <timespec> (timestamp);

  return clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME,
                          &timeToSleep, NULL );
}

void
TimePeriodInit()
{
#if defined(_WIN32)
  timeBeginPeriod(1);
#endif
}

void
TimePeriodDeinit()
{
#if defined(_WIN32)
  timeEndPeriod(1);
#endif
}


Duration::Duration( const struct timespec& time )
  : mTime{time}
{}

Duration::Duration( const double time )
  : mTime{static_cast <time_t> ( std::trunc(time) ),
          static_cast <long> (time * NSEC_IN_SEC) % NSEC_IN_SEC}
{}

Duration::Duration(
  const time_t sec,
  const long nsec )
  : mTime{sec, nsec}
{}

bool
Duration::sleep() const
{
  return TimeUtils::Sleep( *this );
}

Duration
Duration::now()
{
  return *this = TimeUtils::Now();
}

Duration::operator timespec () const
{
  return mTime;
}

Duration::operator double () const
{
  return
    mTime.tv_sec + mTime.tv_nsec / static_cast <double> (NSEC_IN_SEC);
}

Duration::operator float () const
{
  return static_cast <double> (*this);
}

bool
Duration::operator == ( const Duration& other ) const
{
  return
    this->mTime.tv_sec == other.mTime.tv_sec &&
    this->mTime.tv_nsec == other.mTime.tv_nsec;
}

bool
Duration::operator != ( const Duration& other ) const
{
  return !( *this == other );
}

bool
Duration::operator < ( const Duration& other ) const
{
  if ( this->mTime.tv_sec == other.mTime.tv_sec )
    return this->mTime.tv_nsec < other.mTime.tv_nsec;

  return this->mTime.tv_sec < other.mTime.tv_sec;
}

bool
Duration::operator > ( const Duration& other ) const
{
  return other < *this;
}

bool
Duration::operator <= ( const Duration& other ) const
{
  return !( *this > other );
}

bool
Duration::operator >= ( const Duration& other ) const
{
  return !( *this < other );
}

Duration
Duration::operator + ( const Duration& other ) const
{
  Duration result;

  result.mTime.tv_sec = this->mTime.tv_sec + other.mTime.tv_sec;
  result.mTime.tv_nsec = this->mTime.tv_nsec + other.mTime.tv_nsec;

  while ( result.mTime.tv_nsec >= NSEC_IN_SEC )
  {
    ++result.mTime.tv_sec;
    result.mTime.tv_nsec -= NSEC_IN_SEC;
  }

  return result;
}

Duration
Duration::operator - ( const Duration& other ) const
{
  Duration result;

  result.mTime.tv_sec = this->mTime.tv_sec - other.mTime.tv_sec;
  result.mTime.tv_nsec = this->mTime.tv_nsec - other.mTime.tv_nsec;

  while ( result.mTime.tv_nsec < 0 )
  {
    --result.mTime.tv_sec;
    result.mTime.tv_nsec += NSEC_IN_SEC;
  }

  return result;
}

Duration&
Duration::operator += ( const Duration& other )
{
  return *this = *this + other;
}

Duration&
Duration::operator -= ( const Duration& other )
{
  return *this = *this - other;
}

} // namespace TimeUtils
