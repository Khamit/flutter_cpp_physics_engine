#pragma once // after ffi

#include "PhysicsWorld.h"
#include "Jewel.h"
#include <vector>
#include <random>

class BoardManager {
public:
    BoardManager(int rows, int cols, float cellSize, PhysicsWorld& worldRef,
                int shapeCount, int colorCount, double matchChance = 0.0);

    // Main game mechanics
    void swapJewels(int row1, int col1, int row2, int col2);
    int checkAndClearMatches();      // Detect and remove matches, return score
    void refillBoard();              // Drop jewels and fill new ones
    void fillBoardRandomly();        // Fill grid with random jewels (no initial matches)
    void resolveMatches();           // Repeat match-check-clear-refill until stable

    // Getters
    Jewel* getJewel(int row, int col) const;
    const std::vector<std::vector<Jewel*>>& getGrid() const;
    int getRows() const;
    int getCols() const;
    bool hasValidMove();
    // Query
    bool isJewelMatched(int row, int col) const;
    bool wouldCauseMatchAt(int row, int col, JewelColor color) const;

private:
    bool createsMatchAt(int row, int col);

    void internalRefill();           // Helper to drop jewels and fill empty spots
    void fillFallbackBoard();        // Fallback board if no valid moves after retries

private:
    int numRows;
    int numCols;
    float cellSize;

    int jewelShapeCount;
    int jewelColorCount;
    double matchChance;

    PhysicsWorld& world;

    std::vector<std::vector<Jewel*>> grid;
    std::vector<std::vector<bool>> matched;

    mutable std::mt19937 rng;        // Random engine, mutable because of const methods RR
};
