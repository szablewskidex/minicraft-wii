#include "resource.h"

void init_plantable_resource(Resource* resource, char* name, int sprite, int color, TileID target, TileID* sources, int size) {
	init_resource(resource, name, sprite, color);
	resource->type = RES_TYPE_PLANTABLE;
	resource->plantable.sourceTilesSize = size;
	resource->plantable.sourceTiles = sources;
	resource->plantable.targetTile = target;
}
