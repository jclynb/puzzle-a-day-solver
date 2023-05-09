#ifndef BLOCK
#define BLOCK

#include "position.h"
#include <algorithm>
#include <vector>
#include <iostream>

class Block {
    public:
        Block (std::string s, std::vector<std::vector<char>> bmap, position pos);
        std::string getID();
        void rotate();
        void flip();
        std::vector<position> getPositions();
        void displayBlock();

    private:
        std::string id;
        std::vector<std::vector<char>> bitmap;
        position board_pos;  // (x,y) position of "2" on game board
        position bitmap_pos; // (x,y) position of "2" in bitmap
};

#endif