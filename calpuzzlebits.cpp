#include "calpuzzlebits.h"
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

bool board[64] = {1111111111000100000001000000010000000100000001100000011000000};
// Block positions (x, y) oriented at top left labeled 2

bool rec[6] = {111111};
bool z[9] = {110010011};
bool L[8] = {10101011};
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
