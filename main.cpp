#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <stdio.h>

int score = 0;
float linewidth = 12.0f;
float circlerad = 2.7f;
float foodrad = 15.0f;
float headrad = 10.0f;

int initpoints = 4;
float length = 4.0f;

const int screenWidth = 700;
const int screenHeight = 600;

float speed = 40.0f;

Color GetRandomColor(){
    return {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
}

void ClampPosition(Vector2& pos){
    pos.y = Clamp(pos.y, headrad, screenHeight-headrad);
    pos.x = Clamp(pos.x, headrad, screenWidth-headrad);
}

int main(void)
{
    float neg = -1.57f;
    float pos = 1.57f;

    float angle = 0.0f;
    std::vector<Vector2> points;
    Vector2 direction = {1.0f, 0.0f};
    Vector2 center = {screenWidth/2.0f, screenHeight/2.0f};
    
    Vector2 snakepos = center;

    SetConfigFlags(ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Hi");

    SetTargetFPS(60);

    Vector2 food = {static_cast<float>(GetRandomValue(foodrad, screenWidth-foodrad)), static_cast<float>(GetRandomValue(foodrad, screenHeight-foodrad))};
    Color foodc = BLUE;

    points.push_back({snakepos});
    for(int i = 0; i < initpoints; i++){
        points.push_back({1.0f, 0.0f});
        points.push_back({1.0f, 0.0f});
        points.push_back({1.0f, 0.0f});
        points.push_back({1.0f, 0.0f});
    }

    points.push_back({1.0f, 0.0f});
    Image im = LoadImage("background.png");
    Texture2D ground = LoadTextureFromImage(im);
    UnloadImage(im);

    while (!WindowShouldClose())    
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            direction = Vector2Normalize(Vector2Subtract(GetMousePosition(), snakepos));
            snakepos = Vector2Add(snakepos, Vector2Scale(direction, speed*1.0f*GetFrameTime()));
        }
        snakepos = Vector2Add(snakepos, Vector2Scale(direction, speed*GetFrameTime()));
        
        ClampPosition(snakepos);
        points[0] = snakepos;
        
        if (CheckCollisionCircles(snakepos, headrad, food, foodrad)){
            points.push_back({1.0f, 0.0f});
            points.push_back({1.0f, 0.0f});
            points.push_back({1.0f, 0.0f});
            points.push_back({1.0f, 0.0f});
            score += 1;
            food = {static_cast<float>(GetRandomValue(foodrad, screenWidth-foodrad)), static_cast<float>(GetRandomValue(foodrad, screenHeight-foodrad))};
            speed += 4.0f;
        }
        BeginDrawing();
            DrawTexture(ground, 0, 0, WHITE);
            DrawFPS(10, 10);
            DrawText(TextFormat("Score: %d", score), 10, 30, 5, GREEN);

            ClearBackground({0x18, 0x18, 0x18, 255});
            
            for(int i = 0; i < points.size()-1; i++){
                auto a = points[i+1];
                auto b = points[i];
                Vector2 line;
                if(i == 0){
                    line = Vector2Normalize(Vector2Subtract(b, a));
                    line = Vector2Scale(line, length-1.0f);
                }else{
                    line = Vector2Scale(Vector2Normalize(Vector2Subtract(b, a)), length);
                }
                a = Vector2Subtract(b, line);
                points[i+1] = Vector2{a.x, a.y};
            }
            
            DrawCircleV(food, foodrad, foodc);
            DrawSplineCatmullRom((Vector2 *)points.data(), points.size(), linewidth, PURPLE);
            DrawCircleV(snakepos, headrad, GRAY);

        EndDrawing();
       
    }
    UnloadTexture(ground);
    CloseWindow();
    return 0;
}