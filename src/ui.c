/**
 * ui.c — THE INPUT LAYER.  ****  YOU WRITE THE BOTTOM HALF  ****
 *
 * Every action here does the same three things and nothing else:
 *
 *      ask the user  ->  validate  ->  write ONE field of the model
 *
 * None of them decide anything. Deciding what a lamp should do is the job of
 * applyRules() in house.c. Keeping those apart is what makes either of them
 * testable — and it is worth marks.
 *
 *  THE FIVE FUNCTIONS ARE IN THE ORDER YOU SHOULD WRITE THEM.
 *  Work straight down the file. Each one is marked [ N / 5 ].
 *
 *      [ 1 / 5 ]  setOccupancy()     FR-08   easiest — start here
 *      [ 2 / 5 ]  setTemperature()   FR-09   ask, range-check, store
 *      [ 3 / 5 ]  switchDevice()     FR-07   a switch with 3 cases
 *      [ 4 / 5 ]  houseReport()      FR-11   counters and bars
 *      [ 5 / 5 ]  runAutomation()    FR-10   the trace — hardest, do it last
 *
 * The top half is GIVEN: the menu, the input helpers, and pickRoom(), which
 * is your validation template. Copy its shape.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Student: <YOUR NAME HERE>
 */
#include <stdio.h>

#include "ui.h"
#include "house.h"
#include "render.h"
#include "platform.h"

/* ============================================================================
 *                            GIVEN — read these
 * ========================================================================== */

void printMenu(void)
{
    /* two fixed-width columns, so the numbers line up under each other */
    printf("\n  %s1%s) %-26s%s2%s) %s\n", CC(C_SEL), CC(C_RESET),
           "Show the house", CC(C_SEL), CC(C_RESET), "Switch lamp/fan/auto");
    printf("  %s3%s) %-26s%s4%s) %s\n", CC(C_SEL), CC(C_RESET),
           "Someone enters/leaves", CC(C_SEL), CC(C_RESET), "Set room temperature");
    printf("  %s5%s) %-26s%s6%s) %s\n", CC(C_SEL), CC(C_RESET),
           "Run automation", CC(C_SEL), CC(C_RESET), "House report");
    printf("  %s7%s) %-26s%s0%s) %s\n", CC(C_SEL), CC(C_RESET),
           "Auto demo (scripted)", CC(C_SEL), CC(C_RESET), "Exit");
    printf("\n  Select > ");
    fflush(stdout);
}

/* Read one whole line, then parse an int out of it. Returns 0 on EOF or on
 * junk like "hello". Reading a LINE at a time means the buffer is never left
 * dirty, so the "press Enter" pause below actually waits. */
int readInt(int *out)
{
    char buf[64];
    if (fgets(buf, (int)sizeof buf, stdin) == NULL) { return 0; }
    return sscanf(buf, "%d", out) == 1;
}

void pauseKey(void)
{
    char buf[64];
    if (g_plain) { return; }
    printf("\n%s  -- press Enter --%s", CC(C_DIM), CC(C_RESET));
    fflush(stdout);
    if (fgets(buf, (int)sizeof buf, stdin) == NULL) { /* EOF: carry on */ }
}

void printBinary(uint8_t value)
{
    printf("0b");
    for (int8_t bit = 7; bit >= 0; bit--) {
        putchar(READ_BIT(value, bit) ? '1' : '0');
    }
}

/* THE VALIDATION TEMPLATE. Ask for a room index, reject anything that is not
 * 0..ROOM_COUNT-1, and return 255 to mean "bad input, do nothing".
 * Every one of your functions below starts with this same shape. */
uint8_t pickRoom(void)
{
    int n = -1;

    printf("  Room (");
    for (uint8_t i = 0U; i < ROOM_COUNT; i++) {
        printf("%s%u%s=%s ", CC(C_SEL), i, CC(C_RESET), houseRoom(i)->name);
    }
    printf("): ");

    if (!readInt(&n) || n < 0 || n >= (int)ROOM_COUNT) {
        statusSet(C_ALARM, "No such room.");
        return 255U;
    }
    return (uint8_t)n;
}

