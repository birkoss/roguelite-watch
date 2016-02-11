#include "Character.h"

Character* character_create(GBitmap *bitmap) {
  Character *c = malloc(sizeof(Character));
  c->bitmap = bitmap;
  c->health = 15;
  c->damageTaken = 0;
  return c;
}

void character_destroy(Character *c) {
  free(c);
}

void character_blood(Character *c, GContext *ctx, GBitmap *bitmap) {
  GSize size = gbitmap_get_bounds(bitmap).size;
  graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(c->position.x, c->position.y, size.w, size.h));
}

void character_render(Character *c, GContext *ctx) {
  // layer_get_bounds(ctx);

  GSize size = gbitmap_get_bounds(c->bitmap).size;
   graphics_draw_bitmap_in_rect(ctx, c->bitmap, GRect(c->position.x, c->position.y, size.w, size.h));
}

void character_set_position(Character *c, GRect bounds, GTextAlignment alignment) {
  GSize sizeBitmap = gbitmap_get_bounds(c->bitmap).size;
  GSize sizeStatusBitmap = GSize(13, 11);

  // Set the character position
  GPoint position = GPoint(0, bounds.size.h - sizeBitmap.h);
  c->status.positionBitmap = GPoint(50, 96);
  c->status.positionText = GPoint(c->status.positionBitmap.x, c->status.positionBitmap.y - 10);

  if( alignment == GTextAlignmentLeft ) {
    position.x = 8;
    c->status.positionBitmap.x = PBL_IF_ROUND_ELSE(8,8);
    c->status.positionText.x = c->status.positionBitmap.x + sizeStatusBitmap.w + 8;
    c->status.alignmentText = GTextAlignmentLeft;
  } else if( alignment == GTextAlignmentRight ) {
    position.x = bounds.size.w - sizeBitmap.w - 8;
    c->status.positionBitmap.x = PBL_IF_ROUND_ELSE(123,123);
    c->status.positionText.x = c->status.positionBitmap.x - sizeStatusBitmap.w - 8 - 5;
    c->status.alignmentText = GTextAlignmentRight;
  } else if( alignment == GTextAlignmentCenter ) {
    position.x = (bounds.size.w - sizeBitmap.w) / 2;
    alignment = GTextAlignmentLeft;
    c->status.positionText.x = c->status.positionBitmap.x + sizeStatusBitmap.w + 8;
    c->status.alignmentText = GTextAlignmentLeft;
  }

  c->position = position;


  // Set the status position
}

void character_status(Character *c, GContext *ctx, GFont font, GBitmap *bitmap) {
  GSize sizeBitmap = gbitmap_get_bounds(bitmap).size;

  graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(c->status.positionBitmap.x, c->status.positionBitmap.y, sizeBitmap.w, sizeBitmap.h));

  char buffer[15];
  snprintf(buffer, sizeof(buffer), "%d", c->health);
  graphics_draw_text(ctx, buffer, font, GRect(c->status.positionText.x, c->status.positionText.y, 20,5), GTextOverflowModeWordWrap, c->status.alignmentText, NULL);
}
