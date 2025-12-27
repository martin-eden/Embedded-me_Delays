// [me_Delays] test

/*
  Author: Martin Eden
  Last mod.: 2025-12-27
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_Pins.h>
#include <me_Duration.h>
#include <me_DebugPrints.h>

const TUint_1 OutputPinNum = 7;

void RunTest()
{
  /*
    We're turning ON output pin for constant amount of time.
    We're turning OFF output pin for constant amount of time.
    Then we're doubling durations and repeat it once.
  */
  TUint_2 Led_TimeOn_Ms = 20;
  TUint_2 Led_TimeOff_Ms = 10;
  TUint_1 StageDelay_Ms = 5;

  me_Pins::TOutputPin OutputPin;

  Console.Print("Duration test");
  Console.Indent();
  me_DebugPrints::Print("Output pin", OutputPinNum);
  me_DebugPrints::Print("Time ON (ms)", Led_TimeOn_Ms);
  me_DebugPrints::Print("Time OFF (ms)", Led_TimeOff_Ms);

  OutputPin.Init(OutputPinNum);
  OutputPin.Write(0);
  me_Delays::Delay_Ms(StageDelay_Ms);

  OutputPin.Write(1);
  me_Delays::Delay_Ms(Led_TimeOn_Ms);
  OutputPin.Write(0);
  me_Delays::Delay_Ms(Led_TimeOff_Ms);
  OutputPin.Write(1);
  me_Delays::Delay_Ms(StageDelay_Ms);

  OutputPin.Write(0);
  Led_TimeOn_Ms = 2 * Led_TimeOn_Ms;
  Led_TimeOff_Ms = 2 * Led_TimeOff_Ms;
  me_Delays::Delay_Ms(StageDelay_Ms);

  OutputPin.Write(1);
  me_Delays::Delay_Ms(Led_TimeOn_Ms);
  OutputPin.Write(0);
  me_Delays::Delay_Ms(Led_TimeOff_Ms);
  OutputPin.Write(1);
  me_Delays::Delay_Ms(StageDelay_Ms);

  OutputPin.Write(0);

  Console.Print("Done");
  Console.Unindent();
}

/*
  Infinite test for oscilloscope
*/
void RunInifiniteTest()
{
  const TUint_4
    On_Duration_Us = 20500,
    Off_Duration_Us = 10250;

  me_Pins::TOutputPin OutputPin;

  Console.Print("Infinite test for oscilloscope");

  OutputPin.Init(OutputPinNum);
  OutputPin.Write(0);

  while(1)
  {
    OutputPin.Write(1);
    me_Delays::Delay_Us(On_Duration_Us);
    OutputPin.Write(0);
    me_Delays::Delay_Us(Off_Duration_Us);
  }
}

void setup()
{
  Console.Init();

  Console.Print("[me_Delays] test");

  RunTest();
  // RunInifiniteTest();

  Console.Print("Done");
}

void loop()
{
}

/*
  2025-08-21
  2025-09-12
  2025-10-25
*/
