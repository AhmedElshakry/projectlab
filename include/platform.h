/**
 * platform.h — DEMO EXTRAS. Colour, screen clearing, timing.
 *
 * None of this is part of the assignment. It is the projector polish that
 * makes the schematic redraw in place and light up. The students' version
 * prints plain text and scrolls; it still scores full marks.
 *
 * Every colour goes through CC(), so one flag strips the lot.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#ifndef PLATFORM_H
#define PLATFORM_H

/* g_ansi  : colour codes are being emitted
 * g_plain : not a terminal (piped/redirected) or NO_COLOR — also means
 *           no screen clearing, no delays and no "press Enter" pauses,
 *           so `house.exe > out.txt` produces clean plain text. */
extern int g_ansi;
extern int g_plain;

#define CC(x)  (g_ansi ? (x) : "")

#define C_RESET  "\033[0m"
#define C_DIM    "\033[90m"
#define C_EDGE   "\033[90m"
#define C_SEL    "\033[97m"
#define C_LAMP   "\033[93m"
#define C_FAN    "\033[96m"
#define C_OK     "\033[92m"
#define C_ALARM  "\033[91m"
#define C_AUTO   "\033[36m"
#define C_MAN    "\033[33m"
#define C_COOL   "\033[94m"
#define C_WARM   "\033[33m"
#define C_TITLE  "\033[1;97m"

/* decide g_ansi / g_plain and switch the Windows console into ANSI mode */
void platformInit(void);

/* sleep, unless we are in plain mode */
void msleep(unsigned ms);

/* home the cursor and wipe the screen, unless we are in plain mode */
void clearScreen(void);

#endif /* PLATFORM_H */
