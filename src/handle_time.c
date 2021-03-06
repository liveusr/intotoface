#include <pebble.h>

#include "handle_time.h"

static TextLayer *s_loc_time_h_layer;
static GFont s_loc_time_h_font;

static TextLayer *s_loc_time_m_layer;
static GFont s_loc_time_m_font;

static BitmapLayer *s_calendar_layer;
static GBitmap *s_calendar_bitmap;

static TextLayer *s_cal_month_layer;
static TextLayer *s_cal_date_layer;
static TextLayer *s_cal_day_layer;

static BitmapLayer *s_deu_flag_layer;
static GBitmap *s_deu_flag_bitmap;

static TextLayer *s_deu_time_h_layer;
static TextLayer *s_deu_time_m_layer;

static BitmapLayer *s_ind_flag_layer;
static GBitmap *s_ind_flag_bitmap;

static TextLayer *s_ind_time_h_layer;
static TextLayer *s_ind_time_m_layer;

void upstr(char *str, int len)
{
  int i = 0;
  
  while(i < len) {
    if(str[i] >= 'a' && str[i] <= 'z') {
      str[i] = str[i] - 32;
    }
    i++;
  }
}

void update_time(void) 
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char s_buf_loc_h[4];
  static char s_buf_loc_m[4];
  static char s_buf_deu_h[4];
  static char s_buf_deu_m[4];
  static char s_buf_ind_h[4];
  static char s_buf_ind_m[4];
  
  static char s_buf_month[4];
  static char s_buf_date[4];
  static char s_buf_day[4];
  
  // Write the current month, date and day into a buffer
  strftime(s_buf_month, sizeof(s_buf_month),"%b", tick_time);
  strftime(s_buf_date, sizeof(s_buf_date), "%d", tick_time);
  strftime(s_buf_day, sizeof(s_buf_day), "%a", tick_time);
  upstr(s_buf_month, sizeof(s_buf_month));
  upstr(s_buf_day, sizeof(s_buf_day));
  // Display this time on the TextLayer
  text_layer_set_text(s_cal_month_layer, s_buf_month);
  text_layer_set_text(s_cal_date_layer, s_buf_date);
  text_layer_set_text(s_cal_day_layer, s_buf_day);
  
  // Write the current hours and minutes into a buffer
  strftime(s_buf_loc_h, sizeof(s_buf_loc_h), clock_is_24h_style() ? "%H" : "%I", tick_time);
  strftime(s_buf_loc_m, sizeof(s_buf_loc_m), clock_is_24h_style() ? ":%M" : ":%M", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_loc_time_h_layer, s_buf_loc_h);
  text_layer_set_text(s_loc_time_m_layer, s_buf_loc_m);
  
  tick_time->tm_hour = (tick_time->tm_hour + 6) % 24;

  // Write the current hours and minutes into a buffer
  strftime(s_buf_deu_h, sizeof(s_buf_deu_h), clock_is_24h_style() ? "%H" : "%I", tick_time);
  strftime(s_buf_deu_m, sizeof(s_buf_deu_m), clock_is_24h_style() ? "%M" : "%M", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_deu_time_h_layer, s_buf_deu_h);
  text_layer_set_text(s_deu_time_m_layer, s_buf_deu_m);
  
  if(tick_time->tm_min >= 30) {
    tick_time->tm_hour = (tick_time->tm_hour + 4) % 24;
  }
  else {
    tick_time->tm_hour = (tick_time->tm_hour + 3) % 24;
  }
  tick_time->tm_min = (tick_time->tm_min + 30) % 60;
  
  // Write the current hours and minutes into a buffer
  strftime(s_buf_ind_h, sizeof(s_buf_ind_h), clock_is_24h_style() ? "%H" : "%I", tick_time);
  strftime(s_buf_ind_m, sizeof(s_buf_ind_m), clock_is_24h_style() ? "%M" : "%M", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_ind_time_h_layer, s_buf_ind_h);
  text_layer_set_text(s_ind_time_m_layer, s_buf_ind_m);
}

