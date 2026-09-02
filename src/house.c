/**
 * house.c — THE MODEL.  ****  YOU WRITE THIS FILE  ****
 *
 * This is the layer that owns the data and applies the rules. Every function
 * below is declared in include/house.h — that header is the contract, and you
 * must not change it. Fill in the bodies marked TODO.
 *
 * It compiles and runs as it stands: you get an empty house and a working
 * menu. Each function you finish makes more of the schematic come alive.
 *
 *  THE SIX FUNCTIONS ARE IN THE ORDER YOU SHOULD WRITE THEM.
 *  Work straight down the file. Each one is marked [ N / 6 ].
 *
 *      [ 1 / 6 ]  houseInit()        FR-03   the rooms appear
 *      [ 2 / 6 ]  tempC()            FR-05   degrees and bars work
 *      [ 3 / 6 ]  applyRules()       FR-10   the three rules  <-- the big one
 *      [ 4 / 6 ]  rulesPass()        FR-10   sweep the whole house
 *      [ 5 / 6 ]  countRoomsWith()   FR-11   for the report
 *      [ 6 / 6 ]  sumAdc()           FR-11   recursive, for the report
 *
 * RULES OF THE HOUSE
 *   - Touch `status` ONLY through SET_BIT / CLR_BIT / TOGGLE_BIT / READ_BIT.
 *     Never `r->status = 5;` — that silently wipes the other four flags.
 *   - No float, no double. Integer maths only.
 *   - No malloc. The array below is the whole house.
 *   - Nothing in this file prints anything. Printing is render.c's job.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Student: <YOUR NAME HERE>
 */
#include "house.h"

/* ---------------- module-private data (NFR-03) ---------------
 * GIVEN. The array is static, so nothing outside this file can reach it.
 * Callers go through houseRoom() / houseRooms() below. Leave it that way —
 * main() must never index the house directly. */
static Room_t house[ROOM_COUNT];

/* GIVEN — the only door to the array. render.c and ui.c both use this. */
Room_t *houseRoom(uint8_t i)
{
    return &house[i];
}

/* GIVEN — the whole array, read-only, for sumAdc() and the report. */
const Room_t *houseRooms(void)
{
    return house;
}


/* ==========================================================================
 *  [ 1 / 6 ]   YOUR WORK HERE  —  houseInit()                        FR-03
 * --------------------------------------------------------------------------
 *  REQUIRES : nothing. Start here.
 *  GIVES    : the six rooms appear on the schematic with their real names,
 *             temperatures and people flags. Your first win.
 *  USES     : SET_BIT, BIT_AUTO, BIT_OCCUPIED, NAME_LEN, the table below
 *  CHECK    : run it — the cards say Living / Kitchen / Bedroom / Bathroom /
 *             Hall / Garage, all tagged AUTO, Living and Hall show people: yes
 * ==========================================================================
 *
 * Build the six rooms. Called once at startup, and again by the scripted
 * demo. The house never changes size — that is why there is no "add room".
 *
 * For every room i:
 *   - copy NAMES[i] into house[i].name   (stop at '\0', and never write past
 *     NAME_LEN - 1; leave room for the terminator)
 *   - house[i].adc    = SEED_ADC[i]
 *   - house[i].status = 0, then SET_BIT the AUTO flag
 *   - if SEED_OCC[i], SET_BIT the OCCUPIED flag
 */
void houseInit(void)
{
    /* GIVEN — the seed table. Do not change the numbers. */
    static const char *const NAMES[ROOM_COUNT] =
        { "Living", "Kitchen", "Bedroom", "Bathroom", "Hall", "Garage" };
    static const uint16_t SEED_ADC[ROOM_COUNT] = { 51U, 64U, 45U, 58U, 49U, 96U };
    static const uint8_t  SEED_OCC[ROOM_COUNT] = { 1U, 0U, 0U, 0U, 1U, 0U };

    /* TODO: the loop described above. */
    (void)NAMES; (void)SEED_ADC; (void)SEED_OCC;   /* delete these */
}


/* ==========================================================================
 *  [ 2 / 6 ]   YOUR WORK HERE  —  tempC()                            FR-05
 * --------------------------------------------------------------------------
 *  REQUIRES : nothing. Independent of [1/6] — but do that one first so you
 *             have real ADC values to look at.
 *  GIVES    : every room shows its temperature in C, and the 8-character
 *             temperature bars start filling in.
 *  USES     : uint32_t for the intermediate, uint16_t for the result
 *  CHECK    : ADC 51 -> 24 C, ADC 96 -> 46 C, ADC 1023 -> 499 C
 * ==========================================================================
 *
 * Raw ADC count to degrees Celsius. An LM35 on a 5 V reference through a
 * 10-bit ADC converts like this:
 *
 *     temperature = adc * 500 / 1024
 *
 * Three things are load-bearing:
 *   - cast to uint32_t BEFORE the multiply, or it overflows
 *   - multiply THEN divide; `adc / 1024 * 500` is 0 for every room
 *   - the return type is uint16_t on purpose: a full-scale 1023 is 499 C,
 *     which does not fit a uint8_t and would come back as 243
 *
 * If you get 0 for everything, you divided before you multiplied.
 */
uint16_t tempC(uint16_t adc)
{
    (void)adc;      /* delete this line */
    return 0U;      /* TODO */
}


