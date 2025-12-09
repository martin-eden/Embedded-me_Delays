// Delays implementation

/*
  Author: Martin Eden
  Last mod.: 2025-12-10
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

  Minimum accepted microseconds: 4
  Maximum accepted microseconds: 9999 (MaxCapacity)
*/
[[gnu::noinline]] void me_Delays::Delay_Us(
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
    return;

  if (NumMicros > MaxCapacity)
    return;

  // See note at end of function
  NumRuns =
    (TUint_4) (NumMicros - SetupCost_Us) *
      Micro_Ticks / (Micro_Ticks + CycleOverhead_Ticks);

  if (NumRuns == 0)
    return;

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

  /*
    Call of DelayMicrosecond() takes <Micro_Ticks>.

    Cycle iteration with call of DelayMicrosecond()
    takes additional <CycleOverhead_Ticks>.
  */
}

/*
  Delay for given amount of milliseconds
*/
void me_Delays::Delay_Ms(
  TUint_2 NumMillis
)
{
  TUint_2 RunNumber;

  for (RunNumber = 1; RunNumber <= NumMillis; ++RunNumber)
    Delay_Us(1000);
}

/*
  Delay for given amount of seconds
*/
void me_Delays::Delay_S(
  TUint_2 NumSecs
)
{
  TUint_2 RunNumber;

  for (RunNumber = 1; RunNumber <= NumSecs; ++RunNumber)
    Delay_Ms(1000);
}

/*
  Delay for given amount of kiloseconds
*/
void me_Delays::Delay_Ks(
  TUint_2 NumKilos
)
{
  TUint_2 RunNumber;

  for (RunNumber = 1; RunNumber <= NumKilos; ++RunNumber)
    Delay_S(1000);
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

  Granularity is GetTime() period duration.
*/
void me_Delays::Delay_Duration(
  me_Duration::TDuration Duration
)
{
  me_Duration::TDuration EndTime;

  EndTime = me_RunTime::GetTime();
  me_Duration::CappedAdd(&EndTime, Duration);

  while (me_Duration::IsLess(me_RunTime::GetTime(), EndTime));
}

/*
  Delay for duration record
*/
void me_Delays::Delay_PreciseDuration(
  me_Duration::TDuration Duration
)
{
  /*
    It's still not perfect delay as we're not disabling interrupts

    But it's simple: we're splitting time interval into big and small
    parts.

    For big part we call Delay_Duration() which has no adverse effects.
    For small part we're actualizing current time, converting remained
    time to micros and call Delay_Us() which is precise.

    We're losing some time doing conversions after getting precise
    time. This can be compensated in outer code.

    And anyway, even we return from here after spending exact requested
    amount of time interrupt can occur and delay your important
    TurnOff() function which typically follows.

    For that stuff you currently have to write your own delay function
    which turns off interrupts before getting precise delay for small
    part. And then calling your TurnOff() before enabling interrupts
    again. At moment of writing [me_ModulatedSignalPlayer] doing this.
  */
  me_Duration::TDuration EndTime;
  me_Duration::TDuration CurTime;
  me_Duration::TDuration TimeRemained;
  TUint_4 TimeRemained_Us;
  me_Duration::TDuration RoughDuration;

  RoughDuration = Duration;
  me_Duration::CappedSub(&RoughDuration, me_RunTime::GetPeriodDuration());

  EndTime = me_RunTime::GetTime_Precise();
  me_Duration::CappedAdd(&EndTime, Duration);

  Delay_Duration(RoughDuration);

  CurTime = me_RunTime::GetTime_Precise();

  TimeRemained = EndTime;
  me_Duration::CappedSub(&TimeRemained, CurTime);

  me_Duration::DurationToMicros(&TimeRemained_Us, TimeRemained);

  Delay_Us(TimeRemained_Us);
}

/*
  2025-08-21
  2025-09-12
  2025-10-25
  2025-10-26
  2025-10-27
  2025-11-17
  2025-12-10
*/
