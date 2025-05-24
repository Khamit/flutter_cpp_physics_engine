#include <random>
#include <vector>
#include <utility>
#include <cstdlib>  // for rand()
#include "InitialBoardGenerator.h"

constexpr int MAX_ATTEMPTS = 10;

InitialBoardGenerator::InitialBoardGenerator(int rows, int cols, int shapeCount, int colorCount, double matchChance)
    : rows(rows), cols(cols), shapeCount(shapeCount), colorCount(colorCount),
    rng(std::random_device{}()), matchChance(matchChance) {}

std::vector<std::vector<std::pair<JewelShape, JewelColor>>> InitialBoardGenerator::generate() {
    std::vector<std::vector<std::pair<JewelShape, JewelColor>>> jewelGrid(rows);
    std::vector<std::vector<JewelColor>> colorGrid(rows, std::vector<JewelColor>(cols));

    std::uniform_real_distribution<double> distReal(0.0, 1.0);

    for (int row = 0; row < rows; ++row) {
        jewelGrid[row].reserve(cols);
        for (int col = 0; col < cols; ++col) {
            JewelShape shape = randomShape();
            JewelColor color;
            int attempts = 0;

            do {
                color = randomColor();
                attempts++;
                // Allow matches with a probability equal to matchChance
                if (distReal(rng) < matchChance) {
                    break; // Break early and allow this match
                }
            } while (wouldCauseMatch(row, col, color, colorGrid) && attempts < MAX_ATTEMPTS);

            colorGrid[row][col] = color;
            jewelGrid[row].emplace_back(shape, color);
        }
    }

    return jewelGrid;
}

bool InitialBoardGenerator::wouldCauseMatch(int row, int col, JewelColor color,
                                        const std::vector<std::vector<JewelColor>>& board) const {
    // Horizontal check for 3-in-a-row???
    if (col >= 2 && board[row][col-1] == color && board[row][col-2] == color)
        return true;
    
    // Vertical check for 3-in-a-column???
    if (row >= 2 && board[row-1][col] == color && board[row-2][col] == color)
        return true;
    
    return false;
}

JewelShape InitialBoardGenerator::randomShape() const {
    std::uniform_int_distribution<int> dist(0, shapeCount - 1);
    return static_cast<JewelShape>(dist(rng));
}

JewelColor InitialBoardGenerator::randomColor() const {
    std::uniform_int_distribution<int> dist(0, colorCount - 1);
    return static_cast<JewelColor>(dist(rng));
}
