# Lab 2: GPIO Input

**Goal:** Button 1 (P0.11) controls LED1 (P0.13). Hold it and the LED turns on. Let go and it turns off.

**Result:** Works. I then extended it so all four buttons drive all four LEDs, which is the version in `main.c`.

## Registers I used

| Register / field | Value | Why |
| --- | --- | --- |
| `IN` (0x510) | read only | Reads all 32 pins at once, like `PINx` on AVR |
| `PIN_CNF[n]` DIR, bit 0 | `Input` | Point the pin inward |
| `PIN_CNF[n]` INPUT, bit 1 | `Connect` (0) | Lets `IN` actually see the pin. It resets to Disconnect |
| `PIN_CNF[n]` PULL, bits 2 to 3 | `Pullup` (3) | The DK buttons have no resistors of their own |

PULL is two bits wide because its biggest value is 3 (`0b11`). Its mask is `0x3 << 2`, which is `0xC`.

## The core pattern

```c
NRF_P0->PIN_CNF[BUTTON1] = (GPIO_PIN_CNF_DIR_Input     << GPIO_PIN_CNF_DIR_Pos)
                         | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                         | (GPIO_PIN_CNF_PULL_Pullup   << GPIO_PIN_CNF_PULL_Pos);

if (!(NRF_P0->IN & GPIO_IN_PIN11_Msk))   // pressed reads 0 (active-low)
```

## What I learned

- **`&` reads, `|` writes.** `IN & mask` keeps only the bit I care about. `IN | mask` is never zero, so an `if` built on it is always true.
- **Setup goes before the loop. Reading and acting go inside it.**
- **Inputs need a pull.** Without one the pin floats and reads whatever noise pushes it toward.
- **The INPUT field is backwards from what you'd guess.** 0 means Connect. It also has nothing to do with direction.

## Predictions vs measurements

| Test | I predicted | I measured |
| --- | --- | --- |
| P0.11 released | HIGH | 3 V |
| P0.11 pressed | LOW | 0 V |

## Drills

| Change | What happened | Why |
| --- | --- | --- |
| Removed the pull-up | LED came on and stayed on after release | Nothing pulls the pin back up to 3 V once the button lets go |
| INPUT set to Disconnect | LED stuck on | `IN` can't see the pin and reads 0, which looks like a press |
| Moved the `if` above `while (1)` | Nothing after boot. Holding the button through reset latched the LED on | The check ran once at startup and never again |

## Bugs I hit

- Compared the whole port with `!=` instead of masking out bit 11.
- Used `|` to read a pin.
- Put the LED's `PIN_CNF` setup inside the `if` instead of driving the pin with `OUTCLR` / `OUTSET`.
