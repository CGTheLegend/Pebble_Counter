#include <pebble.h>

static Window *window;
static TextLayer *customFontTextLayer;
static Layer *draw_layer;
static GFont counterFont;
int count;

static void update_draw_proc(Layer *draw_layer, GContext *ctx){
  GRect minus;

  // readjust the position of the negative sign
  // automatically readjust deppending on certain incraments
  // draws an empty rectagle when count is 0
  minus = GRect(0, 0, 0, 0);
  if(count <= -1000){
    minus = GRect(5, 80, 18, 6);
  }if(count > -1000 && count <= -200){
    minus = GRect(10, 80, 18, 6);
  }else if(count > -200 && count <= -100){
    minus = GRect(15, 80, 18, 6);
  }else if(count > -100 && count <= -20){
    minus = GRect(20, 80, 18, 6);
  }else if(count > -20 && count <= -10){
    minus = GRect(25, 80, 18, 6);
  }else if(count > -10 && count <= -1){
    minus = GRect(20, 80, 18, 6);
  }

  graphics_fill_rect(ctx, minus, 0, GCornerNone);
}

char *itoa(int num){
  static char buff[20] = {};
  int i = 0, temp_num = num, length = 0;
  char *string = buff;

  // count how many characters in the number
  if(num > 0) {  // positive

    while(temp_num) {
      temp_num /= 10;
      length++;
    }

    // assign the number to the buffer starting at the end of the number
    for(i = 0; i < length; i++) {
      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  }else if(num < 0){          // negative
    temp_num = num;
    num = abs(temp_num);

    while(temp_num) {
      temp_num /= 10;
      length++;
    }

    // assign the number to the buffer starting at the end of the number
    for(i = 0; i < length; i++) {
      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  }else{
    string = "0";
  }
  return string;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  /* Resets the count */
  count = 0;
  text_layer_set_text(customFontTextLayer, itoa(count));
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  count++;
  text_layer_set_text(customFontTextLayer, itoa(count));
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  count--;
  text_layer_set_text(customFontTextLayer, itoa(count));
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

  // Text Layer
  customFontTextLayer = text_layer_create(GRect(0, 50, 144, 50));
  text_layer_set_background_color(customFontTextLayer, GColorClear);
  counterFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_XPED_48));
  layer_add_child(window_layer, text_layer_get_layer(customFontTextLayer));
  text_layer_set_font(customFontTextLayer, counterFont);
  text_layer_set_text(customFontTextLayer, "0");
  text_layer_set_text_alignment(customFontTextLayer, GTextAlignmentCenter);

  // Drawing Layer
  draw_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, draw_layer);
  layer_set_update_proc(draw_layer, update_draw_proc);
}

static void window_unload(Window *window) {
  fonts_unload_custom_font(counterFont);
  text_layer_destroy(customFontTextLayer);
  layer_destroy(draw_layer);
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

  draw_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(draw_layer, update_draw_proc);
  layer_set_update_proc(draw_layer, update_draw_proc);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
