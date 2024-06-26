#pragma once

#include <TimeUtils/Duration.hpp>


namespace TimeUtils
{

class TIMEUTILS_EXPORT Timer
{
  using Duration = TimeUtils::Duration;

  bool mCounting;

  Duration mDuration;
  Duration mRemainingTime;


public:
  Timer( const Duration&, bool start = false );

  void start();
  void start( const Duration& newDuration );
  void restart();
  void stop();

  Duration update( const Duration& dt );

  Duration duration() const;
  Duration remaining() const;
  Duration elapsed() const;

  bool isReady() const;
};

} // namespace TimeUtils
