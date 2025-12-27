// Delays implementation

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

// Implementation is fixed to 16 MHz, ATmega328

/*
  Composition

    * One microsecond delay

      Call of function, execution and return takes exactly 16 ticks

    * TUint_2 microseconds delay

      Calls microsecond delay from compensated cycle

    * TUint_4 microseconds delay

      Calls TUint_2-delay from uncompensated cycle
*/

/*
  Notes

    TUint_2 because we can subtract from word via SBIW.

    I've tried to write just TUint_4 delay. But making it perfectly
    precise is hard. Tho I think it's possible and maybe I can do it,
    resulting code will be hard to understand.

    So current approach is TUint_4 consisting from precise TUint_2
    blocks.

    Gaps between blocks are not compensated.

    So delay is not perfect. Longer delays will give more overshoot.
    Still, overshoot is maybe several milliseconds for minutes-range
    delays. It's practical.
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>

using namespace me_Delays;

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
  Delay for given TUint_2 of microseconds

  Minimum accepted microseconds: 4

  Delay is
*/
[[gnu::noinline]] static void Delay_Us_U2(
  TUint_2 NumMicros
)
{
  /*
    We're doing correction in number of calls to adjust for setup cost
    and cycle overhead.
  */

  const TUint_1
    SetupCost_Us = 2,
    Micro_Ticks = 16,
    CycleOverhead_Ticks = 4;

  TUint_2 NumRuns;

  if (NumMicros <= SetupCost_Us)
    return;

  NumMicros = NumMicros - SetupCost_Us;

  // See note at end of function
  NumRuns =
    (TUint_4) NumMicros * Micro_Ticks / (Micro_Ticks + CycleOverhead_Ticks);

  if (NumRuns == 0)
    return;

  /*
    I'm sick and tired of GCC randomly encoding simple iteration
    at least two different ways.
  */
  asm volatile
  (
    R"(
      Start:
        call DelayMicrosecond ; 4
        sbiw %[NumRuns], 1 ; 2
        brne Start ; 2
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
  Delay for given TUint_4 of microseconds
*/
void me_Delays::Delay_Us(
  TUint_4 NumMicros
)
{
  TUint_2 NumBlocks;
  TUint_2 Remainder;

  NumBlocks = (NumMicros >> 16L) & 0xFFFF;
  Remainder = (NumMicros >> 0L) & 0xFFFF;

  Delay_Us_U2(Remainder);

  while (NumBlocks > 0)
  {
    Delay_Us_U2(TUint_2_Max);
    --NumBlocks;
  }
}

/*
  [Handy] Milliseconds delay
*/
void me_Delays::Delay_Ms(
  TUint_2 NumMillis
)
{
  Delay_Us(1000L * NumMillis);
}

/*
  [Handy] Seconds delay
*/
void me_Delays::Delay_S(
  TUint_2 NumSecs
)
{
  const TUint_2 MaxNumSecs = 3999;

  if (NumSecs > MaxNumSecs)
    NumSecs = MaxNumSecs;

  Delay_Us(1000000L * NumSecs);
}

/*
  2025-08-21
  2025-09-12
  2025-10-25
  2025-10-26
  2025-10-27
  2025-11-17
  2025-12-10 Removed run-time clock usage
  2025-12-27 Removed TDuration usage. TUint_4 delay instead
*/
