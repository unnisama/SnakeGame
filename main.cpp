#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <stdio.h>

struct Point{
    Vector2 pos;
};

Color GetRandomColor(){
    return {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
}

int main(void)
{
    int score = 0;
    float linewidth = 12.0f;
    float circlerad = 2.7f;
    float foodrad = 15.0f;
    float headrad = 10.0f;
    
    int initpoints = 4;
    float length = 20.0f;
    std::vector<Point> points;
    
    const int screenWidth = 700;
    const int screenHeight = 400;
    Vector2 snakepos = {screenWidth/2.0f, screenHeight/2.0f};

    SetConfigFlags(ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Hi");

    SetTargetFPS(60);

    Vector2 food = {static_cast<float>(GetRandomValue(foodrad, screenWidth-foodrad)), static_cast<float>(GetRandomValue(foodrad, screenHeight-foodrad))};
    Color foodc = BLUE;

    points.push_back({snakepos});
    for(int i = 0; i < initpoints; i++){
        points.push_back({{1.0f, 1.0f}});
    }

    points.push_back({{1.0f, 1.0f}});

  
    while (!WindowShouldClose())    
    {
        if(IsKeyDown(KEY_UP)){
            snakepos.y += -1.0f;
            snakepos.y = Clamp(snakepos.y, headrad, screenHeight-headrad);
            points[0].pos = snakepos;
        }else if(IsKeyDown(KEY_DOWN)){
            snakepos.y += 1.0f;
            snakepos.y = Clamp(snakepos.y, headrad, screenHeight-headrad);
            points[0].pos = snakepos;
        }
        if(IsKeyDown(KEY_RIGHT)){
            snakepos.x += 1.0f;
            snakepos.x = Clamp(snakepos.x, headrad, screenWidth-headrad);
            points[0].pos = snakepos;
        }else if(IsKeyDown(KEY_LEFT)){
            snakepos.x += -1.0f;
            snakepos.x = Clamp(snakepos.x, headrad, screenWidth-headrad);
            points[0].pos = snakepos;
        }
        
        if (CheckCollisionCircles(snakepos, headrad, food, foodrad)){
            points.push_back({{1.0f, 1.0f}});
            score += 1;
            food = food = {static_cast<float>(GetRandomValue(foodrad, screenWidth-foodrad)), static_cast<float>(GetRandomValue(foodrad, screenHeight-foodrad))};
        }

        BeginDrawing();
            DrawFPS(10, 10);
            DrawText(TextFormat("Score: %d", score), 10, 30, 5, GREEN);

            ClearBackground({0x18, 0x18, 0x18, 255});
            

            for(int i = 0; i < points.size()-1; i++){
                auto a = points[i+1].pos;
                auto b = points[i].pos;
                if(i == 0){
                    a = Vector2Scale(Vector2Normalize(Vector2Subtract(b, a)), 5.0f);
                }else{
                    a = Vector2Scale(Vector2Normalize(Vector2Subtract(b, a)), length);
                }
                a = Vector2Subtract(b, a);
                points[i+1].pos = Vector2{a.x, a.y};
            }
            
            DrawCircleV(food, foodrad, foodc);
            DrawSplineCatmullRom((Vector2 *)points.data(), points.size(), linewidth, GREEN);
            DrawCircleV(snakepos, headrad, GRAY);

            //DrawLineEx(points[0].pos, points[1].pos, linewidth, GREEN);

        EndDrawing();
 
    }
    CloseWindow();
    return 0;
}