// Delays interface

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Delays
{
  // [Core] Microseconds delay
  void Delay_Us(TUint_4);

  // [Handy] Milliseconds delay
  void Delay_Ms(TUint_2);
  // [Handy] Seconds delay
  void Delay_S(TUint_2);
}

/*
  2025-08-21
  2025-10-25
  2025-10-27
  2025-10-30
  2025-11-28 Delays are void functions now
  2025-12-27 Core delay function now uses TUint_4, not TDuration
*/
