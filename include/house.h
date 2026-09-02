/**
 * house.h — the model: the six rooms, the status byte, and the rules.
 *
 * This is the layer that OWNS the data. Nothing outside house.c may touch
 * the array directly — callers go through houseRoom(). That is NFR-03 of the
 * brief ("static at file scope, main() never indexes it") surviving the split
 * into modules.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#ifndef HOUSE_H
#define HOUSE_H

#include <stdint.h>

/* ------------------------- the house ------------------------- */
#define ROOM_COUNT    6U
#define GRID_COLS     3U        /* rooms per row on the floor plan */
#define GRID_ROWS     2U
#define NAME_LEN      12U

/* ------------------------- sensing --------------------------- */
#define ADC_MAX       1023U     /* 10-bit ADC, like the AVR's      */
#define TEMP_HOT      28U       /* R2 threshold, degrees C         */
#define TEMP_ALARM    45U       /* R3 threshold, degrees C         */

/* ---------------- status register bit positions -------------- */
#define BIT_LAMP      0U
#define BIT_FAN       1U
#define BIT_OCCUPIED  2U
#define BIT_AUTO      3U
#define BIT_ALARM     4U

/* bit macros — the ONLY allowed way to touch a status byte */
#define SET_BIT(reg, n)    ((reg) |=  (uint8_t)(1U << (n)))
#define CLR_BIT(reg, n)    ((reg) &= (uint8_t)~(1U << (n)))
#define TOGGLE_BIT(reg, n) ((reg) ^=  (uint8_t)(1U << (n)))
#define READ_BIT(reg, n)   ((uint8_t)(((reg) >> (n)) & 1U))

/* ------------------------- data model ------------------------ */
typedef enum {
    ROOM_LIVING = 0, ROOM_KITCHEN, ROOM_BEDROOM,
    ROOM_BATHROOM,   ROOM_HALL,    ROOM_GARAGE
} RoomId_t;

typedef struct {
    char     name[NAME_LEN];   /* "Living", "Kitchen", ...          */
    uint8_t  status;           /* ALARM AUTO OCCUPIED FAN LAMP      */
    uint16_t adc;              /* raw 10-bit sensor count, 0..1023  */
} Room_t;

/* --------------------------- API ----------------------------- */

/* FR-03 — build the six rooms. The house never changes size. */
void houseInit(void);

/* the only door to the array. i must be < ROOM_COUNT. */
Room_t *houseRoom(uint8_t i);

/* FR-05 — raw ADC count to degrees Celsius. Integer maths, no float. */
uint16_t tempC(uint16_t adc);

/* FR-11 — how many rooms have this bit set. */
uint8_t countRoomsWith(uint8_t bit);

/* FR-11 — recursive sum of every room's raw ADC count. */
uint32_t sumAdc(const Room_t *rooms, uint8_t n);

/* the whole array, read-only — for sumAdc() and the report. */
const Room_t *houseRooms(void);

/* FR-10 — R1, R2, R3 applied to ONE room. Returns 1 if its status changed.
 * A room with BIT_AUTO clear is left alone and returns 0. */
uint8_t applyRules(Room_t *r);

/* FR-10 — applyRules() over the whole house. Returns how many changed. */
uint8_t rulesPass(void);

#endif /* HOUSE_H */
