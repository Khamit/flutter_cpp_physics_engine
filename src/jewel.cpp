#include "Jewel.h"

Jewel::Jewel(const Vector2& pos, const Vector2& vel, float mass, JewelShape s, JewelColor c)
    : data(pos, vel, mass, s, c) {}

JewelData& Jewel::getData() {
    return data; // we return data...
}

const JewelData& Jewel::getData() const {
    return data; // we return data... bcse im idt...FFI - needs external C?
}
