#pragma once

#include <cstdint>
#include <bitset>

const uint8_t MAX_COMPONENTS = 64;
using Signature = std::bitset<MAX_COMPONENTS>;

enum class GameState {
	Start, InGame, GameOver, Victory
};