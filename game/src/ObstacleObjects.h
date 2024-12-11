#ifndef OBSTACLEOBJECTS_H
#define OBSTACLEOBJECTS_H
#include "raylib.h"
#include "entity.h"
#include "player_entity.h"

class ObstacleObjects : public Entity {
private:
    float speedX;
    float speedY;
    float changeDirectionTimer;  // Timer for direction changes
    PlayerEntity* player;

public:
    ObstacleObjects(int x, int y, int width, int height, EntityType type, PlayerEntity* player);

    void tick();
    bool handleCollisions();
};
#endif