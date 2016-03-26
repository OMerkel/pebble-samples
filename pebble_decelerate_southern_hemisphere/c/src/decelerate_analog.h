/**
 * Pebble Watchface: Decelerate Southern Hemisphere
 *
 * Header declaration for Decelerate Southern Hemisphere watchface
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

#define NUM_CLOCK_TICKS 9

static const struct GPathInfo ANALOG_BG_POINTS[] = {
  { 4, (GPoint []){
      {70, 0},
      {73, 0},
      {73, 12},
      {70, 12}
    }
  },
  { 4, (GPoint []){
      {112, 10},
      {114, 12},
      {108, 23},
      {106, 21}
    }
  },
  { 4, (GPoint []){
      {132, 47},
      {144, 40},
      {144, 44},
      {135, 49}
    }
  },
  { 4, (GPoint []){
      {135, 118},
      {144, 123},
      {144, 126},
      {132, 120}
    }
  },
  { 4, (GPoint []){
      {108, 144},
      {114, 154},
      {112, 157},
      {106, 147}
    }
  },
  { 4, (GPoint []){
      {32, 10},
      {30, 12},
      {36, 23},
      {38, 21}
    }
  },
  { 4, (GPoint []){
      {12, 47},
      {-1, 40},
      {-1, 44},
      {9, 49}
    }
  },
  { 4, (GPoint []){
      {9, 118},
      {-1, 123},
      {-1, 126},
      {12, 120}
    }
  },
  { 4, (GPoint []){
      {36, 144},
      {30, 154},
      {32, 157},
      {38, 147}
    }
  },

};

static const GPathInfo HOUR_HAND_POINTS = {
  5, (GPoint []){
    {-5, 5},
    {5, 5},
    {3, -70},
    {0, -75},
    {-3, -70}
  }
};

#define NUM_MOON_PHASES 16

// 2551392 = 2953 * 24 * 36 = moon cycle is period of 29.53 days
#define MOON_CYCLE 2551392

// reasonable offset for start of epoche
#define MOON_OFFSET_EPOCHE (638000)

static const int MOON_ID[] = {
  RESOURCE_ID_IMAGE_MOON_0,
  RESOURCE_ID_IMAGE_MOON_2,
  RESOURCE_ID_IMAGE_MOON_4,
  RESOURCE_ID_IMAGE_MOON_6,
  RESOURCE_ID_IMAGE_MOON_8,
  RESOURCE_ID_IMAGE_MOON_10,
  RESOURCE_ID_IMAGE_MOON_12,
  RESOURCE_ID_IMAGE_MOON_14,
  RESOURCE_ID_IMAGE_MOON_16,
  RESOURCE_ID_IMAGE_MOON_18,
  RESOURCE_ID_IMAGE_MOON_20,
  RESOURCE_ID_IMAGE_MOON_22,
  RESOURCE_ID_IMAGE_MOON_24,
  RESOURCE_ID_IMAGE_MOON_26,
  RESOURCE_ID_IMAGE_MOON_28,
  RESOURCE_ID_IMAGE_MOON_30
};

GBitmap* MOON[NUM_MOON_PHASES];
