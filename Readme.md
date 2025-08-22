## What

(2025-08)

Time delays. ATmega328, 16 MHz.

Interface provides functions for micro-, milli- and second time delay.

Verified with oscilloscope. Implemented as wait loops.

## Code

* [Interface][Interface]
* [Example][Example]
* [Implementation][Implementation]

## Requirements

  * arduino-cli
  * bash

## Install/remove

Easy way is to clone [GetLibs][GetLibs] repo and run it's code.

## Compile

Zero-warnings compilation:

```bash
arduino-cli compile --fqbn arduino:avr:uno --quiet --warnings all . --build-property compiler.cpp.extra_flags="-std=c++1z"
```

## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Interface]: src/me_Delays.h
[Example]: examples/me_Delays/me_Delays.ino
[Implementation]: src/

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
