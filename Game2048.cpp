#include<iostream>
#include<fstream>
#include <bits/stdc++.h> 
#include "ArgumentManager.h"

using namespace std;

//class definition
class Game2048 {
  int size, max, movecnt;
  bool merged;
  ifstream infile;
  stack<int> **grid = NULL;
  stack<int> **backGrid = NULL;
  stack<int> *elem =  NULL;
  string path;
 
  public:
  int dir;
  bool exit;
  ofstream outfile;

  Game2048(int, string);
  ~ Game2048();
  void initGrid(int*);
  void playGame();
  void move();
  void undo();
  void backupGrid();
  void mergeGrid();
  void displayGrid();
  void displayHead(const char*);
  void readFile(string);
  bool gridSolved();
  bool ischangedGrid();
};

//constreuctor
Game2048 :: Game2048(int s, string file):size(s), max(2048){
    
  grid = new stack<int>*[this -> size];
  for (int i = 0; i < this -> size; ++i) {
    grid[i] = new stack<int>[this -> size];
  }
  
  backGrid = new stack<int>*[this -> size];
  for (int i = 0; i < this -> size; ++i) {
    backGrid[i] = new stack<int>[this -> size];
  }

  elem = new stack<int>[this -> size*this -> size];
  this -> exit = false;
  this -> outfile.open(file);
  this -> movecnt = 0;
}

// destructor
Game2048 :: ~Game2048() {
  this -> outfile.close();
}

// function to read file content
void Game2048 :: readFile(string file) {
  infile.open(file);

  if (!infile) {
    cout << "Error: File \"" << file << "\" could not be opened." << endl;
    //exit(1);
  }

  int cnt = 0;

  int n, *elem = NULL;

  elem = new int [this -> size * this -> size]{0};

  while(infile >> n) {
    
    if (cnt > 0) {
      elem[cnt-1] = n;
    }
    if (cnt >= this -> size * this -> size) {
      break;
    }
    cnt++;
  }
  this -> infile.close();
  this -> initGrid(elem);
}

// Funtion to initialize the Grid
void Game2048 :: initGrid(int* elem) {

  for (int i = 0; i < this -> size * this -> size; i++) {
    this -> elem[i].push(elem[i]);
  }
  
  for (int i = 0; i < this -> size; i++){
    for (int j = 0; j < this -> size; j++){
      this -> grid[i][j].push(elem[((this -> size-1)*i)+(i+j)]); 
    }
  }
}

void Game2048 :: displayGrid() {
  
  cout << endl << right;
  
  for (int i = 0; i < this -> size; i++) {
    
    for (int a = 0; a< this -> size; a++) {
      cout << "+----";
    }
    
    cout << "+" << endl;
    cout << "|";

    for (int j = 0; j < this -> size; j++) {
      if (grid[i][j].top() > 999) {
        cout << setw(4) << grid[i][j].top() << "|";
      } else if (grid[i][j].top() > 99) {
        cout << setw(2) << grid[i][j].top() << setw(2) << "|";
      } else if (grid[i][j].top() > 9) {
        cout << setw(3) << grid[i][j].top() << setw(2) << "|";
      } else {
        cout << setw(2) << grid[i][j].top() << setw(3) << "|";
      }
    }
    cout << endl;
  }
  for (int a = 0; a< this -> size; a++) {
    cout << "+----";
  }
  cout << "+" << endl;
}

// funtion to display the title
void Game2048 :: displayHead(const char *head) {
  int totalLength = this -> size * 5;
  int halfspace = (totalLength - strlen(head))/2;
  
  for (int i = 0; i < this -> size; i++) {
    cout << "+----";
  }
  
  cout <<"+" << endl << "|";
  
  for (int i = 0; i < halfspace; i++) {
    cout << " ";
  }

  cout << left << setw(totalLength - halfspace - 1) << head << "|" << endl;

  for (int i = 0; i < this -> size; i++) {
    cout << "+----";
  }
  cout << "+" << endl;
}

// funtion to make backup of a grid
void Game2048 :: backupGrid() {
  for (int i = 0; i < this -> size; i++) {
    for (int j = 0; j < this -> size; j++) {
      backGrid[i][j].push(grid[i][j].top());
    }
  }
}

