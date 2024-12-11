#include <vector>
#include "raylib.h"
#include "world.h"
#include "pubsub.h"
#include "entity_view.h"
#include "player_entity.h"
#include "iomanip"
#include "sstream"
#include "iostream"

class Controller : public Subscriber
{
private:
    bool gameOver;
    float timer;
    World world;
    vector<EntityView *> views;

public:
    Controller() : gameOver(false) {}
    void drawTimer();
    void gameLoop();
    void receiveMessage(string channel, string message, void* data);
};

int main(void)
{
    Controller controller;
    controller.gameLoop();
    return 0;
}

int viewportWidth = 320;
int viewportHeight = 160;
int viewportX = 160;
int viewportY = 200;
int viewportDrawX = 0;
int viewportDrawY = 0;
const int screenWidth = 1200;
const int screenHeight = 1000;
void Controller::gameLoop()
{


    map<KeyboardKey, PlayerAction> keyMapping;

    keyMapping[KEY_W] = PlayerUp;
    keyMapping[KEY_A] = PlayerLeft;
    keyMapping[KEY_S] = PlayerDown;
    keyMapping[KEY_D] = PlayerRight;
    keyMapping[KEY_SPACE] = PlayerJump;

    PubSub::subscribe("entity", this);
    PubSub::subscribe("player", this);

    InitWindow(screenWidth, screenHeight, "Save the bear");
    SetTargetFPS(60);
    timer = 20.0f;  
    // Load the level

    float x = 320;
    float y = 400;
    world.addPlayer(x, y+38, 64, 76, Bear);
    int minX = 160;        
    int maxX = 10640 - 160;  
    int minY = 400;        
    int maxY = 500; 
    int obstacleWidth = 32;
    int obstacleHeight = 32;
    x = 160;
    for (y = 384; y <= 1000; y += 32) {
        world.addEntity(x, y, 32, 32, Obstacle);
    }
    //for (x = 320; x<= 500; x+= 32)
    //world.addEntity(x + 64, y - 32, 32, 32, Obstacle);

    //for (x = 400; x <= 700; x += 32)
    //    world.addEntity(x + 64, y - 64, 32, 32, Obstacle);

    y = 352;
    for (x=160; x <= 10640; x += 32)
        world.addEntity(x, y, 32, 32, Obstacle);
    y = 1000;
    for (x = 160; x <= 10640; x += 32)
        world.addEntity(x, y, 32, 32, Obstacle);
    float flameSpawnTimer = 2.0f;
    bool gameWon = false;
    // Go into the main loop
    while (!WindowShouldClose())
    {
        // Handle user input
        for (auto &i : keyMapping)
        {
            if (IsKeyPressed(i.first))
            {
                PlayerAction action = i.second;
                PubSub::publish("action", "player", &action);
            }
        }
        if (timer > 0)
        {
            timer -= GetFrameTime();  
        }
        else
        {
            timer = 0;  // doesn't go negative
            gameWon = true;
        }

        // Give the world a chance to progress
        if (!gameOver &&!gameWon) {
            flameSpawnTimer -= GetFrameTime();
            if (flameSpawnTimer <= 0.0f) {

                int numFlamesToSpawn = rand() % 5 + 20;
                float randomX;
                float randomY;
                // Spawn the random number of flames at random positions
                for (int i = 0; i < numFlamesToSpawn; i++) {
                    randomX = minX + rand() % (maxX - minX);
                    randomY = minY + rand() % (maxY - minY);
                    world.addObstacleObject(randomX, randomY, 50, 50, ObstacleObject, world.getPlayer());
                }

                world.addObstacleObject(randomX, randomY + 300, 64, 64, ObstacleObject, world.getPlayer());
                // Reset the flameSpawnTimer to the desired spawn interval
                flameSpawnTimer = 0.2f;
            }
            world.tick();
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the views
        for (EntityView *view : views)
            view->draw(viewportX, viewportY, viewportDrawX, viewportDrawY, viewportWidth, viewportHeight);
        if (gameOver) {
            const char* deathText = "You Died!";
            int fontSize = 60;
            int textWidth = MeasureText(deathText, fontSize);
            DrawText(deathText,
                (screenWidth - textWidth) / 2,
                screenHeight / 2,
                fontSize,
                RED);
        }
        else if (!gameWon) {
            drawTimer();
        }
        else {
            const char* winText = "You Won!";
            int fontSize = 60;
            int textWidth = MeasureTextEx(GetFontDefault(), winText, fontSize, 2).x;
            DrawTextEx(GetFontDefault(),
                winText,
                Vector2{ (float)(screenWidth - textWidth) / 2,
                       (float)(screenHeight - fontSize) / 2 },
                fontSize,
                2,
                GREEN);
        }
        EndDrawing();
    }

    CloseWindow();
}

void Controller::drawTimer()
{
    Color timerColor = BLACK;  
    int fontSize = 20;  
  
    std::ostringstream timerText;
    timerText << std::fixed << std::setprecision(2) << timer;

    std::string timerStr = timerText.str();
    int textWidth = MeasureText(timerStr.c_str(), fontSize);

    // Draw the timer in the top-right corner
    DrawText(timerStr.c_str(), screenWidth - textWidth - 10, 10, fontSize, timerColor);
}
void Controller::receiveMessage(string channel, string message, void* data)
{
    // On the "entity" channel there can be "new" and "delete" messages.
    
    if (channel == "entity" && message == "new")
    {
        // Create a view for the entity
        EntityView* view = new EntityView((Entity *) data);
        views.push_back(view);
    }

    if (channel == "entity" && message == "delete")
    {
        Entity* deletedEntity = (Entity*)data;
        if (deletedEntity->getType() == Bear) {
            gameOver = true;
        }
        // Try to find the view for the entity that's being deleted
        for (int i = 0; i < views.size(); i++)
        {
            if (views[i]->isViewFor((Entity*) data))
            {
                // Delete this view's memory
                delete views[i];
                // Delete it from the vector
                views.erase(views.begin() + i);
                // and stop looking, since we found the right view
                break;
            }
        }
    }

    if (channel == "player" && message == "location")
    {
        Vector2* position = (Vector2*)data;
        viewportX = position->x - viewportWidth / 2;
    }
}
