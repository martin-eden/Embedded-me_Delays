// Delays interface

/*
  Author: Martin Eden
  Last mod.: 2025-10-30
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Duration.h>

namespace me_Delays
{
  /*
    Max value for single-unit delay functions is 9999

    For 9050 ms delay you can use "Delay_Ms(9050);" or
    "Delay_S(9); Delay_Ms(50);".

    But delay 10050 ms is done only by "Delay_S(10); Delay_Ms(50);".
  */

  // Init
  void Init();

  // Microseconds delay
  TBool Delay_Us(TUint_2 NumMicros);

  // Milliseconds delay
  TBool Delay_Ms(TUint_2 NumMillis);

  // Seconds delay
  TBool Delay_S(TUint_2 NumSecs);

  // Delay for duration record, milli-second granularity. Cheap
  TBool Delay_Duration(me_Duration::TDuration Duration);

  // Delay for duration record, micro-second granularity. Expensive
  TBool Delay_PreciseDuration(me_Duration::TDuration Duration);
}

/*
  2025-08-21
  2025-10-25
  2025-10-27
  2025-10-30
*/
