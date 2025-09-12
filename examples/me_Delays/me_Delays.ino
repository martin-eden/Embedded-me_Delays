// [me_Delays] test

/*
  Author: Martin Eden
  Last mod.: 2025-09-12
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_Pins.h>
#include <me_Timestamp.h>

void RunTest()
{
  /*
    We're turning on LED for constant amount of time.
    We're turning off LED for constant amount of time.
  */
  const TUint_1 NumRuns = 3;

  const TUint_2
    Led_TimeOn_Ms = 1000,
    Led_TimeOff_Ms = 200;

  const TUint_1 Led_Pin = 13;

  me_Pins::TOutputPin Led;

  Led.Init(Led_Pin);
  Led.Write(0);

  for (TUint_1 RunNumber = 1; RunNumber <= NumRuns; ++RunNumber)
  {
    Console.Print(RunNumber);
    Console.EndLine();

    Led.Write(1);
    me_Delays::Delay_Ms(Led_TimeOn_Ms);
    Led.Write(0);
    me_Delays::Delay_Ms(Led_TimeOff_Ms);
  }

  Led.Write(0);
}

/*
  Infinite test for oscilloscope
*/
void RunInifiniteTest()
{
  const TUint_1 Led_Pin = 13;
  const me_Timestamp::TTimestamp
    Led_Wave_Duration = { 0, 0, 0, 160 };

  me_Pins::TOutputPin Led;

  Led.Init(Led_Pin);
  Led.Write(0);

  while(1)
  {
    Led.Write(1);
    me_Delays::Delay_Duration(Led_Wave_Duration);
    Led.Write(0);
    me_Delays::Delay_Duration(Led_Wave_Duration);
  }
}

void setup()
{
  Console.Init();

  Console.Print("[me_Delays] test");
  RunTest();
  Console.Print("Done");

  Console.Print("Running infinite test for oscilloscope now.");
  RunInifiniteTest();
}

void loop()
{
}

/*
  2025-08-21
  2025-09-12
*/
