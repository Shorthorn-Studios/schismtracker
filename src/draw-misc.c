/*
 * Schism Tracker - a cross-platform Impulse Tracker clone
 * copyright (c) 2003-2004 chisel <someguy@here.is> <http://here.is/someguy/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "headers.h"

#include "it.h"
#include "page.h"

/* --------------------------------------------------------------------- */
/* Thumb bars */

static inline void _draw_thumb_bar_internal(int width, int x, int y,
                                            int val, Uint32 fg)
{
        const byte thumb_chars[2][8] = {
                {155, 156, 157, 158, 159, 160, 161, 162},
                {0, 0, 0, 163, 164, 165, 166, 167}
        };
        int n = ++val >> 3;

        val %= 8;
        draw_fill_chars(x, y, x + n - 1, y, 0);
        SDL_LockSurface(screen);
        draw_char_unlocked(thumb_chars[0][val], x + n, y, fg, 0);
        if (++n < width)
                draw_char_unlocked(thumb_chars[1][val], x + n, y, fg, 0);
        SDL_UnlockSurface(screen);
        if (++n < width)
                draw_fill_chars(x + n, y, x + width - 1, y, 0);
}

void draw_thumb_bar(int x, int y, int width, int min, int max, int val,
                    int selected)
{
        /* this wouldn't happen in a perfect world :P */
        if (val < min || val > max) {
                draw_fill_chars(x, y, x + width - 1, y,
                                ((status.flags & CLASSIC_MODE) ? 2 : 0));
                return;
        }

        /* fix the range so that it's 0->n */
        val -= min;
        max -= min;

        /* draw the bar */
        _draw_thumb_bar_internal(width, x, y, val * (width - 1) * 8 / max,
                                 selected ? 3 : 2);
}

/* --------------------------------------------------------------------- */
/* VU meters */

void draw_vu_meter(int x, int y, int width, int val, int color, int peak)
{
        const byte endtext[8][3] = {
                {174, 0, 0}, {175, 0, 0}, {176, 0, 0}, {176, 177, 0},
                {176, 178, 0}, {176, 179, 180}, {176, 179, 181},
                {176, 179, 182},
        };
        int leftover;
	int chunks = (width / 3);
	int maxval = width * 8 / 3;
	
	// val = val * maxval / 64
	// val = val * (width * 8 / 3) / 64
	val = val * width / 24;
	
        if (!val)
                return;
	if (val == maxval)
		val--;
	
        leftover = val & 7;
        val >>= 3;
	if ((val < chunks - 1) || (status.flags & CLASSIC_MODE))
		peak = color;
	
        SDL_LockSurface(screen);
	
        draw_char_unlocked(endtext[leftover][0], 3 * val + x + 0, y, peak, 0);
        draw_char_unlocked(endtext[leftover][1], 3 * val + x + 1, y, peak, 0);
        draw_char_unlocked(endtext[leftover][2], 3 * val + x + 2, y, peak, 0);
        while (val) {
                val--;
                draw_char_unlocked(176, 3 * val + x + 0, y, color, 0);
                draw_char_unlocked(179, 3 * val + x + 1, y, color, 0);
                draw_char_unlocked(182, 3 * val + x + 2, y, color, 0);
        }

        SDL_UnlockSurface(screen);
}