// funtion to undo the move/merge
void Game2048 :: undo() {
  for (int i = 0; i < this -> size; i++) {
    for (int j = 0; j < this -> size; j++) {
      grid[i][j].push(backGrid[i][j].top());
    }
  }
}

// function to move the cell
void Game2048 :: move() {

  switch (this -> dir) {
    case 1:
    for (int i = 0; i < this -> size; i++) {
      for (int j = 0; j < this -> size; j++) {
        if (!grid[j][i].top()) {
          for (int k = j + 1; k < this -> size; k++) {
            bool brk = false;
            for (int l = k; l >= 0; l--) {
              if (grid[k][i].top() == this -> max) {
                brk = true;
                break;
              }
            }
            if (brk) break;
            if (grid[k][i].top() && grid[k][i].top() != this -> max) {
              grid[j][i].push(grid[k][i].top());
              grid[k][i].push(0);
              break;
            }
          }
        }
      }
    }
    break;

    case 2:
    for (int i = 0; i < size; i++) {
      for (int j = size - 1; j >= 0; j--) {
        if (!grid[i][j].top()) {
          for (int k = j - 1; k >= 0; k--) {
            bool brk = false;
            for (int l = k; l < j; l++) {
              if (grid[i][l].top() == this -> max) {
                brk = true;
                break;
              }
            }
            if (brk) break;
            if (grid[i][k].top() && grid[i][k].top() != this -> max) {
              grid[i][j].push(grid[i][k].top());
              grid[i][k].push(0);
              break;
            }
          }
        }
      }
    }
    break;

    case 3:
    for (int i = 0; i < this -> size; i++) {
      for (int j = size - 1; j >= 0; j--) {
        if (!grid[j][i].top()) {
          for (int k = j - 1; k >= 0; k--) {
            bool brk = false;
            for (int l = k; l < j; l++) {
              if (grid[l][i].top() == this -> max) {
                brk = true;
                break;
              }
            }
            if (brk) break;
            if (grid[k][i].top() && grid[k][i].top() != this -> max) {
              grid[j][i].push(grid[k][i].top());
              grid[k][i].push(0);
              break;
            }
          }
        }
      }
    }
    break;

    case 4:
    for (int i = 0; i < this -> size; i++) {
      for (int j = 0; j < this -> size; j++) {
        if (!grid[i][j].top()) {
          for (int k = j + 1; k < this -> size; k++) {
            bool brk = false;
            for (int l = k; l >= 0; l--) {
              if (grid[i][k].top() == this -> max) {
                brk = true;
                break;
              }
            }
            if (brk) break;
            if (grid[i][k].top()) {
              grid[i][j].push(grid[i][k].top());
              grid[i][k].push(0);
              break;
            }
          }
        }
      }
    }
    break;
  }
}

// funtion to merge the grid
void Game2048 :: mergeGrid() {
  this -> merged = false;
  switch (this -> dir) {
    case 1:
    for (int i = 0; i < this -> size; i++) {
      for (int j = 0; j < this -> size - 1; j++) {
        if (grid[j][i].top() && grid[j][i].top() == grid[j + 1][i].top() && grid[j][i].top() != this -> max) {
          grid[j][i].push(2*grid[j + 1][i].top());
          grid[j + 1][i].push(0);
          this -> merged = true;
        }
      }
    }
    break;

    case 2:
    for (int i = 0; i < this -> size; i++) {
      for (int j = this -> size - 1; j > 0; j--) {
        if (grid[i][j].top() && grid[i][j].top() == grid[i][j - 1].top() && grid[i][j].top() != this -> max) {
          grid[i][j].push(2*grid[i][j - 1].top());
          grid[i][j - 1].push(0);
          this -> merged = true;
        }
      }
    }
    break;

    case 3:
    for (int i = 0; i < this -> size; i++) {
      for (int j = this -> size - 1; j > 0; j--) {
        if (grid[j][i].top() && grid[j][i].top() == grid[j - 1][i].top()  && grid[j][i].top() != this -> max) {
          grid[j][i].push(2*grid[j - 1][i].top());
          grid[j - 1][i].push(0);
          this -> merged = true;
        }
      }
    }
    break;

    case 4:
    for (int i = 0; i < this -> size; i++) {
      for (int j = 0; j < this -> size - 1; j++) {
        if (grid[i][j].top() && grid[i][j].top() == grid[i][j + 1].top() && grid[i][j].top() != this -> max) {
          grid[i][j].push(2*grid[i][j].top());
          grid[i][j + 1].push(0);
          this -> merged = true;
        }
      }
    }
    break;
  }
}

