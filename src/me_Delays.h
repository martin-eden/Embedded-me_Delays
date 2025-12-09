// Delays interface

/*
  Author: Martin Eden
  Last mod.: 2025-12-10
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Duration.h>

namespace me_Delays
{
  /*
    Max guaranteed value for single-unit delay functions is 9999
  */

  // Microseconds delay
  void Delay_Us(TUint_2 NumMicros);

  // Milliseconds delay
  void Delay_Ms(TUint_2 NumMillis);

  // Seconds delay
  void Delay_S(TUint_2 NumSecs);

  // Kiloseconds delay
  void Delay_Ks(TUint_2 NumKilos);

  // Delay for duration record
  void Delay_Duration(me_Duration::TDuration Duration);
}

/*
  2025-08-21
  2025-10-25
  2025-10-27
  2025-10-30
  2025-11-28 Delays are void functions now
*/
