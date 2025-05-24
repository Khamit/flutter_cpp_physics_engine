#pragma once

enum class JewelShape { Circle, Square, Triangle, Star /* should past orign? */ };
enum class JewelColor { Red, Blue, Green, Yellow /* Yo not forget for check it... */ };

enum class JewelType {
    Normal,
    HorizontalClear,
    VerticalClear,
    Bomb,
    Rainbow
};

struct JewelData {
    JewelShape shape;
    JewelColor color;
    JewelType type = JewelType::Normal;
    bool isMarkedForRemoval = false;
    int comboId = -1;
};
