#define RAYGUI_IMPLEMENTATION
#include "./raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_null.h>

typedef struct {
  double lifeTime;
} Timer;

void SetTimer(Timer *timer, double lifeTime) {
  if (timer != NULL)
    timer->lifeTime = lifeTime;
}

void UpdateTimer(Timer *timer) {
  if (timer != NULL)
    timer->lifeTime -= GetFrameTime();
}

bool isTimerDone(Timer *timer) {
  if (timer != NULL)
    return timer->lifeTime <= 0.0f;
  return false;
}

int main(void) {
  const int screenWidth = 225;
  const int screenHeight = 225;
  const int startAngle = 270.0f;
  bool start = false;
  float endAngle = -90.0f;
  Timer timer = {0};
  int minutes = 25;
  int counter = 60 * minutes;
  float increment = 360.0f / (60 * minutes);
  Color ringColor = RED;
  int cycle = 1;

  // Buffer to dynamically allocate text for the timer
  char *buf;
  size_t sz;

  InitWindow(screenWidth, screenHeight, "Pomodoro Timer");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (start) {
      UpdateTimer(&timer);
    }

    if (isTimerDone(&timer) && counter > 0 && start) {
      printf("Increment: %f, counter:  %d, endAngle: %f\n", increment, counter,
             endAngle);
      endAngle += increment;
      counter--;
      SetTimer(&timer, 1.0f);
    }

    if (counter == 0) {
      if (minutes == 25 && cycle < 4) {
        minutes = 5;
        counter = 60 * minutes;
        increment = 360.0f / (60 * minutes);
        ringColor = GREEN;
      } else if (minutes == 3 && cycle == 4) {
        minutes = 20;
        counter = 60 * minutes;
        increment = 360.0f / (60 * minutes);
        ringColor = GREEN;
        cycle = 0;
      } else {
        cycle++;
        minutes = 25;
        counter = 60 * minutes;
        increment = 360.0f / (60 * minutes);
        ringColor = RED;
      }
      SetWindowFocused();
      start = false;
      endAngle = -90.0f;
    }

    // Set the buffer to the formatted string
    sz = snprintf(NULL, 0, "%02d:%02d", counter / 60, counter % 60);
    buf = (char *)calloc(sz + 1, sizeof(char));
    snprintf(buf, sz + 1, "%02d:%02d", counter / 60, counter % 60);

    // Start/Pause button
    if (GuiButton((Rectangle){GetScreenWidth() / 2.0f - 10,
                              GetScreenHeight() / 2.0f + 20, 20, 20},
                  start ? GuiIconText(ICON_PLAYER_PAUSE, "")
                        : GuiIconText(ICON_PLAYER_PLAY, ""))) {
      start = !start;
    }

    // Reset button
    if (GuiButton((Rectangle){GetScreenWidth() / 2.0f - 10,
                              GetScreenHeight() / 2.0f + 50, 20, 20},
                  GuiIconText(ICON_UNDO, ""))) {
      counter = 60 * minutes;
      endAngle = -90.0f;
      start = false;
      SetTimer(&timer, 0.0f);
    }

    // Draw timer text
    Vector2 pos = MeasureTextEx(GetFontDefault(), buf, 40, 1);
    DrawText(buf, GetScreenWidth() / 2.0f - (pos.x / 2),
             GetScreenHeight() / 2.0f - (pos.y / 2) - 20, 40, BLACK);

    DrawRing((Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, 90,
             100, startAngle, endAngle, 360, ringColor);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
