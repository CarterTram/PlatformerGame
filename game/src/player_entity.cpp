#include <iostream>
#include "raylib.h"
#include "player_entity.h"
#include "world.h"

#include <iostream>

PlayerEntity::PlayerEntity(float x, float y, int width, int height, EntityType type)
	: Entity(x, y, width, height, type)
{
	PubSub::subscribe("action", this);

	movementCountdown = 0;
	targetX = 0;
	targetY = 0;
	speedX = 0;
	speedY = 0;
}

void PlayerEntity::tick()
{
	if (isDead){
		PubSub::publish("entity", "delete", this);
		return;
	}
	if (movementCountdown > 0)
	{
		movementCountdown--;

		if (movementCountdown == 0)
		{
			setMoving(false);
			setX(targetX);
		}
		else
		{
			setX(getX() + speedX);
		}
	}

	speedY += 1;

	setY(getY() + speedY);
	setFalling(true);

	Vector2 position = { getX(), getY() };
	PubSub::publish("player", "location", &position);
}

void PlayerEntity::receiveMessage(string channel, string message, void* data)
{
	if (channel == "action" && message == "player")
	{
		PlayerAction* action = (PlayerAction*)data;

		if (*action == PlayerRight)
		{
			targetX = getX() + 64;
			targetY = getY();
			speedX = 64.0 / 8.0;
			setMoving(true);
			setFacing(Right);
			movementCountdown = 8;
		}

		if (*action == PlayerLeft)
		{
			targetX = getX() - 64;
			targetY = getY();
			speedX = -64.0 / 8.0;
			setMoving(true);
			setFacing(Left);
			movementCountdown = 8;
		}

		if (*action == PlayerJump && !getJumping())
		{
			speedY = -15;
			setJumping(true);
		}

	}
}

bool PlayerEntity::handleCollisions()
{
	for (Entity* entity : collisions)
	{
		if (entity->getType() == Obstacle)
		{
			int xDistance = abs(getX() - entity->getX());
			int yDistance = abs(getY() - entity->getY());

			if (xDistance > yDistance)
			{
				setMoving(false);
				movementCountdown = 0;

				setX(getX() - speedX);
			}
			else
			{
				if (getFalling())
				{
					setFalling(false);
					setJumping(false);
					setY(getY() - speedY);
					speedY = 0;
				}
			}
		}
	}

	collisions.clear();
	return false;
}
void PlayerEntity::playerGotHit() {
	isDead = true;
	cout << "rest in peace" << endl;
}
