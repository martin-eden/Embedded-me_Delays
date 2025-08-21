// Delays interface

/*
  Author: Martin Eden
  Last mod.: 2025-08-21
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Delays
{
  /*
    All delay functions have up to 10x overload capability

    Delay 9050 ms: Delay_Ms(9050) equal to Delay_S(9); Delay_Ms(50);

    But delay 10050 ms only equal to Delay_S(10); Delay_Ms(50);
  */

  // Microseconds delay
  TBool Delay_Us(TUint_2 NumMicros);

  // Milliseconds delay
  TBool Delay_Ms(TUint_2 NumMillis);

  // Seconds delay
  TBool Delay_S(TUint_2 NumSecs);
}

/*
  2025-08-21
*/
