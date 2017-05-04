This is a simple UART to single channel PWM converter based on Texas Instrument's C2000 Launchpad Kit (LAUNCHXL-F28027).

I needed a PWM generator for other project with the ability to change the PWM frequency and duty easily, so I've written this code.
After flashing the MCU, one can control the PWM output using a terminal (e.g. Putty).

The details are following:
- the MCU runs at max frequency (60 MHz)
- the UART runs at fastest baud rate possible for this board (3.75MHz, so it's 3750000 baud)
- PWM is configured for ePWM4 channel A, which is GPIO6
- PWM runs in the simplest of modes, which is counting up, clearing the counter after reaching the period
- user can set the PWM values (divisors, period, compare) using a terminal menu
- the code prints the status after each change with calculated PWM frequency, duty and other parameters
- you need to include C2000 Launchpad headers ("XXX\TIcontrolSUITE\development_kits\C2000_LaunchPad") to compile (the code uses HAL drivers from TI)
- written in Code Composer Studio 6.1.1

Enjoy,
Mikolaj Stawiski
