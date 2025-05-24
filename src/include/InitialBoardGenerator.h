#pragma once

#include "Jewel.h"
#include <vector>
#include <utility>
#include <random>

class InitialBoardGenerator {
public:
    // default value (e.g., 0.0 to keep old behavior) - should be 0 - but 30 is not bad? YES! Maybe not, im not sure
    InitialBoardGenerator(int rows, int cols, int shapeCount, int colorCount, double matchChance = 30.0);

    std::vector<std::vector<std::pair<JewelShape, JewelColor>>> generate();

private:
    const int rows, cols;
    const int shapeCount, colorCount;
    const double matchChance;

    mutable std::mt19937 rng;

    JewelShape randomShape() const;
    JewelColor randomColor() const;
    bool wouldCauseMatch(int row, int col, JewelColor color,
                    const std::vector<std::vector<JewelColor>>& board) const;

    static constexpr int MAX_ATTEMPTS = 50;
};
