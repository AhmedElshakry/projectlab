/**
 * platform.c — DEMO EXTRAS. Colour, screen clearing, timing.
 *
 * Not part of the assignment.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#ifndef _WIN32
#  define _POSIX_C_SOURCE 200809L   /* isatty() under -std=c99 */
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include "platform.h"

int g_ansi  = 1;
int g_plain = 0;

void platformInit(void)
{
    /* NO_COLOR set: plain text, no clearing, no delays */
    if (getenv("NO_COLOR") != NULL) {
        g_ansi = 0;
        g_plain = 1;
        return;
    }
#ifdef _WIN32
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;

        /* GetConsoleMode fails on a pipe or a file — that is the "not a
         * terminal" test, so redirected output comes out as clean plain text. */
        if (h == INVALID_HANDLE_VALUE || !GetConsoleMode(h, &mode)) {
            g_ansi = 0;
            g_plain = 1;
            return;
        }
        /* Windows 10+ understands ANSI once virtual-terminal mode is on */
        if (!SetConsoleMode(h, mode | 0x0004u /* VIRTUAL_TERMINAL_PROCESSING */)) {
            g_ansi = 0;   /* old console: no colour, everything still works */
        }
    }
#else
    if (!isatty(fileno(stdout))) {
        g_ansi = 0;
        g_plain = 1;
        return;
    }
#endif
}

void msleep(unsigned ms)
{
    if (g_plain) { return; }
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000u);
#endif
}

void clearScreen(void)
{
    if (g_plain) { printf("\n"); return; }
    if (g_ansi)  { printf("\033[H\033[2J\033[3J"); return; }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
