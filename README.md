# Smart Home Console — Project Lab

**Your task: write the Model and the Input layer of a smart house.**

The house draws itself already. Six rooms, a lamp and a fan in each, a
temperature sensor and a motion sensor. What is missing is everything that
*knows* anything: the data, the rules that decide what should be on, and the
menu actions that let a person change it.
---

## Fill this in before you start

| Field | Your answer |
|-------|-------------|
| **Full name** | |
| **Email** | |
| **Group** | G9 |
| **Date started** | 02-09-2026|
| **Date submitted** | 03-09-2026|

Put your name at the top of `src/house.c` and `src/ui.c` too — both files have
a `Student: <YOUR NAME HERE>` line waiting for it.

---

## 1. Build and run it right now

```bat
build.bat
house.exe
```

```bash
make && ./house
```

It compiles **with zero warnings** and runs before you write a single line.
You will see the frame of the house with six empty rooms:

```
+------------------+------------------+------------------+
|             MAN  |             MAN  |             MAN  |
|  Lamp [ ] Fan ( )|  Lamp [ ] Fan ( )|  Lamp [ ] Fan ( )|
|    0 C --------  |    0 C --------  |    0 C --------  |
|  people: no      |  people: no      |  people: no      |
|                  |                  |                  |
+------------------+------------------+------------------+
```

That is your starting point. Every function you finish fills more of it in.
**If this does not build, fix your toolchain before writing any code.**

---

## 2. What you are given, what you write

```
projectLab/
├── main.c              GIVEN   the menu loop
├── include/            GIVEN   THE CONTRACT — do not change these
│   ├── house.h                 the model's interface
│   ├── render.h                the view's interface
│   ├── ui.h                    your functions' prototypes
│   ├── platform.h              colour, clearing, timing
│   └── demo.h
├── src/
│   ├── render.c        GIVEN   the view — draws everything
│   ├── platform.c      GIVEN   colour and screen handling
│   ├── demo.c          GIVEN   the scripted story (menu 7)
│   ├── house.c    ***  YOU WRITE THIS — the model + the rules
│   └── ui.c       ***  YOU WRITE THE BOTTOM HALF — the menu actions
├── build.bat / Makefile
└── README.md           this file
```

### The headers are a contract

`include/` holds the prototypes. **Do not change them** — not the names, not
the parameters, not the return types. Your code is graded by dropping it into
the reference project, and it has to fit.

Every function you write is already declared there, and every function you
need is already declared there too. If you find yourself wanting a function
that is not in a header, you are probably solving it in the wrong layer.

### What each layer is allowed to do

| Layer | File | Rule |
|-------|------|------|
| **Model** | `house.c` | Owns the data. Applies the rules. **Never prints. Never asks the user.** |
| **View** | `render.c` — given | Reads the house and prints it. **Never writes a bit.** |
| **Input** | `ui.c` | Asks, validates, writes **one** field. **Never decides.** |

That separation is the actual engineering lesson, and it is marked. If your
`houseReport()` changes a lamp, or your `applyRules()` calls `printf`, you
have broken it.

---

## 3. What you have to implement — 11 functions, in order

**The functions sit in the files in the exact order you should write them.**
Work straight down each file. Above every one is a banner telling you where
you are and what it needs:

```c
/* ==========================================================================
 *  [ 3 / 6 ]   YOUR WORK HERE  —  applyRules()                       FR-10
 *              *** THE BIG ONE — this is the heart of the project ***
 * --------------------------------------------------------------------------
 *  REQUIRES : [ 2 / 6 ] tempC() must work — the rules compare temperatures.
 *  GIVES    : nothing visible on its own. [ 4 / 6 ] is what calls it.
 *  USES     : READ_BIT, SET_BIT, CLR_BIT, tempC(), BIT_AUTO, ...
 *  CHECK    : do it together with [ 4 / 6 ], then press 5 twice.
 * ==========================================================================
```

- **REQUIRES** — what must already work before this one can. Respect it and
  you never get stuck.
