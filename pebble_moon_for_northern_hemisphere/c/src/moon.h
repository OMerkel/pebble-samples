/**
 * Pebble Watchface: Moon for Northern Hemisphere
 *
 * Header declaration for Moon for Northern Hemisphere watchface.
 *
 * Copyright (c), 2016
 *
 * @AUTHOR Oliver Merkel, merkel(dot)oliver(at)web(dot)de.
 * 
 * License: The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#pragma once

#include <pebble.h>

// indicate midnight by showing small circle
#define SHOW_MIDNIGHT_POSITION

// Slow tick resolution resulting in low update frequency (battery life)
/**
 * Mind that the combination of accelerator event triggering detailed
 * information display is problematic with RESOLUTION_MINUTE.
 * User will expect immediate response on accelarator event.
 * Furthermore the timeout to expire the detailed display should be
 * in minutes range on RESOLUTION_MINUTE.
 */
//#define RESOLUTION_MINUTE

#if defined(RESOLUTION_MINUTE)
  #define TICK_RESOLUTION MINUTE_UNIT
#else
  #define TICK_RESOLUTION SECOND_UNIT
#endif /* RESOLUTION_MINUTE */

#if !defined(RESOLUTION_MINUTE)
// Show second hand (depending on TICK_RESOLUTION)
// #define SHOW_SECOND_HAND
#endif /* RESOLUTION_MINUTE */

static const GPathInfo HOUR_TICK_POINTS = {
  4, (GPoint []){
    {2, -70},
    {-2, -70},
    {-2, -74},
    {2, -74},
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  3, (GPoint []){
    {7, -62},
    {0, -76},
    {-7, -62}
  }
};

#define NUM_MOON_PHASES 16

// 2551392 sec = 2953 * 24 * 36 sec = moon cycle (in sec) is period of synodic month = 29.53 days
#define MOON_CYCLE 2551392

// reasonable offset (in sec) for start of epoche
#define MOON_OFFSET_EPOCHE (638000)

#define TIMEOUT_TEN_SECONDS 10

static time_t mode_detail_expiration = 0;

static Window *s_my_window;
static BitmapLayer *s_moon_layer;
static TextLayer *s_date_text_layer;
static TextLayer *s_time_text_layer;
static GFont s_date_text_font;
static GFont s_time_text_font;
static GPath *s_hour_arrow;
static GPath *s_hour_tick;
static char datebuf[64];
static char timebuf[16];

#if defined(GColorOrange)
  #define GColorHand GColorOrange
#else
  #define GColorHand GColorWhite
#endif /* GColorOrange */

#if defined(GColorYellow)
  #define GColorTick GColorYellow
#else
  #define GColorTick GColorWhite
#endif /* GColorYellow */
