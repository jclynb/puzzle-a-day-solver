#ifndef BOARD
#define BOARD

#include "block.h"
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

class Board {
    public:
        Board();
        Board (const Board &b);
        void initPositions(const std::string month, const std::string day);
        bool floodfill(position p);
        bool placeBlock(Block b);
        bool checkWin();

        std::vector<Block> placed; // Keeps track of all the blocks we have placed
        std::unordered_set<position, position_hash, position_comparator> available_positions; // Keeps track of positions left on board
        std::vector<std::string> available_blocks; // Keeps track of unplaced blocks
};

#endif