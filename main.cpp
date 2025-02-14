#include <iostream>
#include <stdio.h>      /* printf */
#include <math.h>       /* ceil */
#include <string>
#include <random>
#include "raylib.h"

#define DICT_SIZE 3

// g++ -I/usr/local/include -o main main.cpp -L/usr/local/lib -lraylib



// What it should be int the game?
/*
    a sound effect when a word is correctly typed (positive) 
    a sound effect when a typo is made (negative)
    a sound effect when the word has not been typed before the timer (negative)
    player has three lives when live count is zero than it is game over
    lives are lost when word is not typed timely or they are lost when there is a typo 
    the timer will get faster progressively the more words you type correctly the faster the timer gets 
    each letter of a word is highlighted as you type them when you make a typo it will be shown via an animation 
    when you lose a life it also will be shown via an animation
    the background will change the further you go
    timer will be represented as a bar that decreased as time goes on but will be restarted when a word is correctly typed
    words to type will be randomly selected
*/


const std::string Dict[DICT_SIZE] = {"interesting", "mother", "father"};

int main(void) 
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Typex");

    int timer = 60; 

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, DICT_SIZE-1);

    int rand = distr(gen);


    while (!WindowShouldClose()) {
        auto a = MeasureTextEx(GetFontDefault(), Dict[rand].c_str(), 120, 10);
        BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 45, RAYWHITE);              
            DrawText(Dict[rand].c_str(),SCREEN_WIDTH / 2 -  a.x / 2 - 10, SCREEN_HEIGHT / 2 - a.y / 2, 120, RAYWHITE);                 
        EndDrawing();
    }

    // Close the window and clean up resources
    CloseWindow();
    return 0;
}