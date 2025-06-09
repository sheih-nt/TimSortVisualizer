#include <stdexcept>
#include "visualizer.hpp"
#include "timsort.hpp"
#include <algorithm>
#include "raylib.h"

void TimSortVisual(std::vector<int>& arr, bool stepMode, int speed) {
    int n = arr.size();
    const int RUN = 32;

    auto render = [&](int a, std::optional<int> b, std::vector<int> merged = {}) {
        if (WindowShouldClose()) return;
        AppState state = RenderSorting(arr, a, b, merged, stepMode);
        if (state == MENU) throw std::runtime_error("Exit to menu");
    };

    try {
        for (int i = 0; i < n; i += RUN) {
            for (int j = i + 1; j <= std::min(i + RUN - 1, n - 1); ++j) {
                int temp = arr[j];
                int k = j - 1;
                while (k >= i && arr[k] > temp) {
                    arr[k + 1] = arr[k];
                    render(k + 1, j);
                    k--;
                }
                arr[k + 1] = temp;
                render(k + 1, j);
            }
        }

        for (int size = RUN; size < n; size *= 2) {
            for (int left = 0; left < n; left += 2 * size) {
                int mid = std::min(left + size - 1, n - 1);
                int right = std::min(left + 2 * size - 1, n - 1);

                if (mid < right) {
                    std::vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
                    std::vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

                    int i = 0, j = 0, k = left;
                    while (i < leftArr.size() && j < rightArr.size()) {
                        arr[k] = (leftArr[i] <= rightArr[j]) ? leftArr[i++] : rightArr[j++];
                        render(k, {}, {k});
                        k++;
                    }

                    while (i < leftArr.size()) {
                        arr[k] = leftArr[i++];
                        render(k, {}, {k});
                        k++;
                    }

                    while (j < rightArr.size()) {
                        arr[k] = rightArr[j++];
                        render(k, {}, {k});
                        k++;
                    }
                }
            }
        }
    } catch (...) {
        return;
    }
}
