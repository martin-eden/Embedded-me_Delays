// Delays. Core implementation

/*
  Author: Martin Eden
  Last mod.: 2025-10-24
*/

#include <me_Delays.Freetown.h>

#include <me_BaseTypes.h>

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
  Delay for one millisecond
*/
[[gnu::noinline]] void me_Delays::Freetown::DelayMillisecond()
{
  /*
    See math comment for "Delay_Us()". I didn't count overhead
    in disassembly for this, but 16 / 20 works great.
  */
  const TUint_2 NumRuns = 1000 * 16 / 20;

  for (TUint_2 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
  {
    DelayMicrosecond();
  }
}

/*
  Delay for one second
*/
[[gnu::noinline]] void me_Delays::Freetown::DelaySecond()
{
  const TUint_2 NumRuns = 999;

  for (TUint_2 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
  {
    DelayMillisecond();
  }
}

/*
  2025-08-21
*/
