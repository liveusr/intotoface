#include <pebble.h>

#include "js_interface.h"
#include "handle_time.h"
#include "handle_weather.h"

static Window *s_main_window;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  //update_time();
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    //send_js_message(2);
  }
  //send_js_message(1);
}

static void show_launch_sreen(void)
{
  //update_time();
}

static void init_background_layer(Layer *window_layer)
{
  GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  //bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  //layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
}

static void deinit_background_layer(void)
{
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
}

static void main_window_load(Window *window) 
{
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);

  init_background_layer(window_layer);
  
  init_time_layer(window_layer);
  
  init_weather_layer(window_layer);
}

static void main_window_unload(Window *window) 
{
  deinit_time_layer();
  
  deinit_weather_layer();
  
  deinit_background_layer();
}


static void init() 
{
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set the background color
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register JS message interface
  js_interface_init();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

  // Make sure the watchface is displayed from the start
  show_launch_sreen();
}

static void deinit() 
{
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}
