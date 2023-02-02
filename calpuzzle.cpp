#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <stack>
#include <cassert>
#include <memory>
using namespace std;

// Block positions (x, y) oriented at top left labeled "2"
vector<vector<char>> rec = {
    {2, 1},
    {1, 1},
    {1, 1}
};

vector<vector<char>> z = {
    {2, 1, 0},
    {0, 1, 0},
    {0, 1, 1}
};

vector<vector<char>> L = {
    {2, 0},
    {1, 0},
    {1, 0},
    {1, 1}
};

vector<vector<char>> corner = {
    {2, 0, 0},
    {1, 0, 0},
    {1, 1, 1}
};

vector<vector<char>> lightning = {
    {2, 0},
    {1, 1},
    {0, 1},
    {0, 1}
};

vector<vector<char>> horseshoe = {
    {2, 0, 1},
    {1, 1, 1}
};

vector<vector<char>> cactus = {
    {2, 0},
    {1, 1},
    {1, 0},
    {1, 0}
};

vector<vector<char>> chair = {
    {2, 0},
    {1, 1},
    {1, 1}
};

// 43 board positions
vector<vector<string>> boardArray = {
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun"},
  {"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
  {"1", "2", "3", "4", "5", "6", "7"},
  {"8", "9", "10", "11", "12", "13", "14"},
  {"15", "16", "17", "18", "19", "20", "21"},
  {"22", "23", "24", "25", "26", "27", "28"},
  {"29", "30", "31"}
};

// To help with initializing the board
unordered_map<int, int> bcmap = {
  {0, 5}, // row 0 has columns 0-5
  {1, 5},
  {2, 6},
  {3, 6},
  {4, 6},
  {5, 6},
  {6, 2}
};

typedef struct position {
  int x;
  int y;
} position;

struct position_comparator {
  // Check if two positions are equal
  bool operator() (const position &a, const position &b) const {
    return (a.x == b.x) && (a.y == b.y);
  }
};

struct position_hash {
  // Hash tuple
public:
  size_t operator() (const position &p) const {
    auto hash1 = hash<int>()(p.x);
    auto hash2 = hash<int>()(p.y);

    if (hash1 != hash2) { // FIXME: figure out if xor makes sense here
      return hash1 ^ hash2;
    }

    return hash1;
  }
};

class Block {
  string id;
  vector<vector<char>> bitmap;
  position board_pos;  // (x,y) position of "2" on game board
  position bitmap_pos; // (x,y) position of "2" in bitmap

public:
  Block (string s, vector<vector<char>> bmap, position pos) {
    id = s;
    bitmap = bmap;
    board_pos.x = pos.x;
    board_pos.y = pos.y;
    bitmap_pos.x = 0;
    bitmap_pos.y = 0;
  };

  string getID() {
    return this->id;
  };

  void rotate() {
    // Rotate a block to the right 90 degrees
    // Input is m x n, Output is n x m
    int m = this->bitmap.size();
    int n = this->bitmap[0].size();
    vector<char> output_row(m, 0);
    vector<vector<char>> output(n, output_row);
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

  void flip() {
    // Flip a block across the vertical
    // Input is m x n, Output is m x n
    int m = this->bitmap.size();
    int n = this->bitmap[0].size();
    vector<vector<char>> output(this->bitmap);
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

  vector<position> getPositions() {
    // Using the block's board position and bitmap position, return a vector of all the board positions for the block.
    vector<position> positions;
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

  void displayBlock() {
    std::cout << "this block is " << this->bitmap.size() << "x" << this->bitmap[0].size() << "\n";
    for (int i = 0; i < this->bitmap.size(); i++) {
      const vector<char> &row = this->bitmap[i];
      for (int j = 0; j < row.size(); j++) {
        std::cout << static_cast<int>(row[j]);
      }
      std::cout << "\n";
    }
    std::cout << "x pos: " << this->board_pos.x << " y pos:" << this->board_pos.y << "\n";
  };
};

class Board {
public:
  shared_ptr<vector<Block>> placed; // Keeps track of all the blocks we have placed
  shared_ptr<unordered_set<position, position_hash, position_comparator>> available_positions; // Keeps track of positions left on board
  shared_ptr<unordered_map<string, vector<vector<char>>>> available_blocks; // Keeps track of unplaced blocks

  Board() {
    placed = make_shared<vector<Block>>();
    available_positions = make_shared<unordered_set<position, position_hash, position_comparator>>();
    available_blocks = make_shared<unordered_map<string, vector<vector<char>>>>();
    *available_blocks = {
        {"rec", rec},
        {"z", z},
        {"L", L},
        {"corner", corner},
        {"lightning", lightning},
        {"horseshoe", horseshoe},
        {"cactus", cactus},
        {"chair", chair}
    };
  };

  // Copy constructor
  Board(const Board &b) {
    placed = make_shared<vector<Block>>(*b.placed);
    available_positions = make_shared<unordered_set<position, position_hash, position_comparator>>(*b.available_positions);
    available_blocks = make_shared<unordered_map<string, vector<vector<char>>>>(*b.available_blocks);
  };

  void initPositions(const string month, const string day) {
    // Add all the board positions except the given month and day to available_positions
    position cur_position;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j <= bcmap[i]; j++) {
        if (boardArray[i][j] != month && boardArray[i][j] != day) {
          cur_position.x = i;
          cur_position.y = j;
          this->available_positions->insert(cur_position);
        }
      }
    }
  };

  bool floodfill(position p) {
    // Return true if flood fill finds an island of size >= 5
    // Return false if the island is too small for a block, and therefore the board is invalid (smallest block size is 5 spaces)
    unordered_set<position, position_hash, position_comparator> island;
    position left, right, up, down;
    stack<position> q;
    q.push(p);

    while (!q.empty()) {
      if (island.size() >= 5) {
        return true;
      }
      position curpos = q.top();
      q.pop();

      left.y = right.y = curpos.y;
      left.x = curpos.x - 1;
      if (this->available_positions->find(left) != this->available_positions->end() && island.find(left) == island.end()) {
        island.insert(left);
        q.push(left);
      }
      right.x = curpos.x + 1;
      if (this->available_positions->find(right) != this->available_positions->end() && island.find(right) == island.end()) {
        island.insert(right);
        q.push(right);
      }
      up.x = down.x = curpos.x;
      up.y =curpos.y + 1;
      if (this->available_positions->find(up) != this->available_positions->end() && island.find(up) == island.end()) {
        island.insert(up);
        q.push(up);
      }
      down.y = curpos.y - 1;
      if (this->available_positions->find(down) != this->available_positions->end() && island.find(down) == island.end()) {
        island.insert(down);
        q.push(down);
      }
    }
    return false;
  };

  bool placeBlock(Block b) {
    // Before placing, check that the block fits on the board (all block positions are in board available_positions)
    // Return false if the block doesn't fit.
    // If the block placement is valid, remove the block's positions from avaiable_positions, add the block to placed, remove the block from available_blocks
    // Check to see if the placement created any "islands" (regions of empty space too small for any block)
    // If an island with a size less than 5 spaces is found, return false, otherwise the board is valid, return true
    for (position pos: b.getPositions()) {
      if (this->available_positions->find(pos) == this->available_positions->end()) {
        return false;
      }
    }

    this->placed->push_back(b);
    this->available_blocks->erase(b.getID());
    for (position p : b.getPositions()) {
      this->available_positions->erase(p);
    }
   // If we have placed at least 1 block and haven't filled the board, floodfill the board from each available position
   // Floodfill returns false if an island with a size less than 5 board spaces is found
    if (!this->available_positions->empty() && this->placed->size() > 1) {
      for (position pos : *this->available_positions) {
        if (!floodfill(pos)) {
          return false;
        }
      }
    } return true;
  };

  bool checkWin() {
    return (this->available_blocks->empty()); // (this->available_positions->empty()) && (this->placed->size() == 8));
  };
};

int main(int argc, char *argv[]) {
  // DFS approach:
  // Start by adding an empty board to the stack.
  // Pop the board, and check for win condition.
  // Else, loop through the board's available blocks and avaiable positions.
  // Try placing a given block at a given board position. If it's a valid placement, add the new board with this new valid placement to the stack.
  // Next try placing rotations/flips of the same block. If any of those placements are valid, add those new boards to stack.
  // Repeat until checkWin is True.

  if (argc != 3) {
    cerr << "incorrect input, type a month and date" << "\n";
    return -1;
  }
  string month = argv[1];
  string day = argv[2];
  vector<string> asymmetrical = {"cactus", "chair", "lightning", "L", "z"};

  auto board = make_shared<Board>();
  board->initPositions(month, day);

  stack<shared_ptr<Board>> s;
  s.push(board);

  while (!s.empty()) {
    auto board = s.top();
    s.pop();

    if (board->checkWin()) {
      for (Block b : *board->placed) {
        b.displayBlock();
      }
      cout << "solution found" << "\n";
      return 1;
    }

    for (auto i : *board->available_blocks) {
      for (const auto pos : *board->available_positions) {

        auto blk = make_shared<Block>(i.first, i.second, pos);
        auto new_board_1 = make_shared<Board>(*board);
        if (new_board_1->placeBlock(*blk)) {
          assert(new_board_1->available_blocks->size() == board->available_blocks->size() - 1);
          s.push(new_board_1);
        }
        // rotate 3 times
        for (int i = 0; i < 3; i++) {
          blk->rotate();
          auto new_board_2 = make_shared<Board>(*board);
          if (new_board_2->placeBlock(*blk)) {
            assert(new_board_2->available_blocks->size() == board->available_blocks->size() - 1);
            s.push(new_board_2);
          }
        }
        // only need to flip asymmetrical blocks
        if (find(asymmetrical.begin(), asymmetrical.end(), blk->getID()) != asymmetrical.end()) {
          blk->flip();
          auto new_board_3 = make_shared<Board>(*board);
          if (new_board_3->placeBlock(*blk)) {
            assert(new_board_3->available_blocks->size() == board->available_blocks->size() - 1);
            s.push(new_board_3);
          }
          // rotate 3 times
          for (int i = 0; i < 3; i++) {
            blk->rotate();
            auto new_board_4 = make_shared<Board>(*board);
            if (new_board_4->placeBlock(*blk)) {
              assert(new_board_4->available_blocks->size() == board->available_blocks->size() - 1);
              s.push(new_board_4);
            }
          }
        }
      }
    }
  }
  cout << "no solution found" << "\n";
  return -1;
};
/*
int main(int argc, char* argv[]) {
  // TEST CASE FOR A JAN 1 WINNING SOLUTION
  string month = argv[1];
  string day = argv[2];

  auto board = make_shared<Board>();
  board->initPositions(month, day);
  cout << board->placed->size() << "\n";
  cout << board->available_blocks->size() << "\n";
  cout << board->available_positions->size() << "\n";

  position one = {0,1};
  position two = {0,4};
  position three = {0,5};
  position four = {2,4};
  position five = {3,0};
  position six = {3,3};
  position seven = {5,6};
  position eight = {5,2};
  Block a("cactus", cactus, one); //flip()
  a.rotate();
  a.rotate();
  a.flip();
  a.displayBlock();
  Block b("corner", corner, two); //rot()
  b.rotate();
  b.displayBlock();
  Block c("z", z, three); //rot()
  c.rotate();
  c.displayBlock();
  Block d("chair", chair, four); // flip, rot 3 times
  d.rotate();
  d.rotate();
  d.rotate();
  d.flip();
  d.displayBlock();
  Block e("horseshoe", horseshoe, five);
  Block f("lightning", lightning, six);
  f.rotate();
  f.rotate();
  f.rotate();
  f.flip();
  f.displayBlock();
  Block g("L", L, seven);
  g.rotate();
  g.flip();
  g.displayBlock();
  Block h("rec", rec, eight);
  h.rotate();
  h.displayBlock();

  board->placeBlock(a);
  board->placeBlock(b);
  board->placeBlock(c);
  board->placeBlock(d);
  board->placeBlock(e);
  board->placeBlock(f);
  board->placeBlock(g);
  board->placeBlock(h);

  if(board->checkWin()) {
    for (Block b : *board->placed) {
      b.displayBlock();
    }
    return 1;
  }
  cout << "no solution found" << "\n";
  return -1;
};
*/
