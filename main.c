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
  const int screenWidth = 800;
  const int screenHeight = 450;
  const int startAngle = 270.0f;
  int endAngle = -90.0f;
  Timer timer = {0};
  int minutes = 25;
  int counter = 60 * minutes;
  const float increment = 360.0f / counter;
  char *buf;
  size_t sz;

  InitWindow(screenWidth, screenHeight, "Raylib");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (GetFrameTime() == 0.0f) {
      SetTimer(&timer, 1.0f);
    }
    UpdateTimer(&timer);

    if (isTimerDone(&timer) && counter > 0) {
      endAngle += increment;
      counter--;
      SetTimer(&timer, 1.0f);
    }

    if (counter == 0) {
      SetWindowFocused();
    }

    sz = snprintf(NULL, 0, "%02d:%02d", counter / 60, counter % 60);
    buf = (char *)calloc(sz + 1, sizeof(char));
    snprintf(buf, sz + 1, "%02d:%02d", counter / 60, counter % 60);

    Vector2 pos = MeasureTextEx(GetFontDefault(), buf, 40, 1);

    DrawText(buf, GetScreenWidth() / 2.0f - (pos.x / 2),
             GetScreenHeight() / 2.0f - (pos.y / 2), 40, BLACK);

    DrawRing((Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, 90,
             100, startAngle, endAngle, 360, RED);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
