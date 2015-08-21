#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
int *count;

char *itoa(int num){
  static char buff[20] = {};
  int i = 0, temp_num = num, length = 0;
  char *string = buff;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished variable init.");

  // count how many characters in the number
  if(num >= 0) {

    APP_LOG(APP_LOG_LEVEL_DEBUG, "In IF loop.");

    while(temp_num) {

      APP_LOG(APP_LOG_LEVEL_DEBUG, "In WHILE loop.");

      temp_num /= 10;
      length++;
    }

    // assign the number to the buffer starting at the end of the number
    for(i = 0; i < length; i++) {

      APP_LOG(APP_LOG_LEVEL_DEBUG, "In FOR loop.");

      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  }else{
    return "Unsupported Number";
  }
  return string;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  /* This button currently does nothing. */
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  count++;
  text_layer_set_text(text_layer, itoa(*count));
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  count--;
  text_layer_set_text(text_layer, itoa(*count));
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  count = 0;


  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "0");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