// funtion to know either grid is solved completely or not
bool Game2048 :: gridSolved() {
  int unmergedNum = 0;
  bool gridMerged = true;
  for (int i = 0; i < this -> size; i++) {
    for (int j = 0; j < this -> size; j++) {
      if (grid[i][j].top() != this -> max && grid[i][j].top()) {
        unmergedNum++;
        if (unmergedNum >= 2) {
          gridMerged = false;
          break;
        }
      }
    }
  }
  return gridMerged;
}

bool Game2048 :: ischangedGrid() {
  for (int i = 0; i < this -> size; i++) {
    for (int j = 0; j < this -> size; j++) {
      if (grid[i][j].top() != backGrid[i][j].top())
        return true;
    }
  }
  return false;
}

// funtion to play the game
void Game2048 :: playGame() {
  
  int tempdir;

  this -> backupGrid();
  this -> move();
  this -> mergeGrid();
  this -> move();
  
  
  if (this -> ischangedGrid()) {
    this -> movecnt++;
  }

  // if not merged, undo the move and try fron another direction
  if (!this -> merged) {
    this -> undo();
    this -> dir++;
    if (this -> dir > 4) {
      this -> dir = 1;
    }
  } else {
    tempdir = dir;
    if (dir == 4) { // gives priority to right if both case mergeable
      this -> move();
      this -> mergeGrid();
      this -> move();
      if (this -> merged) {
        this -> undo();
        dir = 2;
        this -> move();
        this -> mergeGrid();
        this -> move();
        
      }
    }
    
    //if merged, generate output to the file and display the grid
    this -> outfile << this -> dir;
    
    if (this -> dir == 1){
      cout << endl << "MOVE: UP";
    } else if (this -> dir == 2){
      cout << endl << "MOVE: RIGHT";
    } else if (this -> dir == 3){
      cout << endl << "MOVE: DOWN";
    } else if (this -> dir == 4){
      cout << endl << "MOVE: LEFT";
    }
    
    if (tempdir) dir = 1;
    
    this -> displayGrid();
    
    this -> dir = 1; // gives priority to UP direction for each move
  }

  // if there is no possible merge remaining, exit
  if (this -> movecnt >= 4 && ! this -> ischangedGrid()) {
    this -> exit = true;
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "Invalid parameters" << endl;
    exit(EXIT_FAILURE); // terminate with error
  }

  ArgumentManager am(argc, argv);
    
  string infilename = am.get("input");// get the input filename
  string outfilename = am.get("output"); // get the output filename

  int dim;

  // to find the size of GRID
  ifstream infile;
  infile.open(infilename);
  
  while(infile >> dim) {
    break;
  }
  infile.close();

  // create pointer object dynamically
  Game2048 *game = new Game2048(dim, outfilename);

  game -> readFile(infilename);
  game -> displayHead("Original Grid");
  game -> displayGrid();
  
  cout<<endl;
  
  int cnt = 0;
  
  game -> dir = 4;

  // play the game until game is not exit
  while(!game -> exit) {
    
    game -> playGame();
    
    cnt ++;

    // to avoid the uncertain ifinity loop
    if (cnt > 12) {
      break;
    }
  }

  cout<<endl<<endl;
  
  game -> displayHead("Final Grid");
  game -> displayGrid();
  
  // if given grid is not merged completely, output 'Impossible'
  if (!game -> gridSolved()) {
    game -> outfile.close();
    game -> outfile.open(outfilename, ofstream::out | ofstream::trunc);
    game -> outfile << "Impossible";
  }
  delete game;
  return 0;
}