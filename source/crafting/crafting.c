#include "crafting.h"
#include "recipe.h"
#include "../utils/arraylist.h"

ArrayList anvilRecipes = {0};
ArrayList ovenRecipes = {0};
ArrayList furnaceRecipes = {0};
ArrayList workbenchRecipes = {0};


void crafting_init() {
	create_arraylist(&anvilRecipes);
	create_arraylist(&ovenRecipes);
	create_arraylist(&furnaceRecipes);
	create_arraylist(&workbenchRecipes);

	Recipe* recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, LANTERN);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &slime, 10);
	recipe_addCost(recipe, &glass, 4);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, OVEN);
	recipe_addCost(recipe, &stone, 15);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, FURNACE);
	recipe_addCost(recipe, &stone, 20);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, WORKBENCH);
	recipe_addCost(recipe, &wood, 20);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, CHEST);
	recipe_addCost(recipe, &wood, 20);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, ANVIL);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, BED);
	recipe_addCost(recipe, &wood, 10);
	recipe_addCost(recipe, &cloth, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, DOOR);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &boat);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &leatherArmor);
	recipe_addCost(recipe, &leather, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &bow);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &cloth, 3);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &arrow);
	recipe_addCost(recipe, &wood, 3);
	recipe_addCost(recipe, &stone, 2);
	recipe_addCost(recipe, &cloth, 1);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &woodPlank);
	recipe_addCost(recipe, &wood, 1);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &woodWallItem);
	recipe_addCost(recipe, &wood, 3);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &fishingRod);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &cloth, 3);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create_cnt(recipe, &torchItem, 4);
	recipe_addCost(recipe, &wood, 1);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &bucket);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &stoneWallItem);
	recipe_addCost(recipe, &stone, 2);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create_cnt(recipe, &stoneFloor, 2);
	recipe_addCost(recipe, &stone, 2);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create_cnt(recipe, &glassWallItem, 2);
	recipe_addCost(recipe, &glass, 1);
	recipe_addCost(recipe, &wood, 1);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create_cnt(recipe, &ladderItem, 3);
	recipe_addCost(recipe, &wood, 3);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create_cnt(recipe, &fenceItem, 2);
	recipe_addCost(recipe, &wood, 2);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &potItem);
	recipe_addCost(recipe, &dirt, 2);
	recipe_addCost(recipe, &sand, 2);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, BARREL);
	recipe_addCost(recipe, &wood, 10);
	recipe_addCost(recipe, &ironIngot, 2);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	furniturerecipe_create(recipe, SIGN);
	recipe_addCost(recipe, &wood, 4);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &tombstoneItem);
	recipe_addCost(recipe, &stone, 4);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SWORD, 0);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, AXE, 0);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, HOE, 0);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, PICKAXE, 0);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SHOVEL, 0);
	recipe_addCost(recipe, &wood, 5);
	arraylist_push(&workbenchRecipes, recipe);


	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SWORD, 1);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &stone, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, AXE, 1);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &stone, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, HOE, 1);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &stone, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, PICKAXE, 1);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &stone, 5);
	arraylist_push(&workbenchRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SHOVEL, 1);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &stone, 5);
	arraylist_push(&workbenchRecipes, recipe);


	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SWORD, 2);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, AXE, 2);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, HOE, 2);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, PICKAXE, 2);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SHOVEL, 2);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);


	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SWORD, 3);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, AXE, 3);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, HOE, 3);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, PICKAXE, 3);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SHOVEL, 3);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);


	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SWORD, 4);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, AXE, 4);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, HOE, 4);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, PICKAXE, 4);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	toolrecipe_create(recipe, SHOVEL, 4);
	recipe_addCost(recipe, &wood, 5);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &ironArmor);
	recipe_addCost(recipe, &ironIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &goldArmor);
	recipe_addCost(recipe, &goldIngot, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &gemArmor);
	recipe_addCost(recipe, &gem, 5);
	arraylist_push(&anvilRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &shears);
	recipe_addCost(recipe, &ironIngot, 2);
	arraylist_push(&anvilRecipes, recipe);


	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &ironIngot);
	recipe_addCost(recipe, &ironOre, 4);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&furnaceRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &goldIngot);
	recipe_addCost(recipe, &goldOre, 4);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&furnaceRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &glass);
	recipe_addCost(recipe, &sand, 4);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&furnaceRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &bread);
	recipe_addCost(recipe, &wheat, 4);
	arraylist_push(&ovenRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &cookedFish);
	recipe_addCost(recipe, &rawFish, 1);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&ovenRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &cookedSteak);
	recipe_addCost(recipe, &rawBeef, 1);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&ovenRecipes, recipe);

	recipe = malloc(sizeof(Recipe));
	resourcerecipe_create(recipe, &cookedPork);
	recipe_addCost(recipe, &rawPork, 1);
	recipe_addCost(recipe, &coal, 1);
	arraylist_push(&ovenRecipes, recipe);
}


void crafting_free() {
	for (int i = 0; i < anvilRecipes.size; ++i) {
		recipe_free(anvilRecipes.elements[i]);
		free(anvilRecipes.elements[i]);
	}
	arraylist_remove(&anvilRecipes);

	for (int i = 0; i < ovenRecipes.size; ++i) {
		recipe_free(ovenRecipes.elements[i]);
		free(ovenRecipes.elements[i]);
	}
	arraylist_remove(&ovenRecipes);

	for (int i = 0; i < furnaceRecipes.size; ++i) {
		recipe_free(furnaceRecipes.elements[i]);
		free(furnaceRecipes.elements[i]);
	}
	arraylist_remove(&furnaceRecipes);

	for (int i = 0; i < workbenchRecipes.size; ++i) {
		recipe_free(workbenchRecipes.elements[i]);
		free(workbenchRecipes.elements[i]);
	}
	arraylist_remove(&workbenchRecipes);
}
