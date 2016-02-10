#include "Character.h"

Character* character_create(GBitmap *bitmap, GPoint position) {
  Character *c = malloc(sizeof(Character));
  c->position = position;
  c->bitmap = bitmap;
  c->health = 15;
  return c;
}

void character_destroy(Character *c) {
  free(c);
}

void character_render(Character *c, GContext *ctx) {
  GSize size = gbitmap_get_bounds(c->bitmap).size;
  graphics_draw_bitmap_in_rect(ctx, c->bitmap, GRect(c->position.x, c->position.y, size.w, size.h));
}

void character_status(Character *c, GContext *ctx, GFont font, GBitmap *bitmap, const GTextAlignment alignment) {
  GPoint positionBitmap = GPoint(0, 96);
  GSize sizeBitmap = gbitmap_get_bounds(bitmap).size;
  GPoint positionText = GPoint(0, positionBitmap.y-10);

  if( alignment == GTextAlignmentLeft ) {
    positionBitmap.x = PBL_IF_ROUND_ELSE(8,8);
    positionText.x = positionBitmap.x + sizeBitmap.w + 8;
  } else if( alignment == GTextAlignmentRight ) {
    positionBitmap.x = PBL_IF_ROUND_ELSE(123,123);
    positionText.x = positionBitmap.x - sizeBitmap.w - 8 - 5;
  }

  graphics_draw_bitmap_in_rect(ctx, bitmap, GRect(positionBitmap.x, positionBitmap.y, sizeBitmap.w, sizeBitmap.h));

  char buffer[15];
  snprintf(buffer, sizeof(buffer), "%d", c->health);
  graphics_draw_text(ctx, buffer, font, GRect(positionText.x, positionText.y, 20,5), GTextOverflowModeWordWrap, alignment, NULL);
}
