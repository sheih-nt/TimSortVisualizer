#include "raylib.h"
#include "raygui.h"
#include "visualizer.hpp"
#include <vector>

struct InsertionSortState {
    int i = 1;
    int j = 0;
    bool inserting = false;
    int temp = 0;
    bool done = false;

    void reset() {
        i = 1;
        j = 0;
        inserting = false;
        done = false;
    }

    bool step(std::vector<int>& arr) {
        if (done) return true;

        if (!inserting) {
            if (i >= (int)arr.size()) {
                done = true;
                return true;
            }
            temp = arr[i];
            j = i - 1;
            inserting = true;
            return false;
        }

        if (j >= 0 && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
            return false;
        } else {
            arr[j + 1] = temp;
            i++;
            inserting = false;
            return false;
        }
    }
};

int main() {
    InitWindow(1280, 720, "TimSort Visualizer — Insertion Sort First");
    SetTargetFPS(60);

    AppState state = MENU;
    float sortSpeed = 20.0f;
    std::vector<int> original = LoadNumbersFromFile("numbers.txt");

    std::vector<int> arr;
    InsertionSortState sortState;
    float timer = 0.0f;

    while (!WindowShouldClose() && state != EXIT) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) {
            case MENU:
                sortState.reset();
                DrawText("TimSort Visualizer", 100, 80, 40, DARKGRAY);
                if (GuiButton({100, 200, 200, 50}, "Start (anim)")) {
                    arr = original;
                    state = SORTING;
                }
                if (GuiButton({100, 270, 200, 50}, "Start (step)")) {
                    arr = original;
                    state = STEP_MODE;
                }
                if (GuiButton({100, 340, 200, 50}, "About algo")) state = INFO;
                if (GuiButton({100, 410, 200, 50}, "Exit")) state = EXIT;
                DrawText("Speed:", 100, 500, 20, DARKGRAY);
                GuiSlider({170, 500, 200, 20}, "Slow", "Fast", &sortSpeed, 5.0f, 100.0f);
                break;

            case SORTING:
                timer += GetFrameTime();
                if (timer >= 1.0f / sortSpeed && !sortState.done) {
                    timer = 0.0f;
                    sortState.step(arr);
                }

                if (IsKeyPressed(KEY_ENTER)) state = MENU;
                state = RenderSorting(arr, sortState.j + 1, sortState.i, {}, false);
                break;

            case STEP_MODE:
                if (IsKeyDown(KEY_SPACE) && !sortState.done) {
                    sortState.step(arr);
                }

                if (IsKeyPressed(KEY_ENTER)) state = MENU;
                state = RenderSorting(arr, sortState.j + 1, sortState.i, {}, true);
                break;

            case INFO:
                state = RenderInfo();
                break;

            default:
                state = MENU;
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
