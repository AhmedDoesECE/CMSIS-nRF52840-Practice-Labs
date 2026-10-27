# Lab 3: TIMER and Clock Source

**Goal:** Use TIMER0 to toggle LED1 every 500 ms with no delay loops, for a 1 Hz square wave on P0.13.

**Result:** 996 ms period on the default internal clock. Exactly 1.000 s once I started the external crystal.

![1 Hz square wave on P0.13 with the crystal running](scope-1hz-hfxo.png)

## Tasks, events and shortcuts

This is how every nRF peripheral works, including the radio.

| Thing | What it is | How I use it | AVR equivalent |
| --- | --- | --- | --- |
| Task | Starts an action | Write `1` to it | Setting the CS1x bits to start Timer1 |
| Event | A flag the hardware raises | Wait for `1`, then clear it by writing `0` | `OCF1A`, which AVR clears by writing `1` |
| Shortcut | Hardware link from an event to a task | Turn on a bit in `SHORTS` | CTC mode |

```c
PERIPH->EVENTS_X = 0;              // lower the flag
PERIPH->TASKS_Y  = 1;              // start
while (PERIPH->EVENTS_X == 0) { }  // wait for the flag
```

## Registers I used

| Register | Value | Why |
| --- | --- | --- |
| `TIMER0->MODE` | `Timer` | Count clock ticks, not external pulses |
| `TIMER0->BITMODE` | `24Bit` | It resets to 16-bit, which tops out at 65,535 |
| `TIMER0->PRESCALER` | 4 | 16 MHz / 2^4 = 1 MHz, so one tick is 1 µs |
| `TIMER0->CC[0]` | 500000 | 500,000 µs = 500 ms. Works like `OCR1A` |
| `TIMER0->SHORTS` | `COMPARE0_CLEAR` enabled | Reset the count at the target, like CTC mode |
| `TIMER0->TASKS_START` | 1 | Go |
| `TIMER0->EVENTS_COMPARE[0]` | poll, then write 0 | Count reached `CC[0]` |
| `CLOCK->TASKS_HFCLKSTART` | 1 | Start the external crystal |

## The clock result

| Clock | Spec tolerance | Measured period |
| --- | --- | --- |
| HFINT (internal RC, the default at boot) | ±1.5% typical | 996 ms |
| HFXO (external 32 MHz crystal) | ±60 ppm | 1.000 s |

My code was right both times. The internal clock was the problem. The 0.4% error I measured is well inside HFINT's spec, but it's far too much for timing measurements or the radio. At 2,407 MHz, a 1.5% clock error moves the radio about 36 MHz, which is roughly 36 channels away from the receiver. So the crystal start goes at the very top of `main()` in every program from now on.

## Predictions vs measurements

| Test | I predicted | I measured |
| --- | --- | --- |
| Period with the timer running | 500 ms / 2 Hz | 996 ms / about 1 Hz |
| Period with the crystal started | 1,000 ms ± 0.06 ms | 1.000 s / 1.000 Hz |

I got the first one wrong because I mixed up the toggle time with the period. One period is a full ON plus OFF, so two toggles: 2 × 500 ms = 1 s.

## Drills

| Change | What happened | Why |
| --- | --- | --- |
| Started the crystal | 996 ms became 1.000 s | See above |
| Removed the event clear | LED looked like it stayed on | The flag stays at 1, so the wait loop never waits and the LED toggles as fast as the CPU loops |
| Removed `SHORTS` | First toggle at 0.5 s, then about 16.7 s between toggles | The count runs all the way to 2^24 µs (16.78 s) and wraps around before it hits 500,000 again |

## Bugs I hit

- Reached for `TASKS_COUNT` (counter mode) when the target belongs in `CC[0]`.
- Mixed up indices between `CC[0]`, the `SHORTS` field and the event I was polling.
- Started the clock after the timer. The clock goes first.
- Forgot that the timer resets to 16-bit.

## Scope notes (Rigol DS1054Z)

- The big AUTO key is auto-scale, not trigger-mode Auto. On a slow signal it makes a mess of the settings.
- "WAIT" means the trigger hasn't seen an edge yet.
- 200 ms/div shows a full cycle of a 1 Hz signal.
- A bad probe contact made a clean signal look broken. A hook tip on a jumper wire fixed it.
