#include "Character.h"

Character* character_create(GBitmap *bitmap, GPoint position) {
  Character *c = malloc(sizeof(Character));
  c->position = position;
  c->bitmap = bitmap;
  return c;
}

void character_destroy(Character *c) {
  free(c);
}

void character_render(Character *c, GContext *ctx) {
  GSize size = gbitmap_get_bounds(c->bitmap).size;
  graphics_draw_bitmap_in_rect(ctx, c->bitmap, GRect(c->position.x, c->position.y, size.w, size.h));
}
