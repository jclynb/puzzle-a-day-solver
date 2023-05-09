#include <iostream>
#include <vector>
#include "solver.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "incorrect input, type a month and date" << "\n";
        return -1;
    }

    std::string month = argv[1];
    std::string day = argv[2];

    Solver solver;
    solver.findSolution(month, day);
    
    return 1;
};