/**
 * render.h — the view. Everything here READS the house and prints it.
 *
 * Nothing in render.c writes a status bit. If a drawing function ever changes
 * the model, automation and visualisation have merged and both become
 * untestable — that separation is §9 of the brief and the actual engineering
 * lesson of the project.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "house.h"

/* ------------------------- drawing --------------------------- */
#define CARD_W        18U       /* inner width of one room card    */
#define CARD_LINES    5U        /* text lines inside one card      */
#define BAR_W         8U        /* temperature bar in a room card  */
#define REPORT_BAR_W  12U       /* bars in the house report        */
#define BAR_FULL_C    50U       /* a full temperature bar = 50 C   */

/* FR-06 — print exactly `width` chars: '#' for the filled part, '-' for the
 * rest. Used by BOTH the room card and the report — written once.
 * `code` is a colour from platform.h (ignored when colour is off). */
void drawBar(uint16_t value, uint16_t full, uint8_t width, const char *code);

/* FR-04 — one line of one room card, exactly CARD_W visible characters. */
void drawRoomLine(const Room_t *r, uint8_t line);

/* FR-04 — the whole schematic. `highlight` is the room that just changed
 * (its border glows), or -1 for none. */
void drawHouse(int highlight);

void drawTitle(void);
void drawLegend(void);

/* the one-line message under the schematic */
void        statusSet(const char *colour, const char *fmt, ...);
const char *statusText(void);
const char *statusColour(void);

/* the whole live view: clear, title, schematic, legend, status line */
void render(int highlight);

#endif /* RENDER_H */
