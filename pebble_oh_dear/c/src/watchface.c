/**
 * Pebble Watchface: Oh dear! Oh dear! I shall be too late!
 *
 * "CHAPTER I.
 *
 * DOWN THE RABBIT-HOLE.
 *
 * Alice was beginning to get very tired of sitting by her
 * sister on the bank, and of having nothing to do: once or
 * twice she had peeped into the book her sister was reading,
 * but it had no pictures or conversations in it, "and what
 * is the use of a book," thought Alice, "without pictures
 * or conversations?"
 *
 * So she was considering in her own mind, (as well as she
 * could, for the hot day made her feel very sleepy and
 * stupid,) whether the pleasure of making a daisy-chain
 * would be worth the trouble of getting up and picking
 * the daisies, when suddenly a white rabbit with pink
 * eyes ran close by her.
 *
 * There was nothing so very remarkable in that; nor did
 * Alice think it so very much out of the way to hear the
 * Rabbit say to itself,
 *       "Oh dear! Oh dear! I shall be too late!"
 * (when she thought it over afterwards, it occurred to her
 * that she ought to have wondered at this, but at the time
 * it all seemed quite natural;) but when the Rabbit
 * actually took a watch out of its waistcoat-pocket, and
 * looked at it, and then hurried on, Alice started to her
 * feet, for it flashed across her mind that she had never
 * before seen a rabbit with either a waistcoat-pocket, or
 * a watch to take out of it, and, burning with curiosity,
 * she ran across the field after it, and was just in time
 * to see it pop down a large rabbit-hole under the hedge."
 *
 * ... from Lewis Carroll's "Alice in Wonderland"
 *
 * Trust this watch face and you will be 5 Minutes too late!
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND... even if you are too late.
 *
 * The White Rabbit being part of the
 * background image is work from John Tenniel.
 * John Tenniel illustrated early editions of
 * Lewis Carroll's "Alice in Wonderland" back
 * in 19th century.
 *
 * Mind that works of John Tenniel and
 * Lewis Carroll are in the public domain yet!
 *
 * All other portions of this Pebble watchface are MIT licensed.
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

static Window *s_my_window;
static BitmapLayer *s_background_layer;
static TextLayer *s_text_layer;
static GFont s_text_font;
static GBitmap* bm_background;
static char timebuf[64];
static time_t mode_detail_expiration = 0;
static GPath *s_hour_arrow;
static GPath *s_minute_arrow;
static GPath *s_second_arrow;

static void update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  graphics_draw_bitmap_in_rect(ctx, bm_background, bounds);

  time_t now = time(NULL) - TIME_OFFSET;
  struct tm *t = localtime(&now);

  graphics_context_set_fill_color(ctx, GColorHand);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  // gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * (t->tm_hour % 12) / 12.0);
  gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * ((float)(t->tm_hour % 12) + t->tm_min / 60.0) / 12.0);
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * ((float)t->tm_min + 
    (watchface_persistent_config.sweep_minute_hand ? (float)t->tm_sec / 60.0 : 0)) / 60.0);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);

  if (watchface_persistent_config.enable_second_hand) {
    gpath_rotate_to(s_second_arrow, TRIG_MAX_ANGLE * t->tm_sec / 60.0);
    gpath_draw_filled(ctx, s_second_arrow);
    gpath_draw_outline(ctx, s_second_arrow);
  }

  if (0 < mode_detail_expiration) {
    if (now > mode_detail_expiration) {
      mode_detail_expiration = 0;
      timebuf[0] = '\0';
      text_layer_set_background_color(s_text_layer, GColorClear);
    }
    else {
      /*
      t->tm_mday = 4;
      t->tm_mon = 2;
      mktime(t);
      */

      timebuf[sizeof(timebuf) - 1] = '\0';
      strftime(timebuf, sizeof(timebuf) - 1, "CW%W %a %b %e %Y", t);
      if ('9' == timebuf[3]) {
        ++timebuf[2];
        timebuf[3] = '0';
      }
      else {
        ++timebuf[3];
      }
      // graphics_fill_circle(ctx, (GPoint){71, 157}, 4);
    }
  }
}

void watchface_invalidate(void) {
  layer_mark_dirty(window_get_root_layer(s_my_window));  
}

static void handle_accelerometer(AccelAxisType axis, int32_t direction) {
  time_t t = time(NULL);
  mode_detail_expiration = t - TIME_OFFSET + TIMEOUT_TEN_SECONDS;
  text_layer_set_background_color(s_text_layer, GColorBlack);
  watchface_invalidate();
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  watchface_invalidate();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  bm_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  gpath_move_to(s_hour_arrow, CENTER);

  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  gpath_move_to(s_minute_arrow, CENTER);

  s_second_arrow = gpath_create(&SECOND_HAND_POINTS);
  gpath_move_to(s_second_arrow, CENTER);

  s_background_layer = bitmap_layer_create(bounds);
  layer_set_update_proc(bitmap_layer_get_layer(s_background_layer), update_proc);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  s_text_layer = text_layer_create(GRect(0, 154, 144, 168));
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorWhite);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentRight);
  layer_add_child(bitmap_layer_get_layer(s_background_layer), text_layer_get_layer(s_text_layer));

  s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_14));
  text_layer_set_font(s_text_layer, s_text_font);

  timebuf[0] = '\0';
  text_layer_set_text(s_text_layer, timebuf);
}

static void window_unload(Window *window) {
  fonts_unload_custom_font(s_text_font);
  text_layer_destroy(s_text_layer);
  bitmap_layer_destroy(s_background_layer);
  gpath_destroy(s_second_arrow);
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);

  gbitmap_destroy(bm_background);
}

void watchface_set_tick_timer_service(void) {
  tick_timer_service_subscribe(watchface_persistent_config.sweep_minute_hand ||
    watchface_persistent_config.enable_second_hand ?
    SECOND_UNIT : MINUTE_UNIT, handle_tick);
}

void handle_init(void) {
  watchface_persistent_load();
  s_my_window = window_create();

  window_set_window_handlers(s_my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(s_my_window, true);
  watchface_set_tick_timer_service();
  accel_tap_service_subscribe(handle_accelerometer);

  app_message_register_inbox_received(config_message_receive);
  app_message_register_inbox_dropped(config_message_drop);
  app_message_open(app_message_inbox_size_maximum(), 0);
}

void handle_deinit(void) {
  app_message_register_inbox_dropped(NULL);
  app_message_register_inbox_received(NULL);
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
  watchface_persistent_store();
  window_destroy(s_my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
