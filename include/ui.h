/**
 * ui.h — the input layer and the menu actions.
 *
 * Each action asks the user, validates, and writes exactly ONE field of the
 * model. None of them decide anything — deciding is what the rules in
 * house.c do. Keeping those apart is the point of the three-layer split.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#ifndef UI_H
#define UI_H

#include <stdint.h>

void printMenu(void);

/* read one line and parse an int. Returns 0 on EOF or on junk. */
int  readInt(int *out);

/* "press Enter" — a no-op in plain mode so piping never blocks */
void pauseKey(void);

/* ask for a room index. Returns 0..ROOM_COUNT-1, or 255 on bad input. */
uint8_t pickRoom(void);

void printBinary(uint8_t value);

/* menu actions */
void switchDevice(void);      /* FR-07 — lamp / fan / auto, clears AUTO   */
void setOccupancy(void);      /* FR-08 — the motion-sensor bit            */
void setTemperature(void);    /* FR-09 — write a raw ADC count, validated */
void runAutomation(void);     /* FR-10 — the animated rules sweep         */
void houseReport(void);       /* FR-11 — counters, bars, recursive average*/

#endif /* UI_H */
