#include "BoardManager.h"
#include "InitialBoardGenerator.h"
#include <cstdlib>
#include <set>

#include <android/log.h> // - after add into you android folder
#define LOG_TAG "JewelDebug"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


BoardManager::BoardManager(int rows, int cols, float cellSize, PhysicsWorld& worldRef,
                        int shapeCount, int colorCount, double matchChance)
    : numRows(rows), numCols(cols), cellSize(cellSize), world(worldRef),
    jewelShapeCount(shapeCount), jewelColorCount(colorCount),
    matched(rows, std::vector<bool>(cols, false)),
    rng(std::random_device{}()),
    matchChance(matchChance) // initialize member
{
    grid.resize(rows, std::vector<Jewel*>(cols, nullptr));
    fillBoardRandomly();
}


void BoardManager::resolveMatches() {
    int initialScore = checkAndClearMatches();

    LOGI("Initial match count: %d", initialScore);

    if (initialScore == 0) return;

    refillBoard();

    while (true) {
        int score = checkAndClearMatches();
        if (score == 0)
            break;
        refillBoard();
    }
}
void BoardManager::fillBoardRandomly() {
    InitialBoardGenerator generator(numRows, numCols, jewelShapeCount, jewelColorCount, matchChance);
    auto jewelData = generator.generate();

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            auto& [shape, color] = jewelData[row][col];
            Vector2 pos(col * cellSize, row * cellSize);
            grid[row][col] = world.createJewel(pos, Vector2(0, 0), 1.0f, shape, color);

            LOGI("Created jewel at (%d,%d): Color=%d, Shape=%d\n", row, col, color, shape);
        }
    }
}

void BoardManager::fillFallbackBoard() {
    // Example fallback: fill with alternating colors/shapes
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (grid[row][col]) {
                delete grid[row][col];
            }

            JewelShape shape = static_cast<JewelShape>((row + col) % jewelShapeCount);
            JewelColor color = static_cast<JewelColor>((row * col + 1) % jewelColorCount);

            Vector2 pos(col * cellSize, row * cellSize);
            grid[row][col] = world.createJewel(pos, Vector2(0, 0), 1.0f, shape, color);
        }
    }
}



bool BoardManager::createsMatchAt(int row, int col) {
    JewelColor color = grid[row][col]->getData().color;

    // Horizontal
    if (col >= 2 &&
        grid[row][col - 1] && grid[row][col - 2] &&
        grid[row][col - 1]->getData().color == color &&
        grid[row][col - 2]->getData().color == color) {
        return true;
    }

    // Vertical
    if (row >= 2 &&
        grid[row - 1][col] && grid[row - 2][col] &&
        grid[row - 1][col]->getData().color == color &&
        grid[row - 2][col]->getData().color == color) {
        return true;
    }

    return false;
}

Jewel* BoardManager::getJewel(int row, int col) const {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols)
        return nullptr;
    return grid[row][col];
}

void BoardManager::swapJewels(int row1, int col1, int row2, int col2) {
    if (getJewel(row1, col1) && getJewel(row2, col2)) {
        std::swap(grid[row1][col1], grid[row2][col2]);

        // Update positions in JewelData
        if (grid[row1][col1]) {
            auto& data = grid[row1][col1]->getData();
            data.position.x = col1 * cellSize;
            data.position.y = row1 * cellSize;

        }
        if (grid[row2][col2]) {
            auto& data = grid[row2][col2]->getData();
            data.position.x = col2 * cellSize;
            data.position.y = row2 * cellSize;
        }
    }
}

