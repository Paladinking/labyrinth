#include "raylib.h"
#include <iostream>
#include <vector>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

//Determines if the previos sound is done playing
bool IsPrevPlaying(std::vector<Sound> v, int s) {
    if(s < 1) {
        return IsSoundPlaying(v[v.size() - 1]);
    }else {
        return IsSoundPlaying(v[s - 1]);
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    bool pause = false;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 ballSpeed = { 5.0f, 4.0f };
    int ballRadius = 20;

    InitAudioDevice();  

    Music music = LoadMusicStream("resources/Rain.mp3");
    music.looping = false;
    float pitch = 1.0f;

    std::vector<Sound> steps = { LoadSound("resources/Step1.mp3"), LoadSound("resources/Step2.mp3") , LoadSound("resources/Step3.mp3")};
    int step = 0;
    bool walking = false;

    //PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
       /*
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        // Check walls collision for bouncing
        if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
        if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;

        */
        //Music
        UpdateMusicStream(music);      // Update music buffer with new stream data
        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_SPACE))
        {
            StopMusicStream(music);
            PlayMusicStream(music);
            pause = false;
        }
        
        if(IsKeyPressed(KEY_RIGHT)) {
            walking = true;
        }
        if(IsKeyReleased(KEY_RIGHT)) {
            walking = false;
            StopSound(steps[step]);
        }
        
        //Play sound from sound array
        if(walking && !IsPrevPlaying(steps, step)) {
            PlaySound(steps[step]);
            if(step >= steps.size() - 1) {
                step = 0;
            }else {
                step++;
            }
        }
           

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }
        //
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            DrawCircleV(ballPosition, (float)ballRadius, DARKPURPLE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


