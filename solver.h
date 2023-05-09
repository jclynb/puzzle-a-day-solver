#ifndef SOLVER
#define SOLVER

#include <iostream>
#include <string>
#include <chrono>
#include <stack>
#include <vector>
#include <memory>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>

class Solver {
public:
    Solver();
    int findSolution(std::string month, std::string day);

private: 
    void dfs(std::string month, std::string day, int start, int end);
    std::mutex mtx;
    std::condition_variable cv;
    bool solved;
    bool exit_program;
};

#endif