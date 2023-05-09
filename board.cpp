#include "board.h"

Board::Board() {
    placed = std::vector<Block>();
    available_positions = std::unordered_set<position, position_hash, position_comparator>();
    available_blocks = std::vector<std::string>(
                                      {"rec",
                                       "z",
                                       "L",
                                       "corner",
                                       "lightning",
                                       "horseshoe",
                                       "cactus",
                                       "chair"
                                      });
};

// Copy constructor
Board::Board(const Board &b) {
    placed = std::vector<Block>(b.placed);
    available_positions = std::unordered_set<position, position_hash, position_comparator>(b.available_positions);
    available_blocks = std::vector<std::string>(b.available_blocks);
};

void Board::initPositions(const std::string month, const std::string day) {
// Add all the board positions except the given month and day to available_positions
    position cur_position;
    std::unordered_map<int, int> bcmap = {
        {0, 5}, // row 0 has columns 0-5
        {1, 5},
        {2, 6},
        {3, 6},
        {4, 6},
        {5, 6},
        {6, 2}
    };
    // 43 board positions
    std::vector<std::vector<std::string>> boardArray = {
        {"Jan", "Feb", "Mar", "Apr", "May", "Jun"},
        {"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
        {"1", "2", "3", "4", "5", "6", "7"},
        {"8", "9", "10", "11", "12", "13", "14"},
        {"15", "16", "17", "18", "19", "20", "21"},
        {"22", "23", "24", "25", "26", "27", "28"},
        {"29", "30", "31"}
    };
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j <= bcmap[i]; j++) {
            if (boardArray[i][j] != month && boardArray[i][j] != day) {
                cur_position.x = i;
                cur_position.y = j;
                this->available_positions.insert(cur_position);
            }
        }
    }
};

bool Board::floodfill(position p) {
// Checks to see if there are islands where any available block cannot be placed to reduce search space
// Return true if flood fill finds an island of size >= 5
// Return false if the island is too small for a block, and therefore the board is invalid (smallest block size is 5 spaces)
    std::unordered_set<position, position_hash, position_comparator> island;
    position left, right, up, down;
    std::stack<position> s;
    s.push(p);
    island.insert(p);

    while (!s.empty()) {
        if (island.size() >= 5) {
            return true;
        }
        position curpos = s.top();
        s.pop();

        left.y = right.y = curpos.y;
        left.x = curpos.x - 1;
        if (this->available_positions.find(left) != this->available_positions.end() && island.find(left) == island.end()) {
            island.insert(left);
            s.push(left);
        }
        right.x = curpos.x + 1;
        if (this->available_positions.find(right) != this->available_positions.end() && island.find(right) == island.end()) {
            island.insert(right);
            s.push(right);
        }
        up.x = down.x = curpos.x;
        up.y =curpos.y + 1;
        if (this->available_positions.find(up) != this->available_positions.end() && island.find(up) == island.end()) {
            island.insert(up);
            s.push(up);
        }
        down.y = curpos.y - 1;
        if (this->available_positions.find(down) != this->available_positions.end() && island.find(down) == island.end()) {
            island.insert(down);
            s.push(down);
        }
    }
    return false;
  };

bool Board::placeBlock(Block b) {
// Before placing, check that the block fits on the board (all block positions are in board available_positions)
// Return false if the block doesn't fit.
// If the block placement is valid, remove the block's positions from avaiable_positions, add the block to placed, remove the block from available_blocks
// Check to see if the placement created any "islands" (regions of empty space too small for any block)
// If an island with a size less than 5 spaces is found, return false, otherwise the board is valid, return true
    for (position pos: b.getPositions()) {
        if (this->available_positions.find(pos) == this->available_positions.end()) {
            return false;
        }
    }

    this->placed.push_back(b);
    auto block_id = find(this->available_blocks.begin(), this->available_blocks.end(), b.getID());
    if (block_id != this->available_blocks.end()) {
        this->available_blocks.erase(block_id);
    }
    for (position p : b.getPositions()) {
        this->available_positions.erase(p);
    }
   // If we have placed at least 1 block and haven't filled the board, floodfill the board from each available position
   // Floodfill returns false if an island with a size less than 5 board spaces is found
    if (!this->available_positions.empty() && this->placed.size() > 1) {
        for (position pos : this->available_positions) {
            if (!floodfill(pos)) {
                return false;
            }
        }
    } return true;
  };

bool Board::checkWin() {
    return (this->available_blocks.empty()); 
};