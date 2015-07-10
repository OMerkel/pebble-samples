/**
 * Pebble Watchface: Oh dear! Oh dear! I shall be too late!
 * file: config.c
 *
 * Config Message (AppMessage) handling for the Pebble Watchface.
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
#include "watchface.h"

/**
 * enum values have to match JS declaration of appKeys in appinfo.json
 */
enum {
  KEY_ENABLE_SECOND_HAND = 0,
  KEY_SWEEP_MINUTE_HAND = 1,
};

void config_message_drop(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Config message dropped!");
}

/**
 * config message received as generated in JS part of config/config-page-v1.html
 */
void config_message_receive(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);
  while ( t ) {
    switch (t->key) {
      case KEY_ENABLE_SECOND_HAND:
        watchface_persistent_config.enable_second_hand = (bool) t->value->int32;
        break;
      case KEY_SWEEP_MINUTE_HAND:
        watchface_persistent_config.sweep_minute_hand = (bool) t->value->int32;
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Config message key %d is unknown",
          (int) t->key);
        break;
    }
    t = dict_read_next(iterator);
  }
  watchface_invalidate();
  watchface_set_tick_timer_service();
}
