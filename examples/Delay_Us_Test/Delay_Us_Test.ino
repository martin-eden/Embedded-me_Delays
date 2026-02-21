// [Tool] Test of microsecond delay. Verify with oscilloscope

/*
  Author: Martin Eden
  Last mod.: 2026-02-21
*/

#include <me_Delays.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include <me_Menu.h>
#include <me_Pins.h>
#include <me_Interrupts.h>

static TUint_2 EmitDuration_Us = 600;
static const TUint_1 OutputPinNum = 6;

void SetDuration_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  TUint_2 Duration_Us;

  Console.Write("Enter duration (in microseconds): ");

  if (!Console.Read(&Duration_Us))
  {
    Console.Print("Failed to parse");
    return;
  }

  Duration_Us = Duration_Us % 1000;

  EmitDuration_Us = Duration_Us;
}

void GetDuration_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  me_DebugPrints::Print("Duration (us)", EmitDuration_Us);
  Console.EndLine();
}

void Emit_Handler(
  TAddress Data [[gnu::unused]],
  TAddress Instance [[gnu::unused]]
)
{
  const TUint_4 Compensation_Us = 43;

  me_Pins::TOutputPin OutputPin;

  if (EmitDuration_Us < Compensation_Us)
    return;

  OutputPin.Init(OutputPinNum);

  {
    me_Interrupts::TInterruptsDisabler NoInts;

    OutputPin.Write(1);
    me_Delays::Delay_Us(EmitDuration_Us - Compensation_Us);
    OutputPin.Write(0);
  }
}

void AddMenuItems(
  me_Menu::TMenu * Menu
)
{
  using
    me_Menu::Freetown::ToItem;

  const TAddress Unused = 0;

  Menu->AddItem(ToItem("sd", "Set duration (in microseconds)", SetDuration_Handler, Unused));
  Menu->AddItem(ToItem("gd", "Get duration", GetDuration_Handler, Unused));
  Menu->AddItem(ToItem("e", "Emit for given duration", Emit_Handler, Unused));
}


void setup()
{
  me_Menu::TMenu Menu;

  Console.Init();

  AddMenuItems(&Menu);
  Menu.AddBuiltinCommands();

  Menu.Print();

  Menu.Run();
  Console.Print(") Done");
}

void loop()
{
}

/*
  2025-10-27
*/