static void init_calendar(Layer *window_layer)
{
  GRect bounds = layer_get_bounds(window_layer);
  GRect image_bounds;

  // Create GBitmap
  s_calendar_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CALENDAR);
  image_bounds = gbitmap_get_bounds(s_calendar_bitmap);

  // Create BitmapLayer to display the GBitmap
  s_calendar_layer = bitmap_layer_create(GRect(bounds.size.w - image_bounds.size.w, 17, image_bounds.size.w, image_bounds.size.h));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_calendar_layer, s_calendar_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_calendar_layer));
}

static void deinit_calendar(void)
{
  // Destroy GBitmap
  gbitmap_destroy(s_calendar_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_calendar_layer);
}

static void init_deu_flag(Layer *window_layer)
{
  GRect bounds = layer_get_bounds(window_layer);
  GRect image_bounds;

  // Create GBitmap
  s_deu_flag_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DEU_FLAG);
  image_bounds = gbitmap_get_bounds(s_deu_flag_bitmap);

  // Create BitmapLayer to display the GBitmap
  s_deu_flag_layer = bitmap_layer_create(GRect(bounds.size.w - image_bounds.size.w, 85, image_bounds.size.w, image_bounds.size.h));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_deu_flag_layer, s_deu_flag_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_deu_flag_layer));
}

static void deinit_deu_flag(void)
{
  // Destroy GBitmap
  gbitmap_destroy(s_deu_flag_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_deu_flag_layer);
}

static void init_ind_flag(Layer *window_layer)
{
  GRect bounds = layer_get_bounds(window_layer);
  GRect image_bounds;

  // Create GBitmap
  s_ind_flag_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_IND_FLAG);
  image_bounds = gbitmap_get_bounds(s_ind_flag_bitmap);

  // Create BitmapLayer to display the GBitmap
  s_ind_flag_layer = bitmap_layer_create(GRect(bounds.size.w - image_bounds.size.w, 129, image_bounds.size.w, image_bounds.size.h));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_ind_flag_layer, s_ind_flag_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_ind_flag_layer));
}

static void deinit_ind_flag(void)
{
  // Destroy GBitmap
  gbitmap_destroy(s_ind_flag_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_ind_flag_layer);
}

void init_loc_time_h(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_loc_time_h_layer = text_layer_create(GRect(4, -8, 54, 72));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_loc_time_h_layer, GColorClear);
  text_layer_set_text_color(s_loc_time_h_layer, GColorWhite);
  text_layer_set_text(s_loc_time_h_layer, "29");
  text_layer_set_text_alignment(s_loc_time_h_layer, GTextAlignmentRight);

  // Create GFont
  s_loc_time_h_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LENGINEER_68));

  // Apply to TextLayer
  text_layer_set_font(s_loc_time_h_layer, s_loc_time_h_font);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_loc_time_h_layer));
}

void deinit_loc_time_h(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_loc_time_h_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_loc_time_h_font);
}

void init_loc_time_m(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_loc_time_m_layer = text_layer_create(GRect(54, 4, 60, 62));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_loc_time_m_layer, GColorClear);
  text_layer_set_text_color(s_loc_time_m_layer, GColorWhite);
  text_layer_set_text(s_loc_time_m_layer, ":42");
  text_layer_set_text_alignment(s_loc_time_m_layer, GTextAlignmentLeft);

  // Create GFont
  s_loc_time_m_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LENGINEER_56));

  // Apply to TextLayer
  text_layer_set_font(s_loc_time_m_layer, s_loc_time_m_font);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_loc_time_m_layer));
}

void deinit_loc_time_m(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_loc_time_m_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_loc_time_m_font);
}

void init_cal_month(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_cal_month_layer = text_layer_create(GRect(114, 21, 30, 20));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_cal_month_layer, GColorClear);
  text_layer_set_text_color(s_cal_month_layer, GColorWhite);
  text_layer_set_text(s_cal_month_layer, "MAY");
  text_layer_set_text_alignment(s_cal_month_layer, GTextAlignmentCenter);

  // Apply to TextLayer
  text_layer_set_font(s_cal_month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_cal_month_layer));
}

void deinit_cal_month(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_cal_month_layer);
}

void init_cal_date(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_cal_date_layer = text_layer_create(GRect(114, 29, 30, 30));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_cal_date_layer, GColorClear);
  text_layer_set_text_color(s_cal_date_layer, GColorBlack);
  text_layer_set_text(s_cal_date_layer, "05");
  text_layer_set_text_alignment(s_cal_date_layer, GTextAlignmentCenter);

  // Apply to TextLayer
  text_layer_set_font(s_cal_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_cal_date_layer));
}