/* ============================================================================
 *                              YOUR WORK
 *
 * Useful calls you already have:
 *   pickRoom()                    -> room index, or 255 on bad input
 *   houseRoom(i)                  -> Room_t * for room i
 *   tempC(r->adc)                 -> that room's temperature in C
 *   statusSet(COLOUR, "fmt", ...) -> the message line under the schematic
 *                                    (works exactly like printf)
 *   render(i)                     -> redraw, with room i's border glowing;
 *                                    pass -1 to highlight nothing
 *   pauseKey()                    -> "press Enter", so output can be read
 *   printBinary(byte)             -> prints 0b00001011
 *   drawBar(v, full, width, col)  -> the bar renderer, from render.h
 *
 * Colours for statusSet: C_OK (green), C_ALARM (red), C_WARM (amber),
 * C_COOL (blue), C_LAMP (yellow), C_FAN (cyan), C_DIM (grey), C_AUTO, C_MAN.
 * ========================================================================== */


/* ==========================================================================
 *  [ 1 / 5 ]   YOUR WORK HERE  —  setOccupancy()                     FR-08
 * --------------------------------------------------------------------------
 *  REQUIRES : house.c [ 1 / 6 ] houseInit(), so the rooms have names.
 *  GIVES    : menu 3 works — "people: yes/no" flips on the schematic.
 *  USES     : pickRoom(), houseRoom(), TOGGLE_BIT, BIT_OCCUPIED,
 *             READ_BIT, statusSet(), render(), pauseKey()
 *  CHECK    : menu 3, pick Kitchen, its people line flips. The LAMP must
 *             NOT move — only the sensor bit changed.
 * ==========================================================================
 *
 * Somebody walks in or out. The shortest function in the file — do it first
 * to get the shape into your fingers.
 *
 *   1. i = pickRoom(); if it is 255, return immediately
 *   2. TOGGLE_BIT the OCCUPIED flag of that room
 *   3. statusSet() a message saying who is in the room now
 *   4. render((int)i) then pauseKey()
 *
 * NOTE: this changes the SENSOR bit and nothing else. The lamp does not move
 * until the rules run. That distinction is the whole point of the project —
 * do not "helpfully" turn the lamp on in here.
 */
void setOccupancy(void)
{
    printf("  TODO setOccupancy\n");
}


/* ==========================================================================
 *  [ 2 / 5 ]   YOUR WORK HERE  —  setTemperature()                   FR-09
 * --------------------------------------------------------------------------
 *  REQUIRES : house.c [ 1 / 6 ] houseInit() and [ 2 / 6 ] tempC().
 *  GIVES    : menu 4 works — you can heat a room up and watch its bar grow.
 *             This is how you will test your rules later.
 *  USES     : pickRoom(), readInt(), ADC_MAX, tempC(), statusSet(),
 *             render(), pauseKey()
 *  CHECK    : 1023 is accepted (499 C, bar full). 2000 and "abc" are both
 *             rejected AND the room's old temperature is unchanged.
 * ==========================================================================
 *
 * Write a new raw ADC count into one room.
 *
 *   1. i = pickRoom()
 *   2. ask "  Raw ADC reading (0..1023): " and read an int with readInt()
 *   3. reject anything outside 0..ADC_MAX with a statusSet() message and
 *      NO write to the model, then return
 *   4. otherwise store it and statusSet("%s: ADC %u -> %u C", ...)
 *   5. render((int)i), pauseKey()
 *
 * Read into an `int` and validate BEFORE casting to uint16_t. If you read
 * straight into a uint16_t you can never detect a negative number — it has
 * already wrapped to 65532 and sailed through your range check.
 */
void setTemperature(void)
{
    printf("  TODO setTemperature\n");
}


