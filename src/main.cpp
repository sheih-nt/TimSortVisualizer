#include "raylib.h"
#include "raygui.h"
#include "visualizer.hpp"
#include <vector>
#include <string>
#include <ctime>  // Добавлено для time()

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
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "TimSort Visualizer");
    SetTargetFPS(60);

    // Состояния приложения
    AppState state = MENU;
    float sortSpeed = 20.0f;
    bool useRandomData = true;
    int randomCount = 50;
    int randomMaxValue = 100;
    std::string currentFile = "";

    // Данные для сортировки
    std::vector<int> original;
    std::vector<int> arr;
    InsertionSortState sortState;
    float timer = 0.0f;

    // Инициализация случайных чисел
    SetRandomSeed(time(NULL));
    original = GenerateRandomNumbers(randomCount, randomMaxValue);

    while (!WindowShouldClose() && state != EXIT) {
        // Обработка Drag & Drop
        if (IsFileDroppedCustom() && state == MENU) {
            std::string path = GetDroppedFilePath();
            if (!path.empty()) {
                currentFile = path;
                original = LoadNumbersFromFile(currentFile);
                useRandomData = false;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) {
            case MENU: {
                sortState.reset();

                // Заголовок
                DrawText("TimSort Visualizer", 100, 80, 40, DARKGRAY);

                // Левая панель - управление
                if (GuiButton({100, 200, 200, 50}, "Start (animation)")) {
                    arr = original;
                    state = SORTING;
                }
                if (GuiButton({100, 270, 200, 50}, "Start (step mode)")) {
                    arr = original;
                    state = STEP_MODE;
                }
                if (GuiButton({100, 340, 200, 50}, "About algorithm")) {
                    state = INFO;
                }
                if (GuiButton({100, 410, 200, 50}, "Exit")) {
                    state = EXIT;
                }

                // Настройки скорости
                DrawText("Speed:", 100, 500, 20, DARKGRAY);
                GuiSlider({170, 500, 200, 20}, "Slow", "Fast", &sortSpeed, 5.0f, 100.0f);

                // Правая панель - данные
                DrawRectangle(500, 200, 400, 300, Fade(LIGHTGRAY, 0.3f));
                DrawRectangleLines(500, 200, 400, 300, GRAY);

                if (useRandomData) {
                    DrawText("Using random data", 520, 220, 20, RED);

                    // Управление случайными данными
                    DrawText("Number count:", 520, 260, 18, DARKGRAY);
                    GuiSpinner({650, 260, 100, 25}, NULL, &randomCount, 5, 500, false);

                    DrawText("Max value:", 520, 300, 18, DARKGRAY);
                    GuiSpinner({650, 300, 100, 25}, NULL, &randomMaxValue, 10, 1000, false);

                    if (GuiButton({520, 350, 150, 40}, "Generate")) {
                        original = GenerateRandomNumbers(randomCount, randomMaxValue);
                    }
                } else {
                    DrawText("Using file data", 520, 220, 20, BLUE);
                    DrawText(TextFormat("File: %s", currentFile.c_str()), 520, 250, 16, DARKGRAY);
                    DrawText(TextFormat("Numbers: %d", (int)original.size()), 520, 280, 16, DARKGRAY);
                }

                // Переключение между файлом и случайными данными
                if (GuiButton({520, 410, 150, 40}, useRandomData ? "Load file" : "Use random")) {
                    useRandomData = !useRandomData;
                    if (useRandomData) {
                        original = GenerateRandomNumbers(randomCount, randomMaxValue);
                    }
                }

                // Инструкция
                DrawText("Drag & drop file here", 520, 470, 18, GRAY);
                break;
            }

            case SORTING: {
                timer += GetFrameTime();
                if (timer >= 1.0f / sortSpeed && !sortState.done) {
                    timer = 0.0f;
                    sortState.step(arr);
                }

                if (IsKeyPressed(KEY_ENTER)) state = MENU;
                state = RenderSorting(arr, sortState.j + 1, sortState.i, {}, false);
                break;
            }

            case STEP_MODE: {
                if (IsKeyPressed(KEY_SPACE) && !sortState.done) {
                    sortState.step(arr);
                }

                if (IsKeyPressed(KEY_ENTER)) state = MENU;
                state = RenderSorting(arr, sortState.j + 1, sortState.i, {}, true);
                break;
            }

            case INFO: {
                state = RenderInfo();
                break;
            }

            default: {
                state = MENU;
                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
