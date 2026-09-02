/**
 * render.c — the view. Reads the house, prints it. Writes nothing.
 *
 * The colour codes are zero-width, and they are always placed AROUND the
 * padded fields, never inside them — so every room card is exactly CARD_W
 * (18) visible characters whether colour is on or off. That is why the
 * schematic is byte-identical to the plain version the students write.
 *
 * Smart Home Console · Day 03 midterm — G9
 * Author: Ahmed Ellamie · ahmed.ellamiee@gmail.com
 */
#include <stdio.h>
#include <stdarg.h>

#include "render.h"
#include "platform.h"

static const char *const VERSION = "SMART HOME CONSOLE  v2.0";

/* the one-line message under the schematic */
static char        g_msg[192] = "";
static const char *g_msgCol   = C_DIM;

void statusSet(const char *colour, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_msg, sizeof g_msg, fmt, ap);
    va_end(ap);
    g_msgCol = colour;
}

const char *statusText(void)   { return g_msg; }
const char *statusColour(void) { return g_msgCol; }

/* FR-06 — one bar renderer, used by the room card AND the report. */
void drawBar(uint16_t value, uint16_t full, uint8_t width, const char *code)
{
    uint8_t filled = (value >= full)
                   ? width
                   : (uint8_t)(((uint32_t)value * width) / full);

    printf("%s", CC(code));
    for (uint8_t i = 0U; i < filled; i++) { putchar('#'); }
    printf("%s", CC(C_DIM));
    for (uint8_t i = filled; i < width; i++) { putchar('-'); }
    printf("%s", CC(C_RESET));
}

/* FR-04 — one line of one room card. Each case prints EXACTLY CARD_W (18)
 * visible characters, or the columns drift and the right border goes ragged.
 *
 *   line 0 : " %-11s %-5s"            name, then AUTO / MAN
 *   line 1 : "  Lamp %-3s Fan %-3s"   [#]/[ ] and (#)/( )
 *   line 2 : "  %3u C " + 8-wide bar + "  "
 *   line 3 : "  people: %-8s"
 *   line 4 : "  %-16s"                !! OVERHEAT !! or blank
 */
void drawRoomLine(const Room_t *r, uint8_t line)
{
    uint16_t t = tempC(r->adc);

    switch (line) {
        case 0: {
            int a = READ_BIT(r->status, BIT_AUTO);
            printf(" %-11s %s%-5s%s", r->name,
                   CC(a ? C_AUTO : C_MAN), a ? "AUTO" : "MAN", CC(C_RESET));
            break;
        }
        case 1: {
            int lamp = READ_BIT(r->status, BIT_LAMP);
            int fan  = READ_BIT(r->status, BIT_FAN);
            printf("  Lamp %s%s%s Fan %s%s%s",
                   CC(lamp ? C_LAMP : C_DIM), lamp ? "[#]" : "[ ]", CC(C_RESET),
                   CC(fan  ? C_FAN  : C_DIM), fan  ? "(#)" : "( )", CC(C_RESET));
            break;
        }
        case 2: {
            const char *tc = (t >= TEMP_ALARM) ? C_ALARM
                           : (t >= TEMP_HOT)   ? C_WARM
                                               : C_COOL;
            printf("  %s%3u%s C ", CC(tc), t, CC(C_RESET));
            drawBar(t, BAR_FULL_C, BAR_W, tc);
            printf("  ");
            break;
        }
        case 3: {
            int occ = READ_BIT(r->status, BIT_OCCUPIED);
            printf("  people: %s%-8s%s",
                   CC(occ ? C_OK : C_DIM), occ ? "yes" : "no", CC(C_RESET));
            break;
        }
        default: {
            int al = READ_BIT(r->status, BIT_ALARM);
            printf("  %s%-16s%s",
                   CC(al ? C_ALARM : C_DIM), al ? "!! OVERHEAT !!" : "", CC(C_RESET));
            break;
        }
    }
}

/* one horizontal rule; the segments of a highlighted room glow */
static void drawBorder(int rowAbove, int rowBelow, int hl)
{
    for (uint8_t col = 0U; col < GRID_COLS; col++) {
        int a = (rowAbove >= 0) ? (rowAbove * (int)GRID_COLS + (int)col) : -1;
        int b = (rowBelow >= 0) ? (rowBelow * (int)GRID_COLS + (int)col) : -1;
        int lit = (hl >= 0) && (hl == a || hl == b);

        printf("%s+", CC(lit ? C_SEL : C_EDGE));
        for (uint8_t i = 0U; i < CARD_W; i++) { putchar('-'); }
        printf("%s", CC(C_RESET));
    }
    printf("%s+%s\n", CC(C_EDGE), CC(C_RESET));
}

/* FR-04 — the schematic. `highlight` is the room that just changed, or -1.
 * The house is a 1-D array drawn as a 2-D grid: house[row * GRID_COLS + col]. */
void drawHouse(int highlight)
{
    for (int row = 0; row < (int)GRID_ROWS; row++) {
        drawBorder(row - 1, row, highlight);

        for (uint8_t line = 0U; line < CARD_LINES; line++) {
            for (int col = 0; col < (int)GRID_COLS; col++) {
                int idx = row * (int)GRID_COLS + col;
                int lit = (highlight == idx) || (col > 0 && highlight == idx - 1);
                printf("%s|%s", CC(lit ? C_SEL : C_EDGE), CC(C_RESET));
                drawRoomLine(houseRoom((uint8_t)idx), line);
            }
            {
                int last = row * (int)GRID_COLS + (int)GRID_COLS - 1;
                printf("%s|%s\n", CC(highlight == last ? C_SEL : C_EDGE), CC(C_RESET));
            }
        }
    }
    drawBorder((int)GRID_ROWS - 1, -1, highlight);
}

void drawTitle(void)
{
    printf("%s", CC(C_TITLE));
    /* inner width 56, to match the 3 x 18 schematic below it */
    printf("+--------------------------------------------------------+\n");
    printf("|  %-24s         Day 03 midterm demo  |\n", VERSION);
    printf("+--------------------------------------------------------+%s\n",
           CC(C_RESET));
}

void drawLegend(void)
{
    printf("%s  ", CC(C_DIM));
    printf("%s[#]%s lamp   ", CC(C_LAMP), CC(C_DIM));
    printf("%s(#)%s fan   ", CC(C_FAN), CC(C_DIM));
    printf("%speople%s   ", CC(C_OK), CC(C_DIM));
    printf("%sfan >= %u C%s   ", CC(C_WARM), TEMP_HOT, CC(C_DIM));
    printf("%salarm >= %u C%s", CC(C_ALARM), TEMP_ALARM, CC(C_DIM));
    printf("%s\n", CC(C_RESET));
}

/* the whole live view */
void render(int highlight)
{
    clearScreen();
    drawTitle();
    printf("\n");
    drawHouse(highlight);
    printf("\n");
    drawLegend();
    if (g_msg[0] != '\0') {
        printf("\n  %s%s%s\n", CC(g_msgCol), g_msg, CC(C_RESET));
    }
}