- **GIVES** — what appears on screen when you finish. Your reward.
- **USES** — the macros, constants and helpers you need. No hunting.
- **CHECK** — how to prove it works before moving on.

### `src/house.c` — THE MODEL · 6 functions

| # | Function | FR | Requires | You get |
|---|----------|----|----------|---------|
| **1/6** | `houseInit()` | FR-03 | nothing — **start here** | The six rooms appear with real names |
| **2/6** | `tempC()` | FR-05 | nothing | Degrees and temperature bars work |
| **3/6** | `applyRules()` | FR-10 | `[2/6] tempC()` | *(the big one — the three rules)* |
| **4/6** | `rulesPass()` | FR-10 | `[3/6] applyRules()` | **Menu 7 comes alive** — your free test rig |
| **5/6** | `countRoomsWith()` | FR-11 | nothing | Feeds the report counters |
| **6/6** | `sumAdc()` | FR-11 | nothing — **recursive** | Feeds the report average |

Given to you: the `static Room_t house[6]` array and the accessors
`houseRoom()` / `houseRooms()`.

### `src/ui.c` — THE INPUT LAYER · 5 functions

| # | Function | FR | Requires | You get |
|---|----------|----|----------|---------|
| **1/5** | `setOccupancy()` | FR-08 | `house [1/6]` | Menu 3 — people flip on the schematic |
| **2/5** | `setTemperature()` | FR-09 | `house [1/6] [2/6]` | Menu 4 — heat a room and watch its bar grow |
| **3/5** | `switchDevice()` | FR-07 | `house [1/6]` | Menu 2 — flip lamps by hand, room drops to `MAN` |
| **4/5** | `houseReport()` | FR-11 | `house [2/6] [5/6] [6/6]` | Menu 6 — counters and bars |
| **5/5** | `runAutomation()` | FR-10 | `house [3/6] [4/6]` | Menu 5 — the before → after trace |

Given to you: `printMenu()`, `readInt()`, `pauseKey()`, `printBinary()`, and
**`pickRoom()` — your validation template. Copy its shape.**

### The dependency chain, at a glance

```
  house [1/6] houseInit ─┬─► ui [1/5] setOccupancy
                         ├─► ui [3/5] switchDevice
  house [2/6] tempC ─────┼─► ui [2/5] setTemperature
          │              │
          ▼              │
  house [3/6] applyRules │
          │              │
          ▼              │
  house [4/6] rulesPass ─┴─► ui [5/5] runAutomation   + menu 7 works

  house [5/6] countRoomsWith ─┬─► ui [4/5] houseReport
  house [6/6] sumAdc ─────────┘
```

Nothing depends on `render.c` — that is given and already correct.

**The comment block above each function is the specification. Read it.**

---

## 4. The model you are building

### One room

```c
typedef struct {
    char     name[NAME_LEN];   /* "Living", "Kitchen", ...          */
    uint8_t  status;           /* packed flags — bit macros only    */
    uint16_t adc;              /* raw 10-bit sensor count, 0..1023  */
} Room_t;

static Room_t house[ROOM_COUNT];   /* six rooms, fixed, no malloc */
```

Rooms 0–2 are the top row of the schematic, 3–5 the bottom row.

| index | Name | Seed ADC | → °C | Occupied |
|-------|------|----------|------|----------|
| 0 | Living | 51 | 24 | yes |
| 1 | Kitchen | 64 | 31 | no |
| 2 | Bedroom | 45 | 21 | no |
| 3 | Bathroom | 58 | 28 | no |
| 4 | Hall | 49 | 23 | yes |
| 5 | Garage | 96 | 46 | no |

### The status register — one byte per room

| Bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|---|---|---|---|---|---|---|---|
| Meaning | — | — | — | `ALARM` | `AUTO` | `OCCUPIED` | `FAN` | `LAMP` |

Touch it **only** through these four macros:

```c
SET_BIT(r->status, BIT_LAMP)      /* reg |=  (1U << n) */
CLR_BIT(r->status, BIT_AUTO)      /* reg &= ~(1U << n) */
TOGGLE_BIT(r->status, BIT_FAN)    /* reg ^=  (1U << n) */
READ_BIT(r->status, BIT_OCCUPIED) /* (reg >> n) & 1U   */
```

