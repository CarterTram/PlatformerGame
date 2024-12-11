#include <map>
#include <string>
#include "entity_view.h"
#include "textures.h"

struct SpriteSheetInfo
{
    string fileName;
    int numFrames = 0;
    int numRows = 0;
    map<EntityFacing, int> facings;
};

map<EntityType, SpriteSheetInfo> spriteMapping = {
    {Bear, {"resources/bear.png", 3, 4, {
        {Right, 1},
        {Up, 0},
        {Down, 2},
        {Left, 3}}
    }},
    {Obstacle, {"resources/wall.png", 1, 1, {
        {Right, 0}}
    }},
    {ObstacleObject, {"resources/obstacles.png", 6, 1, {
        {Right, 0},
        {Up, 0},
        {Down, 0},
        {Left, 0}}
    }} 
};

EntityView::EntityView(Entity* entity)
{
    this->entity = entity;
    texture = Textures::loadTexture(spriteMapping[entity->getType()].fileName);
    frame = 0;
    delay = 0;
}

bool EntityView::isViewFor(Entity* entity)
{
    return this->entity == entity;
}

void EntityView::draw(int vx, int vy, int vdx, int vdy, int vw, int vh)
{
    int animationDelay = 5;
    int numFrames = spriteMapping[entity->getType()].numFrames;
    int numRows = spriteMapping[entity->getType()].numRows;

    if (entity->getMoving())
    {
        delay--;
        if (delay == 0)
        {
            frame = (frame + 1) % numFrames;
            delay = animationDelay;
        }
    }
    else
    {
        frame = 0;
        delay = animationDelay;
    }

    int frameWidth = texture.width / numFrames;
    int frameHeight = texture.height / numRows;
    int row = spriteMapping[entity->getType()].facings[entity->getFacing()];
    Rectangle frameRec = { frame * frameWidth, row * frameHeight, frameWidth, frameHeight };
    float x = (entity->getX() - vx) + vdx;
    float y = (entity->getY() - vy) + vdy;

 
    if (entity->getType() == ObstacleObject) {
        static float rotation = 0.0f;
        rotation += 2.0f;
        Vector2 origin = { frameWidth / 2.0f, frameHeight / 2.0f };
        Rectangle destRec = { x + frameWidth / 2, y + frameHeight / 2, frameWidth, frameHeight };
        DrawTexturePro(texture, frameRec, destRec, origin, rotation, WHITE);
    }
    else {
        DrawTextureRec(texture, frameRec, { x, y }, WHITE);
    }
}