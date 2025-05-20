

#include "raylib.h"
#include <cmath>
#include <iostream>

using namespace std;

struct Controls
{
    int keyUp;
    int keyDown;
    int keyLeft;
    int keyRight;
    int keyDrift;
};

class Car
{
public:
    Vector2 pos;
    Vector2 vel;
    float acc;
    float angle;
    float speed;
    float normalFriction;
    float friction;
    float turningSpeed;

    float drifting;
    float drifFriction;
    float driftAngle;
    float targetdriftAngle;
    Vector2 slideVel;

    Controls controls;
    bool trackMouse; // it's not used yet, but it's here for future use

    Color color;

    Car(float x, float y, Controls controls, Color color, bool trackMouse = false) : pos{x, y},
                                                                                     vel{0, 0},
                                                                                     acc(0.09f),
                                                                                     angle(0),
                                                                                     speed(0),
                                                                                     normalFriction(0.98f),
                                                                                     friction(normalFriction),
                                                                                     turningSpeed(0.01f),

                                                                                     drifting(false),
                                                                                     drifFriction(0.97f),
                                                                                     driftAngle(0),
                                                                                     targetdriftAngle(0),
                                                                                     slideVel{0, 0},

                                                                                     controls(controls),
                                                                                     trackMouse(trackMouse),

                                                                                     color(color)
    {
    }

    void Update()
    {
        if (IsKeyDown(controls.keyUp))
            speed += acc;
        if (IsKeyDown(controls.keyDown))
            speed -= acc * 0.5f;

        float turnDir = 0.0f;
        float effectiveTurnSpeed = turningSpeed / (0.8f + speed * 0.05f);
        if (IsKeyDown(controls.keyRight))
        {
            angle += effectiveTurnSpeed * speed * 1.2f;
            turnDir = -1.0f;
        }
        if (IsKeyDown(controls.keyLeft))
        {
            angle -= effectiveTurnSpeed * speed * 1.2f;
            turnDir = 1.0f;
        }

        targetdriftAngle = drifting ? angle + 0.2f * turnDir : angle;
        driftAngle += (targetdriftAngle - driftAngle) * 0.1f;

        if (drifting)
        {
            slideVel.x = slideVel.x * 0.9f + (speed * 0.9f * cos(driftAngle)) * 0.1f;
            slideVel.y = slideVel.y * 0.9f + (speed * 0.9f * sin(driftAngle)) * 0.1f;
        }
        else
        {
            Vector2 targetVel = {speed * cos(angle), speed * sin(angle)};
            slideVel.x = slideVel.x * 0.9f + targetVel.x * 0.1f;
            slideVel.y = slideVel.y * 0.9f + targetVel.y * 0.1f;
        }

        // vel.x = slideVel.x;
        // vel.y = slideVel.y;

        vel.x = drifting ? slideVel.x : slideVel.x * speed * 0.3f;
        vel.y = drifting ? slideVel.y : slideVel.y * speed * 0.3f;

        pos.x += vel.x;
        pos.y += vel.y;

        if (drifting)
        {
            if (friction < drifFriction)
            {
                friction += 0.01f;
            }
            else
            {
                friction = drifFriction;
            }
        }
        else
        {
            if (friction > normalFriction)
            {
                friction -= 0.01f;
            }
            else
            {
                friction = normalFriction;
            }
        }

        speed *= friction;

        drifting = IsKeyDown(controls.keyDrift);

        if (pos.x < 0)
            pos.x += 800;
        if (pos.x > 800)
            pos.x -= 800;
        if (pos.y < 0)
            pos.y += 600;
        if (pos.y > 600)
            pos.y -= 600;
    }

    void Draw()
    {
        DrawRectanglePro({pos.x, pos.y, 40, 20}, {20, 10}, angle * RAD2DEG, color);
    }
};

int main()
{
    Vector2 windowSizes = {800, 600};
    string title = "Car Drifting Physics";
    InitWindow(windowSizes.x, windowSizes.y, title.c_str());
    SetTargetFPS(60);

    Car car = Car(200, 150, {KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE}, BLUE);
    // Car car2 = Car(600, 450, {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER}, RED);
    Car car2 = Car(600, 450, {KEY_KP_5, KEY_KP_2, KEY_KP_1, KEY_KP_3, KEY_KP_0}, RED);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        car.Draw();
        car.Update();

        car2.Draw();
        car2.Update();

        DrawText(title.c_str(), 20, 20, 30, BLACK);

        DrawText("Handbrake Car 1", 20, 60, 20, car.drifting ? RED : BLUE);
        DrawText("Handbrake Car 2", 20, 80, 20, car2.drifting ? RED : BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