// new
bool BoardManager::isJewelMatched(int row, int col) const {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols)
        return false;
    return matched[row][col];
}
// --- check
int BoardManager::checkAndClearMatches() {
    int score = 0;
    // Reset matched flags (use member variable)
    matched.assign(numRows, std::vector<bool>(numCols, false));

    // Check horizontal matches
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols - 2; ++col) {
            if (!grid[row][col] || !grid[row][col + 1] || !grid[row][col + 2]) continue;

            JewelColor c1 = grid[row][col]->getData().color;
            JewelColor c2 = grid[row][col + 1]->getData().color;
            JewelColor c3 = grid[row][col + 2]->getData().color;

            if (c1 == c2 && c2 == c3) {
                int matchLen = 3;
                matched[row][col] = matched[row][col + 1] = matched[row][col + 2] = true;

                int k = col + 3;
                while (k < numCols && grid[row][k] && grid[row][k]->getData().color == c1) {
                    matched[row][k++] = true;
                    matchLen++;
                }

                col = k - 1;
                score += matchLen * 10;
            }
        }
    }

    // Check vertical matches
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows - 2; ++row) {
            if (!grid[row][col] || !grid[row + 1][col] || !grid[row + 2][col]) continue;

            JewelColor c1 = grid[row][col]->getData().color;
            JewelColor c2 = grid[row + 1][col]->getData().color;
            JewelColor c3 = grid[row + 2][col]->getData().color;

            if (c1 == c2 && c2 == c3) {
                int matchLen = 3;
                matched[row][col] = matched[row + 1][col] = matched[row + 2][col] = true;

                int k = row + 3;
                while (k < numRows && grid[k][col] && grid[k][col]->getData().color == c1) {
                    matched[k++][col] = true;
                    matchLen++;
                }

                row = k - 1;
                score += matchLen * 10;
            }
        }
    }

    // Delete matched jewels
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (matched[row][col]) {
                delete grid[row][col];
                grid[row][col] = nullptr;
            }
        }
    }

    return score;
}
// fix
bool BoardManager::wouldCauseMatchAt(int row, int col, JewelColor color) const {
    // Check horizontal
    if (col >= 2) {
        if (grid[row][col - 1] && grid[row][col - 2]) {
            if (grid[row][col - 1]->getData().color == color &&
                grid[row][col - 2]->getData().color == color) {
                return true;
            }
        }
    }

    // Check vertical (only look downward because we fill top-down)
    if (row <= numRows - 3) {
        if (grid[row + 1][col] && grid[row + 2][col]) {
            if (grid[row + 1][col]->getData().color == color &&
                grid[row + 2][col]->getData().color == color) {
                return true;
            }
        }
    }

    return false;
}

void BoardManager::refillBoard() {
    int attempts = 0;
    const int maxAttempts = 10;

    do {
        // Drop and refill
        internalRefill();

        // Step 2: Clear accidental matches (chain reaction)
        while (checkAndClearMatches() > 0) {
            internalRefill();
        }

        //Check for at least one valid move
        attempts++;
    } while (!hasValidMove() && attempts < maxAttempts);

    if (attempts >= maxAttempts) {
        LOGI("Fallback triggered: no valid moves after %d attempts", maxAttempts);
        fillFallbackBoard();  //predefined board with a known valid move
    }
}

void BoardManager::internalRefill() {
    for (int col = 0; col < numCols; ++col) {
        std::vector<Jewel*> columnJewels;

        for (int row = numRows - 1; row >= 0; --row) {
            if (grid[row][col]) {
                columnJewels.push_back(grid[row][col]);
                grid[row][col] = nullptr;
            }
        }

        int writeRow = numRows - 1;
        for (Jewel* jewel : columnJewels) {
            grid[writeRow][col] = jewel;
            auto& data = jewel->getData();
            data.position.x = col * cellSize;
            data.position.y = writeRow * cellSize;
            --writeRow;
        }

        std::uniform_int_distribution<int> shapeDist(0, jewelShapeCount - 1);
        std::uniform_int_distribution<int> colorDist(0, jewelColorCount - 1);

        for (int row = writeRow; row >= 0; --row) {
            JewelShape shape = static_cast<JewelShape>(shapeDist(rng));
            JewelColor color;
            int attempts = 0;
            const int maxAttempts = 10;

            do {
                color = static_cast<JewelColor>(colorDist(rng));
                attempts++;
            } while (wouldCauseMatchAt(row, col, color) && attempts < maxAttempts);

            Vector2 pos(col * cellSize, row * cellSize);
            grid[row][col] = world.createJewel(pos, Vector2(0, 0), 1.0f, shape, color);
        }
    }
}

bool BoardManager::hasValidMove() {
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            Jewel* jewel = grid[row][col];
            if (!jewel) continue;

            JewelColor color = jewel->getData().color;

            // Try right swap
            if (col + 1 < numCols && grid[row][col + 1]) {
                std::swap(grid[row][col], grid[row][col + 1]);
                if (createsMatchAt(row, col) || createsMatchAt(row, col + 1)) {
                    std::swap(grid[row][col], grid[row][col + 1]);
                    return true;
                }
                std::swap(grid[row][col], grid[row][col + 1]);
            }

            // Try down swap
            if (row + 1 < numRows && grid[row + 1][col]) {
                std::swap(grid[row][col], grid[row + 1][col]);
                if (createsMatchAt(row, col) || createsMatchAt(row + 1, col)) {
                    std::swap(grid[row][col], grid[row + 1][col]);
                    return true;
                }
                std::swap(grid[row][col], grid[row + 1][col]);
            }
        }
    }
    return false;
}


const std::vector<std::vector<Jewel*>>& BoardManager::getGrid() const {
    return grid;
}

int BoardManager::getRows() const { return numRows; }
int BoardManager::getCols() const { return numCols; }
