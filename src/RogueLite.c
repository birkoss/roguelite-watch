#include <pebble.h>

#include "RogueLite.h"
#include "Character.h"

static Window *window;
static Layer *layerBackground;

static GBitmap *bitmapWall;
static GBitmap *bitmapPlayer;
static GBitmap *bitmapGoblin;
static GBitmap *bitmapHealth;
static GBitmap *bitmapBackgroundTime;
static GBitmap *bitmapBattery;
static GBitmap *bitmapBlood;

static GFont fontHealth;
static GFont fontTime;

static TextLayer *layerTime;

static Character *player;
static Character *enemy;

static GameState state;

static GRect screenDimension;

static void update_game() {
  switch( state ) {
    case waiting:
      break;
    case wondering:
      if( rand() % 10 < 5 ) {
        character_set_position(player, screenDimension, GTextAlignmentLeft);

        enemy = character_create(bitmapGoblin);
        character_set_position(enemy, screenDimension, GTextAlignmentRight);

        state = player_turn;
      }
      break;
    case player_turn:
      if( player->health <= 0 ) {
        state = waiting;
      } else {
        if( rand() % 10 >= 2 ) {
          enemy->damageTaken = 1;
          enemy->health -= enemy->damageTaken;
          if( enemy->health <= 0 ) {
            enemy->health = 0;
          }
          layer_mark_dirty(layerBackground);
          state = (enemy_turn);
        } else {
          state = enemy_turn;
        }
      }
      break;
    case enemy_turn:
      if( enemy->health <= 0 ) {
        enemy = NULL;
        character_set_position(player, screenDimension, GTextAlignmentCenter);
        state = wondering;
      } else {
        if( rand() % 10 >= 2 ) {
          player->damageTaken = 1;
          player->health -= player->damageTaken;
          if( player->health <= 0 ) {
            player->health = 0;
          }
          layer_mark_dirty(layerBackground);
          state = player_turn;
        } else {
          state = player_turn;
        }
      }
      break;
  }
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  text_layer_set_text(layerTime, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits unit_changed) {
  update_time();
  update_game();
}

static void layer_update_proc(Layer *layer, GContext *ctx) {
  GSize bitmapSize = gbitmap_get_bounds(bitmapWall).size;

  graphics_draw_bitmap_in_rect(ctx, bitmapWall, GRect(-((bitmapSize.w - screenDimension.size.w)/2), 88, bitmapSize.w, bitmapSize.h));

  // Player and enemy
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  character_render(player, ctx);
  character_status(player, ctx, fontHealth, bitmapHealth);
  if( player->damageTaken > 0 ) {
    character_blood(player, ctx, bitmapBlood);
    player->damageTaken = 0;
    layer_mark_dirty(layerBackground);
  }

  if( enemy != NULL ) {
    character_render(enemy, ctx);
    character_status(enemy, ctx, fontHealth, bitmapHealth);
    if( enemy->damageTaken > 0 ) {
      character_blood(enemy, ctx, bitmapBlood);
      enemy->damageTaken = 0;
      layer_mark_dirty(layerBackground);
    }
  }

  // Time Background
  graphics_draw_bitmap_in_rect(ctx, bitmapBackgroundTime, GRect(0, 40, 144, 50));

  // Top Bar
  graphics_fill_rect(ctx, GRect(0, 0, 144, 40), 0, GCornerNone);
  graphics_draw_text(ctx, "Knight", fontHealth, GRect(6, -4, 144, 10), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, "Gold: 135", fontHealth, GRect(6, 13, 144, 10), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

  // Battery
  graphics_draw_bitmap_in_rect(ctx, bitmapBattery, GRect(120, 6, 18, 10));
  graphics_context_set_fill_color(ctx, GColorGreen);
  graphics_fill_rect(ctx, GRect(123, 9, 10, 4), 0, GCornerNone);
}

static void window_load(Window *window) {
  bitmapWall = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WALL);
  bitmapPlayer = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAYER);
  bitmapGoblin = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_GOBLIN);
  bitmapHealth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HEALTH);
  bitmapBackgroundTime = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_TIME);
  bitmapBattery = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  bitmapBlood = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLOOD);

  fontHealth = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FIVEPFIVE_20));
  fontTime = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FIVEPFIVE_48));

  Layer *layerWindow = window_get_root_layer(window);
  screenDimension = layer_get_bounds(layerWindow);

  layerTime = text_layer_create( GRect(0, PBL_IF_ROUND_ELSE(28,28), screenDimension.size.w, 50));
  text_layer_set_background_color(layerTime, GColorClear);
  text_layer_set_text_color(layerTime, GColorWhite);
  text_layer_set_text(layerTime, "00:00");
  text_layer_set_font(layerTime, fontTime);
  text_layer_set_text_alignment(layerTime, GTextAlignmentCenter);

  layerBackground = layer_create(screenDimension);
  layer_set_update_proc(layerBackground, layer_update_proc);
  layer_add_child(layerWindow, layerBackground);

  layer_add_child(layerWindow, text_layer_get_layer(layerTime));

  player = character_create(bitmapPlayer);
  character_set_position(player, screenDimension, GTextAlignmentCenter);

  // enemy = character_create(bitmapGoblin);
  // character_set_position(enemy, screenDimension, GTextAlignmentRight);
}

static void window_unload(Window *window) {
  layer_destroy(layerBackground);

  gbitmap_destroy(bitmapWall);
  gbitmap_destroy(bitmapPlayer);
  gbitmap_destroy(bitmapGoblin);
  gbitmap_destroy(bitmapHealth);
  gbitmap_destroy(bitmapBackgroundTime);
  gbitmap_destroy(bitmapBattery);
  gbitmap_destroy(bitmapBlood);

  text_layer_destroy(layerTime);
  fonts_unload_custom_font(fontTime);

  fonts_unload_custom_font(fontHealth);

  character_destroy(player);
  character_destroy(enemy);
}

static void init(void) {
  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true);

  state = wondering;

  // Show the time at the start
  update_time();

  // update_game();

  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  // tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
