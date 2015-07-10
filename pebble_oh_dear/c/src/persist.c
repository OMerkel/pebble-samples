/**
 * Pebble Watchface: Oh dear! Oh dear! I shall be too late!
 * file: persist.c
 *
 * Persistent Storage handling for the Pebble Watchface.
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

#define PERSISTENT_STORAGE_VERSION_KEY 1
#define PERSISTENT_STORAGE_DATA_KEY 2

static const int32_t s_persistent_storage_version = 1;
static const watchface_persistent_config_t s_config_defaults = {
  .enable_second_hand = true,
  .sweep_minute_hand = true
};

watchface_persistent_config_t watchface_persistent_config;

static void read_v1(void) {
  persist_read_data(PERSISTENT_STORAGE_DATA_KEY, &watchface_persistent_config,
    sizeof(watchface_persistent_config));
}

void watchface_persistent_load(void) {
  watchface_persistent_config = s_config_defaults;
  if (persist_exists(PERSISTENT_STORAGE_VERSION_KEY)) {
    int version = persist_read_int(PERSISTENT_STORAGE_VERSION_KEY);

    switch (version) {
      case 1:
        read_v1();
        break;

      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown persistent storage data version: %d", version);
        break;
    }
  }
}

void watchface_persistent_store(void) {
  persist_write_int(PERSISTENT_STORAGE_VERSION_KEY, s_persistent_storage_version);
  persist_write_data(PERSISTENT_STORAGE_DATA_KEY, &watchface_persistent_config,
    sizeof(watchface_persistent_config));
}
