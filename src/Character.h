#include <pebble.h>

typedef struct {
  GBitmap *bitmap;
  GPoint position;
  int health;
} Character;

Character* character_create(GBitmap *bitmap, GPoint position);
void character_destroy(Character *c);

void character_render(Character *c, GContext *ctx);
void character_status(Character *, GContext *, GFont, GBitmap *, const GTextAlignment);
