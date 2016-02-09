#define PBL_ROUND_WIDTH 180
#define PBL_TIME_WIDTH 144

#include <pebble.h>

static Window *window;
static Layer *layerBackground;

static GBitmap *bitmapWall;
static GBitmap *bitmapPlayer;
static GBitmap *bitmapGoblin;
static GBitmap *bitmapHealth;
static GBitmap *bitmapBackgroundTime;

static GFont fontHealth;
static GFont fontTime;

static TextLayer *layerTime;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  text_layer_set_text(layerTime, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits unit_changed) {
  update_time();
}

static void layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, bitmapWall, GRect(PBL_IF_ROUND_ELSE(0, -((PBL_ROUND_WIDTH-PBL_TIME_WIDTH)/2)), 88, PBL_ROUND_WIDTH, 80));

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, bitmapPlayer, GRect(PBL_IF_ROUND_ELSE(34,16), 120, 45, 48));
  graphics_draw_bitmap_in_rect(ctx, bitmapGoblin, GRect(PBL_IF_ROUND_ELSE(88,88), 123, 48, 45));

  graphics_draw_bitmap_in_rect(ctx, bitmapHealth, GRect(PBL_IF_ROUND_ELSE(8,8), 96, 13, 11));
  graphics_draw_bitmap_in_rect(ctx, bitmapHealth, GRect(PBL_IF_ROUND_ELSE(123,123), 96, 13, 11));

  graphics_draw_text(ctx, "8", fontHealth, GRect(30, 86, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, "5", fontHealth, GRect(98, 86, 20, 20), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);

  graphics_draw_bitmap_in_rect(ctx, bitmapBackgroundTime, GRect(0, 40, 144, 50));

  graphics_fill_rect(ctx, GRect(0, 0, 144, 40), 0, GCornerNone);
  graphics_draw_text(ctx, "Knight", fontHealth, GRect(6, -4, 144, 10), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

  graphics_draw_text(ctx, "Gold: 135", fontHealth, GRect(6, 13, 144, 10), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

}

static void window_load(Window *window) {
  bitmapWall = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WALL);
  bitmapPlayer = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAYER);
  bitmapGoblin = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GOBLIN);
  bitmapHealth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HEALTH);
  bitmapBackgroundTime = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_TIME);

  fontHealth = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FIVEPFIVE_20));
  fontTime = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FIVEPFIVE_48));

  Layer *layerWindow = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(layerWindow);

  layerTime = text_layer_create( GRect(0, PBL_IF_ROUND_ELSE(28,28), bounds.size.w, 50));
  text_layer_set_background_color(layerTime, GColorClear);
  text_layer_set_text_color(layerTime, GColorWhite);
  text_layer_set_text(layerTime, "00:00");
  text_layer_set_font(layerTime, fontTime);
  text_layer_set_text_alignment(layerTime, GTextAlignmentCenter);

  layerBackground = layer_create(bounds);
  layer_set_update_proc(layerBackground, layer_update_proc);
  layer_add_child(layerWindow, layerBackground);

  layer_add_child(layerWindow, text_layer_get_layer(layerTime));
}

static void window_unload(Window *window) {
  layer_destroy(layerBackground);

  gbitmap_destroy(bitmapWall);
  gbitmap_destroy(bitmapPlayer);
  gbitmap_destroy(bitmapGoblin);
  gbitmap_destroy(bitmapHealth);
  gbitmap_destroy(bitmapBackgroundTime);

  text_layer_destroy(layerTime);
  fonts_unload_custom_font(fontTime);

  fonts_unload_custom_font(fontHealth);
}

static void init(void) {
  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true);

  // Show the time at the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
