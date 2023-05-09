#include "block.h"

Block::Block (std::string s, std::vector<std::vector<char>> bmap, position pos) {
    id = s;
    bitmap = bmap;
    board_pos.x = pos.x;
    board_pos.y = pos.y;
    bitmap_pos.x = 0;
    bitmap_pos.y = 0;
};

std::string Block::getID() {
    return this->id;
};

void Block::rotate() {
// Rotate a block to the right 90 degrees
// Input is m x n, Output is n x m
    int m = this->bitmap.size();
    int n = this->bitmap[0].size();
    std::vector<char> output_row(m, 0);
    std::vector<std::vector<char>> output(n, output_row);
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        int new_j = m-1-i;
        int new_i = j;
        output[new_i][new_j] = this->bitmap[i][j];
        if (output[new_i][new_j] == 2){
          this->bitmap_pos.x = new_i;
          this->bitmap_pos.y = new_j;
        }
      }
    }
    this->bitmap = output;
};

void Block::flip() {
// Flip a block across the vertical
// Input is m x n, Output is m x n
    int m = this->bitmap.size();
    int n = this->bitmap[0].size();
    std::vector<std::vector<char>> output(this->bitmap);
    reverse(begin(output), end(output));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (output[i][j] == 2) {
          this->bitmap_pos.x = i;
          this->bitmap_pos.y = j;
        }
      }
    }
    this->bitmap = output;
};

std::vector<position> Block::getPositions() {
// Using the block's board position and bitmap position, 
// return a vector of all the board positions for the block.
    std::vector<position> positions;
    position cur_pos;
    int m = this->bitmap.size();
    int n = this->bitmap[0].size();
    int x = this->board_pos.x - this->bitmap_pos.x;
    int y = this->board_pos.y - this->bitmap_pos.y;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (this->bitmap[i][j] != 0) {
          cur_pos.x = x + i;
          cur_pos.y = y + j;
          positions.push_back(cur_pos);
        }
      }
    }
    return positions;
};

void Block::displayBlock() {
    std::cout << "this block is " << this->bitmap.size() << "x" << this->bitmap[0].size() << "\n";
    for (int i = 0; i < this->bitmap.size(); i++) {
      const std::vector<char> &row = this->bitmap[i];
      for (int j = 0; j < row.size(); j++) {
        std::cout << static_cast<int>(row[j]);
      }
      std::cout << "\n";
    }
    std::cout << "x pos: " << this->board_pos.x << " y pos:" << this->board_pos.y << "\n";
};

