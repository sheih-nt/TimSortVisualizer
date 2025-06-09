#include "raylib.h"
#include "raygui.h"
#include "visualizer.hpp"
#include <fstream>
#include <algorithm>

std::vector<int> LoadNumbersFromFile(const std::string& path) {
    std::ifstream file(path);
    std::vector<int> numbers;
    int num;
    while (file >> num) numbers.push_back(num);
    return numbers;
}

AppState RenderMenu() {
    DrawText("TimSort Visualizer", 100, 80, 40, DARKGRAY);
    if (GuiButton({100, 200, 200, 50}, "Start (anim)")) return SORTING;
    if (GuiButton({100, 270, 200, 50}, "Start (sahg)")) return STEP_MODE;
    if (GuiButton({100, 340, 200, 50}, "About algo")) return INFO;
    if (GuiButton({100, 410, 200, 50}, "Exit")) return EXIT;
    return MENU;
}

AppState RenderInfo() {
    DrawText("TimSort — гибрид Insertion и Merge Sort.", 100, 100, 24, DARKGRAY);
    DrawText("Нажмите [ESC] для возврата", 100, 150, 20, GRAY);
    if (IsKeyPressed(KEY_ESCAPE)) return MENU;
    return INFO;
}

AppState RenderSorting(const std::vector<int>& arr, int a, std::optional<int> b, std::vector<int> merged, bool stepMode) {
    ClearBackground(RAYWHITE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int barWidth = screenWidth / arr.size();
    int fontSize = 12;
    int textPadding = 5;

    for (size_t i = 0; i < arr.size(); ++i) {
        Color color = GRAY;
        if ((int)i == a) color = RED;
        else if (b && (int)i == b.value()) color = GREEN;
        else if (std::find(merged.begin(), merged.end(), i) != merged.end()) color = ORANGE;

        int height = arr[i] * 5;
        DrawRectangle(i * barWidth, screenHeight - height - fontSize - textPadding,
                       barWidth - 1, height, color);

        DrawText(TextFormat("%d", arr[i]),
                 i * barWidth + (barWidth - MeasureText(TextFormat("%d", arr[i]), fontSize)) / 2,
                 screenHeight - fontSize,
                 fontSize,
                 DARKGRAY);
    }

    // Обновленная строка подсказки
    DrawText(stepMode ? "ENTER - Menu | SPACE - Step" : "ENTER - Menu", 10, 10, 20, DARKGRAY);

    if (IsKeyPressed(KEY_ENTER)) return MENU;
        return stepMode ? STEP_MODE : SORTING;
}

std::vector<int> GenerateRandomNumbers(int count, int max_value) {
    std::vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        numbers.push_back(GetRandomValue(1, max_value));
    }
    return numbers;
}

bool IsFileDroppedCustom() {
    return ::IsFileDropped();
}

std::string GetDroppedFilePath() {
    if (IsFileDroppedCustom()) {
        FilePathList files = ::LoadDroppedFiles();
        if (files.count > 0) {
            std::string path(files.paths[0]);
            ::UnloadDroppedFiles(files); // Используем UnloadDroppedFiles вместо ClearDroppedFiles
            return path;
        }
    }
    return "";
}