Never write `r->status = 3;` to mean "lamp and fan on". It also wipes
`OCCUPIED`, `AUTO` and `ALARM`, and the room silently drops out of
automation. On Day 05 the same mistake on `PORTA` turns off half your LEDs
and you will spend an hour blaming the wiring.

### Temperature — integer maths, no float

```
temperature (°C) = adc * 500 / 1024
```

An LM35 on a 5 V reference through a 10-bit ADC. Cast to `uint32_t` **before**
the multiply, and multiply **before** you divide. Check on paper: ADC 51 → 24,
ADC 96 → 46.

---

## 5. The three rules — the heart of it

For every room whose `AUTO` bit is set, **in this order**:

| # | Rule | Condition | Action |
|---|------|-----------|--------|
| **R1** | Light follows people | `OCCUPIED` set | `LAMP` on — **else `LAMP` off** |
| **R2** | Fan follows heat | `tempC >= 28` | `FAN` on — **else `FAN` off** |
| **R3** | Overheat overrides | `tempC >= 45` | `ALARM` on **and** `LAMP` on, whatever R1 decided — else `ALARM` off |

A room with `AUTO` clear is **skipped entirely**.

**R3 runs last and overwrites R1 on purpose.** An overheating empty room still
lights up. In an `if/else` chain the last write wins, so the order you put
your rules in is a design decision, not a formatting choice.

> **Your README must answer this:** what happens to the Garage lamp if you
> move R3 *before* R1, and why? Try it, look at the screen, then put it back.

---

## 6. Order of work

The functions are already in this order inside the two files. Work straight
down `house.c`, then straight down `ui.c`, checking the screen after each one.

| Step | File | Write | You should see |
|------|------|-------|----------------|
| 1 | `house.c` | **[1/6]** `houseInit()` | The six rooms appear with real names |
| 2 | `house.c` | **[2/6]** `tempC()` | Degrees and temperature bars work |
| 3 | `house.c` | **[3/6]** `applyRules()` | *(nothing yet — step 4 calls it)* |
| 4 | `house.c` | **[4/6]** `rulesPass()` | **Menu 7 runs the whole story** |
| 5 | `house.c` | **[5/6]** `countRoomsWith()` | *(feeds the report)* |
| 6 | `house.c` | **[6/6]** `sumAdc()` | *(feeds the report)* |
| 7 | `ui.c` | **[1/5]** `setOccupancy()` | Menu 3 flips `people: yes/no` |
| 8 | `ui.c` | **[2/5]** `setTemperature()` | Menu 4 changes a temperature |
| 9 | `ui.c` | **[3/5]** `switchDevice()` | Menu 2 flips lamps, room goes `MAN` |
| 10 | `ui.c` | **[4/5]** `houseReport()` | Menu 6 |
| 11 | `ui.c` | **[5/5]** `runAutomation()` | Menu 5 — the before → after trace |

**Steps 1 and 2 are your first win** — two small functions and the whole
schematic fills in with names, temperatures and bars. Do them before anything
else; everything after is easier when you can see the house.

**Step 4 gets you a free test rig.** The moment `rulesPass()` works, menu
option 7 runs a scripted evening by itself — lights following people, the
Kitchen fan starting, the Garage alarming — and shows you whether your rules
behave. You do not write any of it; it is given.

If you prefer to see the menu working sooner, steps 7–9 can be done straight
after step 2 — they only need `houseInit()` and `tempC()`. Steps 10 and 11 are
the only ones with hard prerequisites (see the chain in §3).

---

## 7. Test it the way it will be graded

