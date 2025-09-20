// Delays interface

/*
  Author: Martin Eden
  Last mod.: 2025-09-20
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Duration.h>

namespace me_Delays
{
  /*
    Single-unit delay functions have up to 10x overload capability

    Delay 9050 ms: Delay_Ms(9050) equal to Delay_S(9); Delay_Ms(50);

    But delay 10050 ms only equal to Delay_S(10); Delay_Ms(50);
  */

  // Microseconds delay
  TBool Delay_Us(TUint_2 NumMicros);

  // Milliseconds delay
  TBool Delay_Ms(TUint_2 NumMillis);

  // Seconds delay
  TBool Delay_S(TUint_2 NumSecs);

  // Delay for duration record
  TBool Delay_Duration(me_Duration::TDuration Duration);
}

/*
  2025-08-21
*/
