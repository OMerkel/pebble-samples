/**
 * Pebble Watchface: Moon for Northern Hemisphere
 *
 * The Moon for Northern Hemisphere watchface shows a single hour hand only.
 * The hour hand is represented by an orange triangle.
 * Midnight is on bottom of the clockface while Noon is indicated on top.
 * One rotation of the hour hand takes a whole day. If holding the Pebble watch
 * parallel over ground and turning it so the hour hand is pointing towards
 * the sun then top of the watchface will always point in south direction on
 * northern hemisphere. Additionally the current moon phase is shown for
 * northern hemisphere. Moon phase display is expected to work until end of
 * Unix Time (overflow on Jan 19th, 2038 03:14:07 GMT) at least as
 * implemented here.
 * On Accelerometer Event of the watch more detailed time information is
 * displayed for a specific while. On timeout the display toggles back to
 * normal view.
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

#include "moon.h"

static const int MOON_ID[] = {
  RESOURCE_ID_IMAGE_MOON_0,
  RESOURCE_ID_IMAGE_MOON_1,
  RESOURCE_ID_IMAGE_MOON_2,
  RESOURCE_ID_IMAGE_MOON_3,
  RESOURCE_ID_IMAGE_MOON_4,
  RESOURCE_ID_IMAGE_MOON_5,
  RESOURCE_ID_IMAGE_MOON_6,
  RESOURCE_ID_IMAGE_MOON_7,
  RESOURCE_ID_IMAGE_MOON_8,
  RESOURCE_ID_IMAGE_MOON_9,
  RESOURCE_ID_IMAGE_MOON_10,
  RESOURCE_ID_IMAGE_MOON_11,
  RESOURCE_ID_IMAGE_MOON_12,
  RESOURCE_ID_IMAGE_MOON_13,
  RESOURCE_ID_IMAGE_MOON_14,
  RESOURCE_ID_IMAGE_MOON_15
};

static GBitmap* bm_moon;
static int8_t i_drawn_index;

static void update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

#if defined(SHOW_MIDNIGHT_POSITION)
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  // graphics_fill_circle(ctx, (GPoint){72, 157}, 4);
  graphics_draw_circle(ctx, (GPoint){72, 157}, 4);
#endif

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  /*
  t->tm_mday = 16;
  t->tm_mon = 8;
  t->tm_hour = 16;
  t->tm_min = 0;
  t->tm_year = 116;
  now = mktime(t);
   */

  uint32_t tmp = now - MOON_OFFSET_EPOCHE;
  tmp %= MOON_CYCLE;
  int8_t i_index = (tmp * NUM_MOON_PHASES) / MOON_CYCLE;
  
  if (i_drawn_index != i_index) {
    if (bm_moon) {
      gbitmap_destroy(bm_moon);
    }
    bm_moon = gbitmap_create_with_resource(MOON_ID[i_index]);
    i_drawn_index = i_index;
  }
  
  graphics_draw_bitmap_in_rect(ctx, bm_moon, GRect( 12, 24, 120, 120));

  graphics_context_set_fill_color(ctx, GColorTick);
  graphics_context_set_stroke_color(ctx, GColorTick);
  for(int i=0; i<24; i+=1) {
    if(i != 12) {
      gpath_rotate_to(s_hour_tick, TRIG_MAX_ANGLE * i / 24.0);
      gpath_draw_filled(ctx, s_hour_tick);
      gpath_draw_outline(ctx, s_hour_tick);
    }
  }

  graphics_context_set_fill_color(ctx, GColorHand);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  // gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * (t->tm_sec / 60.0));
  gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * ((t->tm_hour + t->tm_min / 60.0) / 24.0 + 0.5));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

#if defined(SHOW_MINUTE_HAND)
  gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * (t->tm_sec / 60.0));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
#endif /* SHOW_MINUTE_HAND */

//  mode_detail_expiration = now + 10000;
  if (0 < mode_detail_expiration) {
    if (now > mode_detail_expiration) {
      mode_detail_expiration = 0;
      datebuf[0] = '\0';
      timebuf[0] = '\0';
    }
    else {
      datebuf[sizeof(datebuf) - 1] = '\0';
      strftime(datebuf, sizeof(datebuf) - 1, "CW%W, %a,%b %e,%Y", t);
      if ('9' == datebuf[3]) {
        ++datebuf[2];
        datebuf[3] = '0';
      }
      else {
        ++datebuf[3];
      }
      // graphics_fill_circle(ctx, (GPoint){71, 157}, 4);
      timebuf[sizeof(timebuf) - 1] = '\0';
      strftime(timebuf, sizeof(timebuf) - 1, "%R", t);
    }
  }
}

static void handle_accelerometer(AccelAxisType axis, int32_t direction) {
  time_t t = time(NULL);
  mode_detail_expiration = t + TIMEOUT_TEN_SECONDS;
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(s_my_window));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  
  bm_moon = 0;
  i_drawn_index = -1;

  s_hour_tick = gpath_create(&HOUR_TICK_POINTS);
  gpath_move_to(s_hour_tick, center);

  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  gpath_move_to(s_hour_arrow, center);

  s_moon_layer = bitmap_layer_create(bounds);
  layer_set_update_proc(bitmap_layer_get_layer(s_moon_layer), update_proc);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_moon_layer));

  s_date_text_layer = text_layer_create(GRect(0, 0, 144, 28));
  text_layer_set_background_color(s_date_text_layer, GColorClear);
  text_layer_set_text_color(s_date_text_layer, GColorWhite);
  text_layer_set_text_alignment(s_date_text_layer, GTextAlignmentRight);
  layer_add_child(bitmap_layer_get_layer(s_moon_layer), text_layer_get_layer(s_date_text_layer));

  s_time_text_layer = text_layer_create(GRect(9, 46, 126, 60));
  text_layer_set_background_color(s_time_text_layer, GColorClear);
  text_layer_set_text_color(s_time_text_layer, GColorWhite);
  text_layer_set_text_alignment(s_time_text_layer, GTextAlignmentCenter);
  layer_add_child(bitmap_layer_get_layer(s_moon_layer), text_layer_get_layer(s_time_text_layer));

  s_date_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_14));
  text_layer_set_font(s_date_text_layer, s_date_text_font);

  s_time_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_56));
  text_layer_set_font(s_time_text_layer, s_time_text_font);

  datebuf[0] = '\0';
  text_layer_set_text(s_date_text_layer, datebuf);
  timebuf[0] = '\0';
  text_layer_set_text(s_time_text_layer, timebuf);
}

static void window_unload(Window *window) {
  fonts_unload_custom_font(s_time_text_font);
  fonts_unload_custom_font(s_date_text_font);
  text_layer_destroy(s_time_text_layer);
  text_layer_destroy(s_date_text_layer);
  bitmap_layer_destroy(s_moon_layer);
  gpath_destroy(s_hour_arrow);
  gpath_destroy(s_hour_tick);

  if (bm_moon) {
    gbitmap_destroy(bm_moon);
  }
}

void handle_init(void) {
  s_my_window = window_create();

  window_set_window_handlers(s_my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(s_my_window, true);
  tick_timer_service_subscribe(TICK_RESOLUTION, handle_minute_tick);
  accel_tap_service_subscribe(handle_accelerometer);
}

void handle_deinit(void) {
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
  window_destroy(s_my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
