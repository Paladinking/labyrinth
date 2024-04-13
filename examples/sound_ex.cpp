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
    InitAudioDevice();  
    std::vector<Sound> steps = { LoadSound("resources/Step1.mp3"), LoadSound("resources/Step2.mp3") , LoadSound("resources/Step3.mp3")};
    int step = 0;
    bool walking = false;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
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


