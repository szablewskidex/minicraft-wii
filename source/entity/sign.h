#ifndef SIGN_H
#define SIGN_H
#include "furniture.h"

typedef struct _Sign {
	Furniture furniture;
	int signIndex;
} Sign;

void sign_create(Sign* sign);
char sign_use(Sign* sign, struct _Player* player, int attackDir);
void sign_free(Sign* sign);

#endif // SIGN_H
