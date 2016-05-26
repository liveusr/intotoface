#include <pebble.h>

#include "handle_battery.h"

static BitmapLayer *s_battery_layer;
static GBitmap *s_battery_bitmap;

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
void init_battery_layer(Layer *window_layer)
{
  init_battery(window_layer);
}

void deinit_battery_layer(void)
{
  deinit_battery();
}
