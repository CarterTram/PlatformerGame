#include "ObstacleObjects.h"
#include <cstdlib>
#include <cmath>

ObstacleObjects::ObstacleObjects(int x, int y, int width, int height, EntityType type, PlayerEntity* player)
    : Entity(x, y, width, height, type), player(player)
{
    speedX = (float)(rand() % 11 - 5);
    speedY = -5.0f - (float)(rand() % 5); 

    // Add random direction changes
    changeDirectionTimer = (float)(rand() % 120 + 60);  // Change direction every 1-3 seconds
}

void ObstacleObjects::tick() {
    const float GRAVITY = 0.3f;
    const float MAX_FALL_SPEED = 8.0f;
    const float MAX_HORIZONTAL_SPEED = 6.0f;
    setMoving(true);
    speedY += GRAVITY;
    if (speedY > MAX_FALL_SPEED) {
        speedY = MAX_FALL_SPEED;
    }

    changeDirectionTimer--;
    if (changeDirectionTimer <= 0) {
        speedX += (float)(rand() % 5 - 2);  

        if (speedX > MAX_HORIZONTAL_SPEED) speedX = MAX_HORIZONTAL_SPEED;
        if (speedX < -MAX_HORIZONTAL_SPEED) speedX = -MAX_HORIZONTAL_SPEED;

        // Reset timer with random duration
        changeDirectionTimer = (float)(rand() % 120 + 60);

        // Occasionally give an upward boost
        if (rand() % 4 == 0) { 
            speedY = -5.0f - (float)(rand() % 3);
        }
    }

    setX(getX() + speedX);
    setY(getY() + speedY);
}

bool ObstacleObjects::handleCollisions() {
    bool collided = false;

    for (Entity* entity : collisions) {
        if (entity->getType() == Bear) {
            speedX = -speedX;
            speedY = -speedY;
            player->playerGotHit();
        }
        else if (entity->getType() == Obstacle) {
            // Bounce off walls and other obstacles
            speedX = -speedX + (float)(rand() % 3 - 1);  
            speedY = -speedY * 0.8f;  
            collided = true;
        }
    }

    collisions.clear();
    return collided;
}