| # | Do this | Must happen |
|---|---------|-------------|
| 1 | Start it | Six rooms, all `AUTO`, all lamps off |
| 2 | `5` | 5 rooms change; Garage shows `!! OVERHEAT !!` |
| 3 | `5` **again** | **`0 room(s) changed`** |
| 4 | `2` → `0` → `1` | Living lamp flips, room shows `MAN` |
| 5 | `5` | `Living  skipped (MANUAL)`, its lamp stays put |
| 6 | `2` → `0` → `3`, then `5` | Living is back under the rules |
| 7 | `4` → `2` → `1023` | Accepted, 499 °C, bar full, not overrun |
| 8 | `4` → `2` → `2000` | Rejected, and the temperature did **not** move |
| 9 | `4` → `2` → `abc` | Rejected, no crash, no endless menu spin |
| 10 | `2` → room `9` | `No such room.` |
| 11 | Type `hello` at the menu | `Invalid choice, try again.` once |
| 12 | `6` | Counters match what the schematic shows |

**Test 3 is the one people fail.** If a second pass keeps reporting changes,
a rule is fighting itself — almost always an `if` with no `else`, so the bit
gets set but never cleared.

Quick check from the shell:

```bash
NO_COLOR=1 ./house <<< $'5\n5\n0' | grep -E "^  [0-9]+ room\(s\) changed\.$"
#   5 room(s) changed.
#   0 room(s) changed.      <- correct
```

---

## 8. Mistakes that cost marks

- **Writing `status` directly.** `r->status = 5;` destroys the other flags.
  Macros only, every time.
- **`if` with no `else` in the rules.** R1 must turn the lamp *off* for an
  empty room, not merely fail to turn it on. Same for R2.
- **Deciding inside `ui.c`.** Your menu actions write one field. They do not
  work out what a lamp should do — `applyRules()` does.
- **Printing inside `house.c`.** The model never prints. Not even for debugging
  — use the trace in `runAutomation()`.
- **`float` anywhere.** The AVR has no FPU. Integer maths, `uint16_t` result.
- **A `for` loop inside `sumAdc()`.** It must be recursive. A loop scores zero
  for that requirement.
- **Changing a header.** Your code has to drop into the reference project.
- **Ignoring the compiler.** `-Wall -Wextra`, zero warnings, or you lose NFR
  marks. It starts clean — keep it clean.

---

## 9. What to hand in

| Item | Details |
|------|---------|
| `src/house.c` | Your model and rules, with your name at the top |
| `src/ui.c` | Your input layer, with your name at the top |
| `README.md` | **This file** — your details filled in at the top, the checklist in §11 ticked, and your R3 answer written in §12 |
| Screenshots | 3: the house after startup, an automation pass showing `!! OVERHEAT !!`, and the house report |

Hand in the whole `projectLab` folder. It must build with
`gcc -std=c99 -Wall -Wextra` and **zero warnings**.

You do not write a separate report — this README *is* the report. Fill in the
table at the top, tick §11 as you go, and answer §12 at the end.

**Individual work.** Discuss ideas freely; never share code. Your R3
explanation is in your own words — identical explanations are treated as
identical code.

---

## 10. If you get stuck

- The comment block above each empty function is the specification. Re-read it.
- `pickRoom()` in `ui.c` is a complete, working example of ask → validate →
  act. Every one of your functions has that same shape.
- `render.c` is given and correct. If the screen looks wrong, the bug is in
  your model, not in the drawing.
- The full brief is in
  [`../../Assignment/Project_Overview.md`](../../Assignment/Project_Overview.md),
  and there is a clickable version of this house on the **Day 03 interactive
  page → Smart Home Console**. Click a room, flip a lamp, press *Run
  automation*, and compare it with what your program does.

---

## 11. Progress checklist

Tick a box by putting an **`x`** between the brackets: `[ ]` becomes `[x]`.
Do not tick one until the **Proof it works** column actually happens on
your screen — that column is how it will be marked.

### `src/house.c` — the model · 6 functions

