#pragma once
#include <vector>
#include <optional>
#include <string>

enum AppState {
    MENU,
    SORTING,
    STEP_MODE,
    INFO,
    EXIT
};

std::vector<int> LoadNumbersFromFile(const std::string& path);
AppState RenderMenu();
AppState RenderInfo();
AppState RenderSorting(const std::vector<int>& arr, int a, std::optional<int> b, std::vector<int> merged, bool stepMode);
