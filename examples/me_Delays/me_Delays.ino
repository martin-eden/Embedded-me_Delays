// [me_Delays] test

/*
  Author: Martin Eden
  Last mod.: 2025-10-25
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
  */
  const TUint_2 Led_TimeOn_Ms = 50;
  const TUint_2 Led_TimeOff_Ms = 10;
  const TUint_1 NumRuns = 2;

  me_Pins::TOutputPin OutputPin;

  Console.Print("Duration test");
  Console.Indent();
  me_DebugPrints::Print("Output pin", OutputPinNum);
  me_DebugPrints::Print("Time ON (ms)", Led_TimeOn_Ms);
  me_DebugPrints::Print("Time OFF (ms)", Led_TimeOff_Ms);
  me_DebugPrints::Print("Number of runs", NumRuns);

  OutputPin.Init(OutputPinNum);
  OutputPin.Write(0);

  for (TUint_1 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
  {
    OutputPin.Write(1);
    me_Delays::Delay_Ms(Led_TimeOn_Ms);
    OutputPin.Write(0);
    me_Delays::Delay_Ms(Led_TimeOff_Ms);
  }

  Console.Print("Done");
  Console.Unindent();
}

/*
  Infinite test for oscilloscope
*/
void RunInifiniteTest()
{
  const me_Duration::TDuration
    On_Duration = { 0, 0, 20, 500 },
    Off_Duration = { 0, 0, 10, 250 };

  me_Pins::TOutputPin OutputPin;

  Console.Print("Infinite test for oscilloscope");

  OutputPin.Init(OutputPinNum);
  OutputPin.Write(0);

  while(1)
  {
    OutputPin.Write(1);
    me_Delays::Delay_Duration(On_Duration);
    OutputPin.Write(0);
    me_Delays::Delay_Duration(Off_Duration);
  }
}

void setup()
{
  Console.Init();
  me_Delays::Init();

  Console.Print("[me_Delays] test");

  RunTest();
  RunInifiniteTest();

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
