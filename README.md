# Puzzle-a-day Solver🧩

Solves [Puzzle-a-day](https://www.amazon.com/DragonFjord-Puzzle-Day-Original-Challenges/dp/B09BHV12QF?th=1) for a given date using depth-first search to place blocks on the game board until a winning combination is found.

This solver also uses a straightforward parallelization strategy to search large chunks of the search space with distinct cores. Flood-fill is used to minimize the search space. These additions reduced the search time from over an hour for specific dates to only minutes.

Locks are used to ensure thread safety and to allow the program to exit gracefully once the first thread finds a solution. The first thread sets a flag to signal to other threads to exit, and locks and condition variables are used to coordinate the exit of all threads.


## Run Locally

Clone the project

```bash
  git clone git@github.com:jclynb/puzzle-a-day-solver.git
```

Go to the project directory

```bash
  cd puzzle-a-day-solver
```

Run the program

```bash
  make
  ./solver Jan 1
```


## Board

![solved puzzle](https://m.media-amazon.com/images/I/61CMDUOroNL.jpg)

