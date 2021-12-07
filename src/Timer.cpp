#include <TimeUtils/Timer.hpp>


namespace TimeUtils
{

Timer::Timer( const Duration& duration,
              bool start )
  : mCounting(start)
  , mDuration(duration)
  , mRemainingTime(duration)
{}

void
Timer::start()
{
  mCounting = true;
}

void
Timer::start( const Duration& newDuration )
{
  mDuration = mRemainingTime = newDuration;

  start();
}

void
Timer::restart()
{
  start( mDuration );
}

void
Timer::stop()
{
  mCounting = false;
}

Duration
Timer::update( const Duration& dt )
{
  if ( isReady() )
    return mRemainingTime;

  if ( mRemainingTime > Duration() )
  {
    mRemainingTime -= dt;
    return mRemainingTime;
  }

  mRemainingTime = Duration();
  stop();

  return mRemainingTime;
}

Duration
Timer::duration() const
{
  return mDuration;
}

Duration
Timer::remaining() const
{
  return mRemainingTime;
}

Duration
Timer::elapsed() const
{
  return mDuration - mRemainingTime;
}

bool
Timer::isReady() const
{
  return mRemainingTime <= Duration();
}

} // namespace TimeUtils
