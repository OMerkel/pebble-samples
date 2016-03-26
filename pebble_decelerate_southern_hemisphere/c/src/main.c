/**
 * Pebble Watchface: Decelerate Southern Hemisphere
 *
 * The Decelerate Southern Hemisphere watchface shows a single hour hand only.
 * Midnight is on bottom of the clockface while Noon is indicated on top.
 * One rotation of the hour hand takes a whole day. An indicator for the
 * seconds is blinking on bottom of the watchface. If holding the Pebble watch
 * parallel over ground and turning it so the hour hand is pointing towards
 * the sun then top of the watchface will always point in north direction on
 * southern hemisphere. Additionally the current moon phase is shown for
 * southern hemisphere. Moon phase display is expected to work until end of
 * Unix Time (overflow on Jan 19th, 2038 03:14:07 GMT) at least as
 * implemented here.
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
#include "decelerate_analog.h"

#include <pebble.h>

static Window *window;
static Layer *s_simple_bg_layer;
static Layer *s_hands_layer;
static BitmapLayer *s_moon_phase;

static GPath *s_tick_paths[NUM_CLOCK_TICKS];
static GPath *s_hour_arrow;

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, GColorWhite);
  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_draw_filled(ctx, s_tick_paths[i]);
  }
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  bool second_even = 0 == (t->tm_sec % 2);

  // second indicator
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  if (second_even) {
    graphics_draw_circle(ctx, (GPoint){71, 157}, 4);
  } else {
    graphics_fill_circle(ctx, (GPoint){71, 157}, 4);    
  }

  // hour hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE * ((t->tm_hour + t->tm_min / 60.0) / 24.0 + 0.5));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  // struct tm *t = localtime(&now);

  uint32_t tmp = now - MOON_OFFSET_EPOCHE;
  tmp %= MOON_CYCLE;
  uint32_t index = (tmp * NUM_MOON_PHASES) / MOON_CYCLE;
  graphics_draw_bitmap_in_rect(ctx, MOON[index], GRect(0,0, 40, 40));
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_simple_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
  layer_add_child(window_layer, s_simple_bg_layer);

  s_moon_phase = bitmap_layer_create(GRect(52, 100, 40, 40));
  bitmap_layer_set_bitmap(s_moon_phase, MOON[4]);
  layer_set_update_proc(bitmap_layer_get_layer(s_moon_phase), date_update_proc);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_moon_phase));

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
}

static void window_unload(Window *window) {
  layer_destroy(s_hands_layer);
  bitmap_layer_destroy(s_moon_phase);
  for (int a = 0; a < NUM_MOON_PHASES; ++a) {
    gbitmap_destroy(MOON[a]);
  }
  layer_destroy(s_simple_bg_layer);  
}

static void init() {
  for (int a = 0; a < NUM_MOON_PHASES; ++a) {
    MOON[a] = gbitmap_create_with_resource(MOON_ID[a]);
  }  

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // init hand paths
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_hour_arrow, center);

  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    s_tick_paths[i] = gpath_create(&ANALOG_BG_POINTS[i]);
  }

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit() {
  gpath_destroy(s_hour_arrow);

  for (int i = 0; i < NUM_CLOCK_TICKS; ++i) {
    gpath_destroy(s_tick_paths[i]);
  }

  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
