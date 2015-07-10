/**
 * Pebble Watchface: Oh dear! Oh dear! I shall be too late!
 * file: watchface.h
 *
 * Project Header file with shared declarations.
 * 
 * Copyright (c), 2015
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

#define TIMEOUT_TEN_SECONDS 10
#define TIME_OFFSET 300

/*
static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {6, 0},
    {0, -36},
    {-6, 0}
  }
};

static const GPathInfo MINUTE_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {4, 0},
    {0, -54},
    {-4, 0}
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {3, 0},
    {0, -54},
    {-3, 0}
  }
};
*/

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {6, 0},
    {0, -30},
    {-6, 0}
  }
};

static const GPathInfo MINUTE_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {4, 0},
    {0, -48},
    {-4, 0}
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4, (GPoint []){
    {0, 7},
    {3, 0},
    {0, -48},
    {-3, 0}
  }
};

// #define RESOURCE_ID_IMAGE_BACKGROUND RESOURCE_ID_IMAGE_BACKGROUND_DALI
#define RESOURCE_ID_IMAGE_BACKGROUND RESOURCE_ID_IMAGE_WHITE_RABBIT

// #define CENTER_DALI (GPoint){90, 70}
// #define CENTER CENTER_DALI
#define CENTER_CARROLL (GPoint){30, 130}
#define CENTER CENTER_CARROLL

#if defined(GColorSunsetOrange)
  #define GColorHand GColorSunsetOrange 
#else
  #define GColorHand GColorWhite
#endif /* GColorSunsetOrange */

/**
 * Watchface
 */

void watchface_invalidate(void);
void watchface_set_tick_timer_service(void);

/**
 * Config message handling
 */

void config_message_drop(AppMessageResult reason, void *context);
void config_message_receive(DictionaryIterator *iterator, void *context);

/**
 * Persistent storage
 */

typedef struct {
  bool enable_second_hand;
  bool sweep_minute_hand;
} __attribute__((__packed__)) watchface_persistent_config_t;
extern watchface_persistent_config_t watchface_persistent_config;

void watchface_persistent_load(void);
void watchface_persistent_store(void);
