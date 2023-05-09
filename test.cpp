
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

