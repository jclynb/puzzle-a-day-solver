#include "solver.h"
#include "block.h"
#include "board.h"

Solver::Solver() : mtx(),
                   cv(),
                   solved(false),
                   exit_program(false)
{}

int Solver::findSolution(std::string month, std::string day) {
    // Spawn threads to solve the puzzle in parallel
    std::vector<std::thread> threads;
    int num_threads = std::thread::hardware_concurrency();
    for (int i = 0; i < num_threads; i++) {
        int start_blk = i * 8 / num_threads;
        int end_blk = (i + 1) * 8 / num_threads;
        threads.emplace_back(&Solver::dfs, this, month, day, start_blk, end_blk);
    }

    // Wait for the first thread to complete
    std::unique_lock<std::mutex> lock(this->mtx);
    this->cv.wait(lock, [this] { return this->solved; });

    // Set a flag to signal to other threads to exit
    this->exit_program = true;

    // Join all remaining threads
    lock.unlock();
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    return 0;
};

void Solver::dfs(std::string month, std::string day, int start, int end) {
// DFS approach:
// Start by adding an empty board to the stack.
// Pop the board, and check for win condition.
// Else, loop through the board's available blocks and avaiable positions.
// Try placing a given block at a given board position. If it's a valid placement, add the new board with this new valid placement to the stack.
// Next try placing rotations/flips of the same block. If any of those placements are valid, add those new boards to stack.
// Repeat until checkWin is True.
    auto start_time = std::chrono::high_resolution_clock::now(); // Start measuring time

    // Block positions (x, y) oriented at top left labeled "2"
    std::vector<std::vector<char>> rec = {
        {2, 1},
        {1, 1},
        {1, 1}
    };
    std::vector<std::vector<char>> z = {
        {2, 1, 0},
        {0, 1, 0},
        {0, 1, 1}
    };
    std::vector<std::vector<char>> L = {
        {2, 0},
        {1, 0},
        {1, 0},
        {1, 1}
    };
    std::vector<std::vector<char>> corner = {
        {2, 0, 0},
        {1, 0, 0},
        {1, 1, 1}
    };
    std::vector<std::vector<char>> lightning = {
        {2, 0},
        {1, 1},
        {0, 1},
        {0, 1}
    };
    std::vector<std::vector<char>> horseshoe = {
        {2, 0, 1},
        {1, 1, 1}
    };
    std::vector<std::vector<char>> cactus = {
        {2, 0},
        {1, 1},
        {1, 0},
        {1, 0}
    };
    std::vector<std::vector<char>> chair = {
        {2, 0},
        {1, 1},
        {1, 1}
    };
    std::unordered_map<std::string, std::vector<std::vector<char>>> block_map = {
        {"rec", rec},
        {"z", z},
        {"L", L},
        {"corner", corner},
        {"lightning", lightning},
        {"horseshoe", horseshoe},
        {"cactus", cactus},
        {"chair", chair}
    };
    std::vector<std::string> asymmetrical = {"cactus", "chair", "lightning", "L", "z"}; // Asymmetrical blocks can be flipped
    
    // Start DFS
    auto board = std::make_shared<Board>();
    board->initPositions(month, day);
    if (board->available_positions.size() != 41) {
        std::cerr << "Please enter a valid month and date from the game board." << "\n";
        exit(1); 
    }

    bool finished_designated_blocks = false;

    std::stack<std::shared_ptr<Board>> s;
    s.push(board);

    while (!s.empty()) {
        auto board = s.top();
        s.pop();
        // For threading: make sure each search starts with the thread designated blocks before looping through the rest of available blocks
        // Once finished_designated_blocks is true, the boolean stays true.
        finished_designated_blocks = board->available_blocks.size() <= 8 - (end-start) ? true : finished_designated_blocks;
        if (finished_designated_blocks) {
            start = 0;
            end = board->available_blocks.size();
        }

        if (board->checkWin()) {
            // Calculate duration
            auto end_time = std::chrono::high_resolution_clock::now(); // Stop measuring time
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
            // Print duration and solution
            std::cout << "Found a solution in " << duration.count() << " seconds" << std::endl;
            for (Block b : board->placed) {
                b.displayBlock();
            }
            // Notify the main thread that a solution was found
            std::unique_lock<std::mutex> lock(this->mtx);
            this->solved = true;
            this->cv.notify_all();
            lock.unlock();

            exit(0);
        }

        for (int b = start; b < end; b++) {
            for (const auto pos : board->available_positions) {
                std::string block_id = board->available_blocks.at(b);
                auto blk = std::make_shared<Block>(block_id, block_map[block_id], pos);
                auto new_board_1 = std::make_shared<Board>(*board);
                if (new_board_1->placeBlock(*blk)) {
                    assert(new_board_1->available_blocks.size() == board->available_blocks.size() - 1);
                    s.push(new_board_1);
                }
                // rotate 3 times
                for (int i = 0; i < 3; i++) {
                    blk->rotate();
                    auto new_board_2 = std::make_shared<Board>(*board);
                    if (new_board_2->placeBlock(*blk)) {
                        assert(new_board_2->available_blocks.size() == board->available_blocks.size() - 1);
                        s.push(new_board_2);
                    }
                }
                // only need to flip asymmetrical blocks
                if (find(asymmetrical.begin(), asymmetrical.end(), blk->getID()) != asymmetrical.end()) {
                    blk->flip();
                    auto new_board_3 = std::make_shared<Board>(*board);
                    if (new_board_3->placeBlock(*blk)) {
                        assert(new_board_3->available_blocks.size() == board->available_blocks.size() - 1);
                        s.push(new_board_3);
                    }
                    // rotate 3 times
                    for (int i = 0; i < 3; i++) {
                        blk->rotate();
                        auto new_board_4 = std::make_shared<Board>(*board);
                        if (new_board_4->placeBlock(*blk)) {
                            assert(new_board_4->available_blocks.size() == board->available_blocks.size() - 1);
                            s.push(new_board_4);
                        }
                    }
                }
            }
        }
        // Check if the program should exit
        std::unique_lock<std::mutex> lock(mtx);
        if (this->exit_program) {
            lock.unlock();
            exit(0);
        }
        lock.unlock();
    }
    std::cout << "no solution found" << "\n";
    exit(0);
};
