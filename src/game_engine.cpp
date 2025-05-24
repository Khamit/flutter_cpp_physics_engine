#include "Jewel.h"

#include <jni.h>    // - after add into you android folder
#include "BoardManager.h"
#include "PhysicsWorld.h"
#include "game_engine.h"

#include <ctime>
#include <cstdlib>

#include <android/log.h>    // - after add into you android folder
#define LOG_TAG "JewelDebug"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


// Global instances
static PhysicsWorld g_world(0.0f, -9.8f);
static BoardManager* g_board = nullptr;

// Thread-safe random generator
static std::mt19937& getRandomGenerator() {
    static thread_local std::mt19937 generator(std::random_device{}());
    return generator;
}

extern "C" {

    __attribute__((visibility("default")))
    void destroy_board() {
        if (g_board) {
            LOGI("destroy_board: deleting existing board instance");
            delete g_board;
            g_board = nullptr;
        } else {
            LOGI("destroy_board: no board instance to delete");
        }
    }

    __attribute__((visibility("default")))
    void init_board(int rows, int cols, float cellSize, int colorCount, int shapeCount) {
        destroy_board();
        g_board = new BoardManager(rows, cols, cellSize, g_world, shapeCount, colorCount);
    }

    // Get the color of the jewel at a specific grid cell
    __attribute__((visibility("default")))
    int get_jewel_color_at(int row, int col) {
        if (!g_board) {
            LOGI("get_jewel_color_at: Board is null");
            return -1;
        }
        Jewel* jewel = g_board->getJewel(row, col);
        return jewel ? static_cast<int>(jewel->getData().color) : -1;
    }

    __attribute__((visibility("default")))
    JewelDataFFI get_jewel_data_ffi(int row, int col) {
        JewelDataFFI data = {0, 0, 0, 0, 1.0f, 0, 0, 0};

        if (!g_board) return data;

        Jewel* jewel = g_board->getJewel(row, col);
        if (!jewel) return data;

        const JewelData& internalData = jewel->getData();

        data.posX = internalData.position.x;
        data.posY = internalData.position.y;
        data.velX = internalData.velocity.x;
        data.velY = internalData.velocity.y;
        data.mass = internalData.mass;
        data.shape = static_cast<int>(internalData.shape);
        data.color = static_cast<int>(internalData.color);
        data.isMatched = internalData.isMatched ? 1 : 0;

        return data;
    }

    // Swap two jewels and check for matches
    __attribute__((visibility("default")))
    int swap_and_match(int row1, int col1, int row2, int col2) {
        if (!g_board) return 0;

        g_board->swapJewels(row1, col1, row2, col2);
        int score = g_board->checkAndClearMatches();

        if (score > 0) {
            g_board->refillBoard();
            g_board->resolveMatches();  // Chain reaction
        } else {
            g_board->swapJewels(row1, col1, row2, col2); // Undo
        }

        return score;
    }

    // Match-only step for game loop
    __attribute__((visibility("default")))
    int match_only() {
        if (!g_board) return 0;
        int score = g_board->checkAndClearMatches();
        if (score > 0) {
            g_board->refillBoard();
        }
        return score;
    }

    // Stub for physics
    __attribute__((visibility("default")))
    int ffi_create_body(float posX, float posY, float velX, float velY, float mass);

    // Match-check step
    __attribute__((visibility("default")))
    int check_and_clear_matches() {
        if (!g_board) return 0;
        return g_board->checkAndClearMatches();
    }

    // Refill board
    __attribute__((visibility("default")))
    void refill_board() {
        if (g_board) g_board->refillBoard();
    }
    // has valid move
    __attribute__((visibility("default")))
    int has_valid_move() {
        if (!g_board) return 0;
        return g_board->hasValidMove() ? 1 : 0;
    }
}