/* ==========================================================================
 *  [ 3 / 6 ]   YOUR WORK HERE  —  applyRules()                       FR-10
 *              *** THE BIG ONE — this is the heart of the project ***
 * --------------------------------------------------------------------------
 *  REQUIRES : [ 2 / 6 ] tempC() must work — the rules compare temperatures.
 *  GIVES    : nothing visible on its own. [ 4 / 6 ] is what calls it.
 *  USES     : READ_BIT, SET_BIT, CLR_BIT, tempC(),
 *             BIT_AUTO, BIT_OCCUPIED, BIT_LAMP, BIT_FAN, BIT_ALARM,
 *             TEMP_HOT (28), TEMP_ALARM (45)
 *  CHECK    : do it together with [ 4 / 6 ], then press 5 twice.
 * ==========================================================================
 *
 * THE THREE RULES, applied to ONE room, IN THIS ORDER:
 *
 *   R1  light follows people
 *         OCCUPIED set  ->  LAMP on,  else LAMP off
 *   R2  fan follows heat
 *         tempC >= TEMP_HOT    ->  FAN on,   else FAN off
 *   R3  overheat overrides R1
 *         tempC >= TEMP_ALARM  ->  ALARM on AND LAMP on,  else ALARM off
 *
 * A room whose AUTO bit is clear must be left COMPLETELY alone — return 0
 * immediately, before touching anything.
 *
 * Return 1 if the status byte changed, 0 if it did not. (Save the old value
 * at the top, compare at the bottom.)
 *
 * TWO THINGS THAT COST MARKS
 *   - `if` with no `else`. R1 must turn the lamp OFF for an empty room, not
 *     merely fail to turn it on. Same for R2. Without the else, the bit gets
 *     set once and never clears, and every automation pass reports changes
 *     forever.
 *   - Getting R3's position wrong. R3 runs LAST and overwrites R1 on
 *     purpose, so an overheating empty room still lights up. In an if/else
 *     chain the last write wins — rule order is a design decision. Your
 *     README has to explain what happens if you move R3 first.
 */
uint8_t applyRules(Room_t *r)
{
    (void)r;        /* delete this line */
    return 0U;      /* TODO */
}


/* ==========================================================================
 *  [ 4 / 6 ]   YOUR WORK HERE  —  rulesPass()                        FR-10
 * --------------------------------------------------------------------------
 *  REQUIRES : [ 3 / 6 ] applyRules().
 *  GIVES    : a big one — menu option 7 (the scripted demo) comes alive and
 *             runs the whole evening by itself. That is your free test rig.
 *  USES     : applyRules(), ROOM_COUNT
 *  CHECK    : press 7 and watch the story. Lamps must follow people, the
 *             Kitchen fan must start when it heats up, the Garage must alarm.
 * ==========================================================================
 *
 * One silent pass: applyRules() over all six rooms. Return how many changed
 * (add up what applyRules returns).
 *
 * SELF-CHECK: once [ 5 / 5 ] runAutomation() in ui.c exists, run the pass
 * twice from the menu. A correct rule set reports 5 changed, then 0 changed.
 * If yours keeps reporting changes forever, go back to [ 3 / 6 ] and look for
 * the missing `else`.
 */
uint8_t rulesPass(void)
{
    return 0U;      /* TODO */
}


/* ==========================================================================
 *  [ 5 / 6 ]   YOUR WORK HERE  —  countRoomsWith()                   FR-11
 * --------------------------------------------------------------------------
 *  REQUIRES : nothing. Small and independent — a good breather after [3/6].
 *  GIVES    : nothing on its own; the report in ui.c uses it.
 *  USES     : READ_BIT, ROOM_COUNT
 *  CHECK    : with [ 4 / 5 ] houseReport() done, the "Lamps ON 3/6" counters
 *             must match what you can count on the schematic yourself.
 * ==========================================================================
 *
 * How many of the six rooms have this bit set. One loop, one READ_BIT.
 */
uint8_t countRoomsWith(uint8_t bit)
{
    (void)bit;      /* delete this line */
    return 0U;      /* TODO */
}


/* ==========================================================================
 *  [ 6 / 6 ]   YOUR WORK HERE  —  sumAdc()                           FR-11
 *              *** MUST BE RECURSIVE — a for loop here scores zero ***
 * --------------------------------------------------------------------------
 *  REQUIRES : nothing.
 *  GIVES    : the "Average" line of the house report.
 *  USES     : itself. That is the point.
 *  CHECK    : with the seed house, the raw sum is 363 and the average is 29 C
 * ==========================================================================
 *
 * Sum every room's raw ADC count.
 *
 *     base case : n == 0  ->  0
 *     step      : rooms[n - 1].adc + sumAdc(rooms, n - 1)
 *
 * Base case FIRST, always. `n` is uint8_t, so a missing base case does not
 * stop at -1 — it wraps to 255 and keeps going until the stack is gone.
 *
 * Walk it on paper for two rooms {51, 64}:
 *     sumAdc(r,2) = r[1].adc + sumAdc(r,1)
 *                 = 64       + (r[0].adc + sumAdc(r,0))
 *                 = 64       + (51       + 0)            = 115
 */
uint32_t sumAdc(const Room_t *rooms, uint8_t n)
{
    (void)rooms; (void)n;   /* delete this line */
    return 0UL;             /* TODO */
}