/* ==========================================================================
 *  [ 3 / 5 ]   YOUR WORK HERE  —  switchDevice()                     FR-07
 * --------------------------------------------------------------------------
 *  REQUIRES : house.c [ 1 / 6 ] houseInit().
 *  GIVES    : menu 2 works — lamps and fans flip by hand, and the room drops
 *             to MAN. Once your rules exist, MANUAL rooms get skipped.
 *  USES     : pickRoom(), readInt(), TOGGLE_BIT, CLR_BIT, READ_BIT,
 *             BIT_LAMP, BIT_FAN, BIT_AUTO, statusSet(), render(),
 *             printBinary(), pauseKey()
 *  CHECK    : flip the Living lamp — the card shows [#] and the tag turns to
 *             MAN. The printed status byte must be 0b00000101 (LAMP + people,
 *             AUTO gone).
 * ==========================================================================
 *
 * Switch a lamp, a fan, or auto-mode.
 *
 *   1. i = pickRoom(); if 255, return
 *   2. ask "  Switch (1=Lamp 2=Fan 3=Auto mode): " and read an int
 *   3. switch on the answer:
 *        1 -> TOGGLE_BIT the LAMP, then CLR_BIT the AUTO bit
 *        2 -> TOGGLE_BIT the FAN,  then CLR_BIT the AUTO bit
 *        3 -> TOGGLE_BIT the AUTO bit
 *        anything else -> statusSet "Nothing switched." and return
 *   4. statusSet() a message saying what happened
 *   5. render((int)i), then print "  <name> status = " + printBinary(status)
 *      + "  (0x%02X)", then pauseKey()
 *
 * WHY LAMP AND FAN CLEAR AUTO: a human just touched the switch by hand, so
 * the room drops out of automation until somebody hands control back. That
 * is exactly how a real thermostat behaves — manual override wins.
 */
void switchDevice(void)
{
    printf("  TODO switchDevice\n");
}


/* ==========================================================================
 *  [ 4 / 5 ]   YOUR WORK HERE  —  houseReport()                      FR-11
 * --------------------------------------------------------------------------
 *  REQUIRES : house.c [ 5 / 6 ] countRoomsWith(), [ 6 / 6 ] sumAdc(),
 *             and [ 2 / 6 ] tempC().
 *  GIVES    : menu 6 works — the summary with its four bars.
 *  USES     : render(), countRoomsWith(), drawBar(), REPORT_BAR_W,
 *             sumAdc(), houseRooms(), tempC(), pauseKey()
 *  CHECK    : the counters must match what you can count on the schematic.
 *             With the seed house the raw sum is 363 and the average 29 C.
 * ==========================================================================
 *
 * The house report:
 *
 *   render(-1) first, so the schematic sits above your report
 *
 *   four counter lines, each  countRoomsWith(BIT_x)  followed by
 *       drawBar(count, ROOM_COUNT, REPORT_BAR_W, COLOUR)
 *     -> Lamps ON, Fans ON, Occupied, Alarms
 *
 *   hottest and coldest room BY NAME (one loop comparing .adc)
 *   average temperature: tempC(sumAdc(houseRooms(), ROOM_COUNT) / ROOM_COUNT)
 *
 *   then pauseKey()
 *
 * Use the SAME drawBar() that draws the temperature gauge in each room card.
 * One function, two scales. Four copy-pasted loops score less.
 */
void houseReport(void)
{
    printf("  TODO houseReport\n");
}


/* ==========================================================================
 *  [ 5 / 5 ]   YOUR WORK HERE  —  runAutomation()                    FR-10
 *              *** hardest one — leave it until last ***
 * --------------------------------------------------------------------------
 *  REQUIRES : house.c [ 3 / 6 ] applyRules() and [ 4 / 6 ] rulesPass().
 *             Nothing here works until the rules do.
 *  GIVES    : menu 5 works — the before -> after trace, the thing that proves
 *             your rules are right.
 *  USES     : houseRoom(), tempC(), READ_BIT, BIT_AUTO, applyRules(),
 *             snprintf(), render(), statusSet(), pauseKey()
 *  CHECK    : press 5 twice. You MUST see 5 changed, then 0 changed.
 * ==========================================================================
 *
 * Run the rules over the house and show what moved.
 *
 * The deciding is already done for you — call applyRules() from house.c.
 * What belongs HERE is only the reporting:
 *
 *   for each room i:
 *       remember the status byte BEFORE
 *       if the room is not AUTO -> note "<name>  <t> C   skipped (MANUAL)"
 *       else -> call applyRules(), add its return to a `changed` counter,
 *               and note "<name>  <t> C   0b<before> -> 0b<after>  *"
 *                (the * only when it actually changed)
 *   print every note, then "N room(s) changed.", then pauseKey()
 *
 * Build the notes into a `char trace[ROOM_COUNT][96];` with snprintf() so you
 * can print them all together at the end.
 *
 * SELF-CHECK — this is the one people fail. Run it twice. If a second pass
 * keeps reporting changes, a rule is fighting itself: go back to house.c
 * [ 3 / 6 ] and find the `if` that has no `else`.
 */
void runAutomation(void)
{
    printf("  TODO runAutomation\n");
}
