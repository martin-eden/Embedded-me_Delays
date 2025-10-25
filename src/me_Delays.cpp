// Delays implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-25
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>

#include <me_Duration.h>
#include <me_RunTime.h>

using namespace me_Delays;

const TUint_2 MaxCapacity = 9999;

/*
  Delay for one microsecond
*/
[[gnu::noinline]] void me_Delays::Freetown::DelayMicrosecond()
{
  /*
    For 16 MHz

    Damn, for 16 MHz it's easier to hardcode and tune delay
    than to code and tune delay loops.

    Number of NOPs account for costs of CALL and RET for this function.
  */
  asm volatile
  (
    R"(
      nop
      nop
      nop
      nop

      nop
      nop
      nop
      nop
    )"
  );
}

/*
  Delay for given amount of microseconds

  Minimum accepted microseconds: 3
*/
[[gnu::noinline]] TBool me_Delays::Delay_Us(
  TUint_2 NumMicros
)
{
  /*
    For micros we're doing heavy correction in number of calls
    to adjust for setup and cycle overhead.
  */

  const TUint_2 SetupCost_Us = 3;

  if (NumMicros <= SetupCost_Us)
    return false;

  if (NumMicros > MaxCapacity)
    return false;

  // See note at end of function
  TUint_2 NumRuns = (TUint_4) NumMicros * 16 / 20 - SetupCost_Us;

  for (TUint_2 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
    Freetown::DelayMicrosecond();

  return true;

  /*
    Disassembly-based math

      Num micros = Num ticks / 16
      Num ticks = Num runs * Run execution time (ticks)
      Run execution time (ticks) = 4 + 16   // 20

    4 ?

      Generated code is

        call 0x274
        sbiw r28, 0x01
        brne .-8

      "sbiw" cost is 2, "brne" 2. "call"'s cost is already accounted.

    16 ?

      One microsecond is 16 ticks at 16 MHz

    So

      Num runs = Num micros * 16 / 20

    There also should be setup cost time constant,
    we're subtracting it.
  */
}

/*
  Start run-time tracker that we're using
*/
void me_Delays::Init()
{
  me_RunTime::Init();
  me_RunTime::Start();
}

/*
  Delay for duration record
*/
TBool me_Delays::Delay_Duration(
  me_Duration::TDuration Duration
)
{
  me_Duration::TDuration StartTime;
  me_Duration::TDuration EndTime_Trimmed;
  TUint_2 EndTime_Micros;

  StartTime = me_RunTime::GetTime_Precise();

  EndTime_Trimmed = StartTime;
  if (!me_Duration::Add(&EndTime_Trimmed, Duration))
    return false;
  EndTime_Micros = EndTime_Trimmed.MicroS;
  EndTime_Trimmed.MicroS = 0;

  while (me_Duration::IsLessOrEqual(me_RunTime::GetTime(), EndTime_Trimmed));

  return Delay_Us(EndTime_Micros);
}

/*
  Delay for given amount of milliseconds
*/
TBool me_Delays::Delay_Ms(
  TUint_2 NumMillis
)
{
  return Delay_Duration({ 0, 0, NumMillis, 0 });
}

/*
  Delay for given amount of seconds
*/
TBool me_Delays::Delay_S(
  TUint_2 NumSecs
)
{
  return Delay_Duration({ 0, NumSecs, 0, 0 });
}

/*
  2025-08-21
  2025-09-12
*/
