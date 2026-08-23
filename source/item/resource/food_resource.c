#include "resource.h"

void init_food_resource(Resource* resource, char* name, int sprite, int color, int heal, int staminaCost) {
	init_resource(resource, name, sprite, color);
	resource->type = RES_TYPE_FOOD;
	resource->food.heal = heal;
	resource->food.staminaCost = staminaCost;
}
