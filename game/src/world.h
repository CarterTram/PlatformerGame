#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "entity.h"
#include "player_entity.h"
#include "ObstacleObjects.h"

using namespace std;

enum PlayerAction {CreateEnemy, PlayerUp, PlayerDown, PlayerRight, PlayerLeft, PlayerJump};

class World
{
private:
	vector<Entity*> entities;
	vector<Entity*> collisions;
	vector<Entity*> deletions;
	PlayerEntity* player;
	void checkCollision(Entity* entity, Entity* entity2);

public:
	World();
	void clearWorld();
	void addEntity(int x, int y, int width, int height, EntityType type);
	void addPlayer(int x, int y, int width, int height, EntityType type);

	void addObstacleObject(int x, int y, int width, int height, EntityType type, PlayerEntity* player);

	PlayerEntity* getPlayer();

	void tick();
};

#endif
