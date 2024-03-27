#include <TimeUtils/Duration.hpp>

#if defined(TIME_UTILS_MAC)
  #include <TimeUtils/platform/PosixMachTiming.hpp>
#endif

#include <cmath>


namespace TimeUtils
{

const static int64_t NSEC_IN_SEC = 1'000'000'000;

Duration
Now()
{
#if defined TIME_UTILS_WIN
  LARGE_INTEGER currentTime;
  QueryPerformanceCounter( &currentTime );

#elif defined(TIME_UTILS_LIN) || defined(TIME_UTILS_MAC)
  timespec currentTime;
  clock_gettime( CLOCK_MONOTONIC, &currentTime );

#endif

  return currentTime;
}

bool
Sleep(
  const Duration& duration )
{
#if defined(TIME_UTILS_WIN) || defined(TIME_UTILS_MAC)
  const Duration timestamp = TimeUtils::Now() + duration;
  return SleepUntil( timestamp );

#else
  timespec timeToSleep = duration;

  return clock_nanosleep( 
    CLOCK_MONOTONIC, 0, 
    &timeToSleep, NULL ) == 0;

#endif
}

bool
SleepUntil(
  const Duration& timestamp )
{
#if defined TIME_UTILS_WIN
  while ( TimeUtils::Now() < timestamp )
//    It's the most accurate solution as we can get
//    as long as timer resolution is minimum
//    (see TimePeriodInit())
    ::Sleep(1);

#elif defined(TIME_UTILS_MAC)
  timespec timeToSleep = duration;
  return clock_nanosleep_abstime(&timeToSleep) == 0;

#else
  timespec timeToSleep = timestamp;

  return clock_nanosleep(
    CLOCK_MONOTONIC, TIMER_ABSTIME, 
    &timeToSleep, NULL ) == 0;

#endif
}

void
TimePeriodInit()
{
#if defined TIME_UTILS_WIN
  timeBeginPeriod(1);
#endif
}

void
TimePeriodDeinit()
{
#if defined TIME_UTILS_WIN
  timeEndPeriod(1);
#endif
}


Duration::Duration(
  const double time )
  : mSec(static_cast <int64_t> ( std::trunc(time) ))
  , mNSec(static_cast <int64_t> ( time * NSEC_IN_SEC ) % NSEC_IN_SEC)
{}

Duration::Duration(
  const int64_t sec,
  const int64_t nsec )
  : mSec(sec)
  , mNSec(nsec)
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

#if defined (TIME_UTILS_WIN)
Duration::Duration(
  const LARGE_INTEGER& time )
{
  LARGE_INTEGER perfFreq;
  QueryPerformanceFrequency( &perfFreq );

//  Note: Windows has 100 ns time stamp resolution
  mSec = time.QuadPart / perfFreq.QuadPart;
  mNSec = (time.QuadPart % perfFreq.QuadPart) * NSEC_IN_SEC / perfFreq.QuadPart;
}

Duration::operator LARGE_INTEGER () const
{
  LARGE_INTEGER perfFreq, value;

  QueryPerformanceFrequency( &perfFreq );
  value.QuadPart = mSec * perfFreq.QuadPart + mNSec * perfFreq.QuadPart / NSEC_IN_SEC;

  return value;
}

#elif defined (TIME_UTILS_LIN)
Duration::Duration(
  const struct timespec& time )
{
  mSec = time.tv_sec;
  mNSec = time.tv_nsec;
}

Duration::operator timespec () const
{
  timespec value;

  value.tv_sec  = mSec;
  value.tv_nsec = mNSec;

  return value;
}

#endif

Duration::operator double () const
{
  return
    mSec + mNSec / static_cast <double> ( NSEC_IN_SEC );
}

Duration::operator float () const
{
  return
    mSec + mNSec / static_cast <double> ( NSEC_IN_SEC );
}

bool
Duration::operator == ( const Duration& other ) const
{
  return
    this->mSec == other.mSec &&
    this->mNSec == other.mNSec;
}

bool
Duration::operator != ( const Duration& other ) const
{
  return !( *this == other );
}

bool
Duration::operator < ( const Duration& other ) const
{
  if ( this->mSec == other.mSec )
    return this->mNSec < other.mNSec;

  return this->mSec < other.mSec;
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

  result.mSec = this->mSec + other.mSec;
  result.mNSec = this->mNSec + other.mNSec;

  while ( result.mNSec >= NSEC_IN_SEC )
  {
    ++result.mSec;
    result.mNSec -= NSEC_IN_SEC;
  }

  return result;
}

Duration
Duration::operator - ( const Duration& other ) const
{
  Duration result;

  result.mSec = this->mSec - other.mSec;
  result.mNSec = this->mNSec - other.mNSec;

  while ( result.mNSec < 0 )
  {
    --result.mSec;
    result.mNSec += NSEC_IN_SEC;
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