| Done | # | Function | FR | Requires | Proof it works |
|:----:|---|----------|----|----------|----------------|
| [ ] | **1/6** | `houseInit()` | FR-03 | — | Cards show Living / Kitchen / Bedroom / Bathroom / Hall / Garage, all `AUTO`; Living and Hall say `people: yes` |
| [ ] | **2/6** | `tempC()` | FR-05 | — | ADC 51 → 24 C, ADC 96 → 46 C, ADC 1023 → 499 C; the bars fill in |
| [ ] | **3/6** | `applyRules()` | FR-10 | 2/6 | *(prove it with 4/6)* |
| [ ] | **4/6** | `rulesPass()` | FR-10 | 3/6 | Menu **7** runs the whole evening: lamps follow people, Kitchen fan starts, Garage alarms |
| [ ] | **5/6** | `countRoomsWith()` | FR-11 | — | Report counters match what you can count on the schematic |
| [ ] | **6/6** | `sumAdc()` — **recursive** | FR-11 | — | Report shows raw sum **363**, average **29 C** |

### `src/ui.c` — the input layer · 5 functions

| Done | # | Function | FR | Requires | Proof it works |
|:----:|---|----------|----|----------|----------------|
| [ ] | **1/5** | `setOccupancy()` | FR-08 | house 1/6 | Menu **3** flips `people: yes/no` — and the lamp does **not** move |
| [ ] | **2/5** | `setTemperature()` | FR-09 | house 1/6, 2/6 | Menu **4**: `1023` accepted; `2000` and `abc` rejected with the old value intact |
| [ ] | **3/5** | `switchDevice()` | FR-07 | house 1/6 | Menu **2**: Living lamp flips, tag turns `MAN`, status prints `0b00000101` |
| [ ] | **4/5** | `houseReport()` | FR-11 | house 2/6, 5/6, 6/6 | Menu **6** shows four bars, hottest `Garage`, coldest `Bedroom` |
| [ ] | **5/5** | `runAutomation()` | FR-10 | house 3/6, 4/6 | Menu **5** twice → **5 changed, then 0 changed** |

### Acceptance tests (§7)

| Done | # | Check |
|:----:|---|-------|
| [ ] | 1 | Startup: six rooms, all `AUTO`, all lamps off |
| [ ] | 2 | `5` → 5 rooms change, Garage shows `!! OVERHEAT !!` |
| [ ] | 3 | `5` again → **`0 room(s) changed`** ← the one people fail |
| [ ] | 4 | `2` → `0` → `1` → Living lamp flips, room shows `MAN` |
| [ ] | 5 | `5` → `Living  skipped (MANUAL)`, its lamp stays put |
| [ ] | 6 | `2` → `0` → `3`, then `5` → Living is back under the rules |
| [ ] | 7 | `4` → `2` → `1023` → accepted, 499 C, bar full, not overrun |
| [ ] | 8 | `4` → `2` → `2000` → rejected, temperature unchanged |
| [ ] | 9 | `4` → `2` → `abc` → rejected, no crash, no menu spin |
| [ ] | 10 | `2` → room `9` → `No such room.` |
| [ ] | 11 | `hello` at the menu → `Invalid choice, try again.` once |
| [ ] | 12 | `6` → counters match the schematic |

### Before you hand it in

| Done | Item |
|:----:|------|
| [ ] | Builds with `gcc -std=c99 -Wall -Wextra` and **zero warnings** |
| [ ] | My name is in the table at the top of this file |
| [ ] | My name is in `src/house.c` and `src/ui.c` |
| [ ] | No `float`, no `double`, no `malloc` anywhere |
| [ ] | `status` is only ever touched through the four bit macros |
| [ ] | `sumAdc()` is recursive — no loop inside it |
| [ ] | I did not change anything in `include/` |
| [ ] | **My answer to the R3 question** (§5) is written below |
| [ ] | Screenshot 1 — the house after startup |
| [ ] | Screenshot 2 — an automation pass showing `!! OVERHEAT !!` |
| [ ] | Screenshot 3 — the house report |

---

## 12. My answer to the R3 question

> **The question (§5):** what happens to the Garage lamp if you move R3
> *before* R1, and why?
>
> Try it, look at the screen, then put it back.

*(write your answer here — a few sentences, in your own words)*

---

*A house is an array. A room is a struct. A light switch is one bit.
The drawing is done — now make it think.*
