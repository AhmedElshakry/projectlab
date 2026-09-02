/**
 * ============================================================================
 *  SMART HOME CONSOLE  —  G9 project
 *  Day 03 midterm · Microcontroller-Based Systems (90 Hours)
 *
 *  Author : Ahmed Ellamie · ahmed.ellamiee@gmail.com
 *  Build  : build.bat        (or: make)
 *  Run    : house.exe
 *
 *  main() does one job: read a menu choice and dispatch it. It never touches
 *  the house array — that is NFR-03, and it is why every case below is a
 *  single call into another module.
 *
 *      include/house.h     the model: rooms, status byte, the three rules
 *      include/render.h    the view:  the schematic, bars, the status line
 *      include/ui.h        the input layer: menu, prompts, actions
 *      include/platform.h  demo extras: colour, clearing, timing
 *      include/demo.h      demo extra: the scripted evening (option 7)
 *
 *  See README.md for the full map and for what the students must write.
 * ============================================================================
 */
#include <stdio.h>

#include "house.h"
#include "render.h"
#include "ui.h"
#include "platform.h"
#include "demo.h"

int main(void)
{
    int choice = -1;

    platformInit();
    houseInit();
    statusSet(C_DIM,
              "Six rooms, all in AUTO, every lamp and fan off. Try option 5.");

    do {
        render(-1);
        printMenu();

        if (!readInt(&choice)) { choice = -1; }

        switch (choice) {
            case 1: break;                    /* the loop redraws anyway */
            case 2: switchDevice();   break;
            case 3: setOccupancy();   break;
            case 4: setTemperature(); break;
            case 5: runAutomation();  break;
            case 6: houseReport();    break;
            case 7: scriptedDemo();   break;
            case 0:
                printf("\n%s  Good night.%s\n\n", CC(C_DIM), CC(C_RESET));
                break;
            default:
                statusSet(C_ALARM, "Invalid choice, try again.");
                break;
        }
    } while (choice != 0);

    return 0;
}
