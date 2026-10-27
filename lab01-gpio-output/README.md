# Lab 1: GPIO Output

**Goal:** Turn on the four board LEDs using only register writes.

**Result:** All four LEDs on with two register writes. A pin driven HIGH measures 3 V, not the 5 V I expected from the Mega.

## Registers I used

From the nRF52840 Product Specification, GPIO chapter. Port P0 starts at `0x50000000`.

| Register | Offset | AVR equivalent | What it does |
| --- | --- | --- | --- |
| `DIRSET` | 0x518 | `DDRx \|= mask` | A 1 makes that pin an output. A 0 leaves it alone |
| `OUTSET` | 0x508 | `PORTx \|= mask` | A 1 drives that pin HIGH. A 0 leaves it alone |
| `OUTCLR` | 0x50C | `PORTx &= ~mask` | A 1 drives that pin LOW. A 0 leaves it alone |
| `PIN_CNF[n]` | 0x700 + 4n | none | Per-pin config: direction, input buffer, pull, drive, sense |

The LEDs are active-low, so `OUTCLR` turns them **on**. That's a fact about the DK board, not the chip, so it's in the DK Hardware User Guide rather than the Product Specification.

## What I learned

- **Peripherals are structs.** `NRF_P0->OUTCLR` is just the register at `0x50000000 + 0x50C`.
- **SET and CLR registers fix a real AVR problem.** On AVR, `PORTK |= (1 << 0)` reads the port, changes it and writes it back. An interrupt in the middle can get its change wiped out. Writing to `OUTSET` is one write that only touches the bits set to 1, so there's nothing to race.
- **Macro names follow a pattern:** `GPIO_OUTCLR_PIN13_Msk` is PERIPHERAL, REGISTER, FIELD, then SUFFIX.
  - `_Pos` is the bit number where a field starts.
  - `_Msk` is the field's mask, already shifted into place.
  - Named values like `_Output` are **not** shifted. You write `(VALUE << FIELD_Pos)` yourself.
- **Masks combine with `|`.** One write can set four pins: `0x2000 | 0x4000 | 0x8000 | 0x10000 = 0x1E000`.
- **A HIGH pin sits at the chip's VDD.** The DK runs at 3 V. The absolute max on any pin is VDD + 0.3 V = 3.3 V, so a 5 V Mega output can damage the nRF. Use a divider or a level shifter.

## Predictions vs measurements

| Test | I predicted | I measured |
| --- | --- | --- |
| P0.14 driven HIGH | 5 V | 3 V |
| 8 writes vs 2 combined writes | Same behavior | Same behavior |
| `PIN_CNF[13]` with `<< 13` instead of `<< GPIO_PIN_CNF_DIR_Pos` | (worked out during review) | LED1 stays off |

## Drills

1. Combine all eight writes into one `DIRSET` and one `OUTCLR`.
2. Swap `OUTCLR` for `OUTSET` on LED2. LED2 goes off and the pin reads 3 V.
3. Make P0.13 an output through `PIN_CNF[13]` instead of `DIRSET`.

## Bugs I hit

- Shifted by the **pin number** (13) instead of the **field position** (`GPIO_PIN_CNF_DIR_Pos`, which is 0). Inside `PIN_CNF[13]`, the `[13]` already picks the pin.
- Used a raw binary literal (`0b...11110000...`) before switching to `_Msk` macros.
- Left an assignment unfinished, which chained into the next line.
