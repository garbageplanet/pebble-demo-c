/*
 * main.c
 * Constructs a Window housing an output TextLayer to show data from 
 * either modes of operation of the accelerometer.
 */

#include <pebble.h>

#define TAP_NOT_DATA true
#define KEY_BUTTON_UP   0
#define KEY_BUTTON_DOWN 1

static Window *s_main_window;
static TextLayer *s_output_layer;
static int amount = 0;



static void send(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  dict_write_int(iter, key, &value, sizeof(int), true);

  app_message_outbox_send();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_output_layer, "Press the down button, for uploading");

  send(KEY_BUTTON_UP, 2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_output_layer, "Confirmation \nTrash uploaded !");

  send(KEY_BUTTON_DOWN, amount);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}




static void tap_handler(AccelAxisType axis, int32_t direction) {
  static char t_buffer[100];
  unsigned int umax = -1;
  if (direction > 0) {
    amount += umax + 1 + direction;
    snprintf(t_buffer, sizeof(t_buffer), "Amount of trash:%d \n Shake for more.", amount);
    text_layer_set_text(s_output_layer, t_buffer);
  }  
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "Start shaking to add trash");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  

}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  amount = 0;
  text_layer_destroy(s_output_layer);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
    // 
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  
  accel_tap_service_subscribe(tap_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);

  if (TAP_NOT_DATA) {
    accel_tap_service_unsubscribe();
  } else {
    accel_data_service_unsubscribe();
  }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

