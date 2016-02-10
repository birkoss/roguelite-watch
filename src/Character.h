#include <pebble.h>

typedef struct {
  GPoint positionBitmap;
  GPoint positionText;
  GTextAlignment alignmentText;
} CharacterStatus;

typedef struct {
  GBitmap *bitmap;
  GPoint position;
  int health;
  CharacterStatus status;
} Character;

Character* character_create(GBitmap *, GPoint);
void character_destroy(Character *);

void character_set_position(Character *, GRect, GTextAlignment);

void character_render(Character *, GContext *);
void character_status(Character *, GContext *, GFont, GBitmap *, GTextAlignment);
