#include <pebble.h>

#include "handle_battery.h"

static BitmapLayer *s_battery_layer;
static GBitmap *s_battery_bitmap;

static Layer *s_battery_bar_layer;
static int s_battery_level;

static void battery_update_proc(Layer *layer, GContext *ctx) 
{
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar
  int width = 20 - (s_battery_level / 5);

  // Draw the bar
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(20 - width, 0, width, bounds.size.h), 0, GCornerNone);
}

void battery_callback(BatteryChargeState state) 
{
  // Record the new battery level
  s_battery_level = state.charge_percent;
  
  // Update meter
  layer_mark_dirty(s_battery_bar_layer);
}

static void init_battery(Layer *window_layer)
{
  GRect bounds = layer_get_bounds(window_layer);
  GRect image_bounds;

  // Create GBitmap
  s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  image_bounds = gbitmap_get_bounds(s_battery_bitmap);

  // Create BitmapLayer to display the GBitmap
  s_battery_layer = bitmap_layer_create(GRect(bounds.size.w - image_bounds.size.w - 1, 0, image_bounds.size.w, image_bounds.size.h));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_battery_layer));
}

static void deinit_battery(void)
{
  // Destroy GBitmap
  gbitmap_destroy(s_battery_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_battery_layer);
}

static void init_battery_bar(Layer *window_layer)
{
  // Create battery meter Layer
  s_battery_bar_layer = layer_create(GRect(118, 4, 20, 5));
  layer_set_update_proc(s_battery_bar_layer, battery_update_proc);

  // Add to Window
  layer_add_child(window_layer, s_battery_bar_layer);
}

static void deinit_battery_bar(void)
{
  layer_destroy(s_battery_bar_layer);
}

void init_battery_layer(Layer *window_layer)
{
  init_battery(window_layer);
  
  init_battery_bar(window_layer);
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
}

void deinit_battery_layer(void)
{
  deinit_battery();
  deinit_battery_bar();
}
