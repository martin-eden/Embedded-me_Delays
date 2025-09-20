// Delays implementation

/*
  Author: Martin Eden
  Last mod.: 2025-09-20
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>
#include <me_Duration.h>

#include <me_Delays.Freetown.h>

#include <avr/common.h>
#include <avr/interrupt.h>

using namespace me_Delays;

const TUint_2 MaxCapacity = 9999;

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

  TUint_1 OrigSreg = SREG;

  cli();

  for (TUint_2 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
  {
    Freetown::DelayMicrosecond();
  }

  SREG = OrigSreg;

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

    So

      Num runs = Num micros * 16 / 20

    There also should be setup cost time constant, we're
    subtracting it.
  */
}

/*
  Delay for given amount of milliseconds
*/
TBool me_Delays::Delay_Ms(
  TUint_2 NumMillis
)
{
  /*
    For millis we ignore cycle overhead. It's under 2 us
  */

  if (NumMillis == 0)
    return true;

  if (NumMillis > MaxCapacity)
    return false;

  TUint_1 OrigSreg = SREG;

  cli();

  for (TUint_2 RunNumber = 1; RunNumber <= NumMillis; ++RunNumber)
  {
    Freetown::DelayMillisecond();
  }

  SREG = OrigSreg;

  return true;
}

/*
  Delay for given amount of seconds
*/
TBool me_Delays::Delay_S(
  TUint_2 NumSecs
)
{
  /*
    For seconds we're not disabling interrupts
  */

  if (NumSecs == 0)
    return true;

  if (NumSecs > MaxCapacity)
    return false;

  for (TUint_2 RunNumber = 1; RunNumber <= NumSecs; ++RunNumber)
  {
    Freetown::DelaySecond();
  }

  return true;
}

/*
  Delay for duration record
*/
TBool me_Delays::Delay_Duration(
  me_Duration::TDuration Duration
)
{
  for (TUint_2 RunNumber = 1; RunNumber <= Duration.KiloS; ++RunNumber)
  {
    if (!Delay_S(1000))
      return false;
  }

  if (!Delay_S(Duration.S))
    return false;

  if (!Delay_Ms(Duration.MilliS))
    return false;

  Delay_Us(Duration.MicroS);

  return true;

  /*
    We're not trying any corrections here

    It's futile. You can't directly measure this function delay
    with oscilloscope.

    You have code like

      Led.Write(1); // (1)
      Delay_Duration({ 0, 0, 0, 80 });
      Led.Write(0); // (2)

    And in oscilloscope you'll see wave goes high somewhere inside (1)
    and goes low somewhere inside (2).

    We can try compensate time spent in this function. But at the end
    of day you want to see wave of wished length on oscilloscope.

    For this you need to do duration corrections outside of this code.
  */
}

/*
  2025-08-21
  2025-09-12
*/
