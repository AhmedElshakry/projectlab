/**
 * demo.c — DEMO EXTRA. The scripted evening on menu option 7.
 *
 * Not part of the assignment. It drives the same model the students write —
 * every beat is just a bit or an ADC count plus a rulesPass() — so nothing
 * here is faked for the projector.
 *
 * The story exists to land two points:
 *   R3 runs LAST and overwrites R1 (the Garage lights up while empty)
 *   a human touch clears BIT_AUTO (the Bedroom drops out of automation)
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#include "demo.h"
#include "house.h"
#include "render.h"
#include "ui.h"
#include "platform.h"

/* set the caption, redraw with one room lit, hold it long enough to read */
static void demoBeat(const char *caption, int hl, unsigned ms)
{
    statusSet(C_SEL, "%s", caption);
    render(hl);
    msleep(ms);
}

void scriptedDemo(void)
{
    houseInit();
    for (uint8_t i = 0U; i < ROOM_COUNT; i++) {
        CLR_BIT(houseRoom(i)->status, BIT_OCCUPIED);
    }
    houseRoom(ROOM_GARAGE)->adc = 60U;   /* start cool, heat it up later */

    demoBeat("17:00  The house is empty. Every room is in AUTO.", -1, 1600);
    rulesPass();
    demoBeat("17:00  Automation ran: nothing occupied, so every lamp is off.",
             -1, 1900);

    SET_BIT(houseRoom(ROOM_HALL)->status, BIT_OCCUPIED);
    demoBeat("18:30  Somebody comes home. The Hall motion sensor sets bit 2.",
             ROOM_HALL, 1700);
    rulesPass();
    demoBeat("18:30  R1 fires: light follows people. The Hall lamp is on.",
             ROOM_HALL, 1900);

    CLR_BIT(houseRoom(ROOM_HALL)->status, BIT_OCCUPIED);
    SET_BIT(houseRoom(ROOM_LIVING)->status, BIT_OCCUPIED);
    rulesPass();
    demoBeat("18:35  They walk into the Living room. R1 moves the light with them.",
             ROOM_LIVING, 2000);

    houseRoom(ROOM_KITCHEN)->adc = 78U;   /* ~38 C */
    demoBeat("19:00  Cooking starts. The Kitchen sensor climbs to 38 C.",
             ROOM_KITCHEN, 1700);
    rulesPass();
    demoBeat("19:00  R2 fires: 38 >= 28, so the Kitchen fan switches itself on.",
             ROOM_KITCHEN, 2000);

    houseRoom(ROOM_GARAGE)->adc = 100U;   /* ~48 C */
    demoBeat("20:00  The Garage hits 48 C.", ROOM_GARAGE, 1500);
    rulesPass();
    demoBeat("20:00  R3 fires LAST and overrides R1: alarm on, and the lamp is "
             "forced on even though nobody is in there.", ROOM_GARAGE, 2600);

    TOGGLE_BIT(houseRoom(ROOM_BEDROOM)->status, BIT_LAMP);
    CLR_BIT(houseRoom(ROOM_BEDROOM)->status, BIT_AUTO);
    demoBeat("22:00  Somebody switches the Bedroom lamp on by hand. "
             "That clears BIT_AUTO — the room is MANUAL now.", ROOM_BEDROOM, 2600);

    rulesPass();
    demoBeat("22:00  Automation runs again and skips the Bedroom entirely. "
             "Manual override wins until you hand control back.", ROOM_BEDROOM, 2600);

    statusSet(C_OK, "End of the story. Press 5 to run the rules yourself, "
                    "or 2 to take a room manual.");
    render(-1);
    pauseKey();
}