void deinit_cal_date(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_cal_date_layer);
}

void init_cal_day(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_cal_day_layer = text_layer_create(GRect(114, 56, 30, 20));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_cal_day_layer, GColorClear);
  text_layer_set_text_color(s_cal_day_layer, GColorWhite);
  text_layer_set_text(s_cal_day_layer, "MON");
  text_layer_set_text_alignment(s_cal_day_layer, GTextAlignmentCenter);

  // Apply to TextLayer
  text_layer_set_font(s_cal_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_cal_day_layer));
}

void deinit_cal_day(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_cal_day_layer);
}

void init_deu_time_h(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_deu_time_h_layer = text_layer_create(GRect(113, 104, 16, 18));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_deu_time_h_layer, GColorClear);
  text_layer_set_text_color(s_deu_time_h_layer, GColorWhite);
  text_layer_set_text(s_deu_time_h_layer, "23");
  text_layer_set_text_alignment(s_deu_time_h_layer, GTextAlignmentRight);

  // Apply to TextLayer
  text_layer_set_font(s_deu_time_h_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_deu_time_h_layer));
}

void deinit_deu_time_h(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_deu_time_h_layer);
}

void init_deu_time_m(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_deu_time_m_layer = text_layer_create(GRect(130, 104, 16, 18));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_deu_time_m_layer, GColorClear);
  text_layer_set_text_color(s_deu_time_m_layer, GColorWhite);
  text_layer_set_text(s_deu_time_m_layer, "59");
  text_layer_set_text_alignment(s_deu_time_m_layer, GTextAlignmentLeft);

  // Apply to TextLayer
  text_layer_set_font(s_deu_time_m_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_deu_time_m_layer));
}

void deinit_deu_time_m(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_deu_time_m_layer);
}

void init_ind_time_h(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_ind_time_h_layer = text_layer_create(GRect(113, 148, 16, 18));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_ind_time_h_layer, GColorClear);
  text_layer_set_text_color(s_ind_time_h_layer, GColorWhite);
  text_layer_set_text(s_ind_time_h_layer, "23");
  text_layer_set_text_alignment(s_ind_time_h_layer, GTextAlignmentRight);

  // Apply to TextLayer
  text_layer_set_font(s_ind_time_h_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_ind_time_h_layer));
}

void deinit_ind_time_h(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_ind_time_h_layer);
}

void init_ind_time_m(Layer *window_layer)
{
  // Create the TextLayer with specific bounds
  s_ind_time_m_layer = text_layer_create(GRect(130, 148, 16, 18));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_ind_time_m_layer, GColorClear);
  text_layer_set_text_color(s_ind_time_m_layer, GColorWhite);
  text_layer_set_text(s_ind_time_m_layer, "59");
  text_layer_set_text_alignment(s_ind_time_m_layer, GTextAlignmentLeft);

  // Apply to TextLayer
  text_layer_set_font(s_ind_time_m_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_ind_time_m_layer));
}

void deinit_ind_time_m(void)
{
  // Destroy TextLayer
  text_layer_destroy(s_ind_time_m_layer);
}

void init_time_layer(Layer *window_layer)
{
  init_loc_time_h(window_layer);
  init_loc_time_m(window_layer);
  
  init_calendar(window_layer);
  init_cal_month(window_layer);
  init_cal_date(window_layer);
  init_cal_day(window_layer);
  
  init_deu_flag(window_layer);
  init_deu_time_h(window_layer);
  init_deu_time_m(window_layer);
  
  init_ind_flag(window_layer);
  init_ind_time_h(window_layer);
  init_ind_time_m(window_layer);
}

void deinit_time_layer(void)
{
  deinit_loc_time_h();
  deinit_loc_time_m();
  
  deinit_calendar();
  deinit_cal_month();
  deinit_cal_date();
  deinit_cal_day();
  
  deinit_deu_flag();
  deinit_deu_time_h();
  deinit_deu_time_m();
  
  deinit_ind_flag();
  deinit_ind_time_h();
  deinit_ind_time_m();
}
