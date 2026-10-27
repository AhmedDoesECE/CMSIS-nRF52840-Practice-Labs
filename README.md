# nRF52840 CMSIS Practice Labs

I'm learning bare-metal C on the Nordic nRF52840 by writing straight to the registers, CMSIS style. No drivers, no SDK, no Arduino. Just the Product Specification, the MDK headers and a lot of time with a scope.

I came into this knowing register-level AVR from ECE 216 (ATmega2560), so every lab compares the nRF way of doing things with the AVR way I already know. The lab sequence and reviews come from an AI tutor (Claude). I wrote all of the code myself, made a prediction before every build, and checked every result on real hardware.

## Hardware and tools

| Item | Details |
| --- | --- |
| Board | nRF52840 DK (PCA10056) |
| LEDs | P0.13, P0.14, P0.15, P0.16 (active-low) |
| Buttons | P0.11, P0.12, P0.24, P0.25 (active-low) |
| Compiler | Arm GNU Toolchain 15.3 (`arm-none-eabi-gcc`) |
| Flashing | `nrfjprog` 10.24.2 with the on-board J-Link |
| Headers | CMSIS-Core and the Nordic MDK (`nrf.h`, `nrf52840_bitfields.h`) |
| Bench | Rigol DS1054Z oscilloscope, multimeter |

## Labs

- [x] [Lab 1: GPIO output](lab01-gpio-output/) - turn on the board LEDs from registers
- [x] [Lab 2: GPIO input](lab02-gpio-input/) - buttons control the LEDs
- [x] [Lab 3: TIMER and clock source](lab03-timer/) - exact 1 Hz blink, internal RC vs crystal
- [ ] Lab 4: RADIO I - board-to-board link
- [ ] Lab 5: SysTick
- [ ] Lab 6: NVIC and GPIOTE interrupts
- [ ] Lab 7: PPI
- [ ] Lab 8: UARTE
- [ ] Lab 9: PWM
- [ ] Lab 10: SAADC
- [ ] Lab 11: SPIM / TWIM
- [ ] Lab 12: RADIO II - streaming sensor data

Each lab folder has its own README with the goal, the registers I used, what I measured and the bugs I ran into.

## Building

Each lab is a single `main.c`. To build one, you also need the Nordic MDK startup file (`gcc_startup_nrf52840.S`), `system_nrf52840.c` and the nRF52840 linker script. Those aren't in this repo yet. I'm planning to add a shared `common/` folder with them and a Makefile.

## The best thing I've learned so far

The nRF52840 boots on its internal RC oscillator. In Lab 3, a timer set for a 1 s period measured 996 ms on it, 0.4% off. Starting the external crystal brought it to 1.000 s on the scope. If timing matters, start the crystal first.
