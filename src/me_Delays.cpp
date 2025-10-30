// Delays implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-30
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
extern "C" [[gnu::noinline, gnu::used]] void DelayMicrosecond();
void DelayMicrosecond()
{
  /*
    For 16 MHz

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

  const TUint_1
    SetupCost_Us = 2,
    Micro_Ticks = 16,
    CycleOverhead_Ticks = 4;

  TUint_2 NumRuns;

  if (NumMicros <= SetupCost_Us)
    return false;

  if (NumMicros > MaxCapacity)
    return false;

  // See note at end of function
  NumRuns =
    (TUint_4) (NumMicros - SetupCost_Us) *
      Micro_Ticks / (Micro_Ticks + CycleOverhead_Ticks);

  /*
    We need asm for

      for (TUint_2 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
        Freetown::DelayMicrosecond();

    I'm sick and tired of GCC randomly encoding simple iteration
    at least two different ways.
  */
  asm volatile
  (
    R"(
      Start:
        call DelayMicrosecond
        sbiw %[NumRuns], 1
        brne Start
    )"
    : [NumRuns] "+w" (NumRuns)
  );

  return true;

  /*
    Call of DelayMicrosecond() takes <Micro_Ticks>.

    Cycle iteration with call of DelayMicrosecond()
    takes additional <CycleOverhead_Ticks>.
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

  Granularity is one millisecond. (Precision of GetTime().)
*/
TBool me_Delays::Delay_Duration(
  me_Duration::TDuration Duration
)
{
  me_Duration::TDuration EndTime;

  Duration.MicroS = 0;

  EndTime = me_RunTime::GetTime();
  if (!me_Duration::Add(&EndTime, Duration))
    return false;

  while (me_Duration::IsLess(me_RunTime::GetTime(), EndTime));

  return true;
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
  2025-10-25
  2025-10-26
  2025-10-27
*/
