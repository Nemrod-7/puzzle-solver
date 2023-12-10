#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

#include <stack>
#include <queue>
#include <tuple>

#include <random>

using namespace std;

struct point { int x,y; };

class board {
      public :
          int N;
          point slider;
          vector<int> grid, index, x, y;

      board (vector<vector<int>> data = {}) {
          N = data.size();
          index.resize(N * N), x.resize(N * N), y.resize(N * N);
          int ix ;

          for (int i = 0; i < N; i++) {
              for (int j = 0; j < N; j++) {
                  ix = i * N + j;

                  index[data[i][j]] = ix;
                  x[ix] = j, y[ix] = i;
                  grid.push_back(data[i][j]);

                  if (data[i][j] == 0) slider = {j,i};
              }
          }
      }

      bool is_inside (point p) { return p.x >= 0 && p.y >= 0 && p.x < N && p.y < N; }
      int locate (int num) { return index[num]; }
      int &operator [] (point p) { return grid[p.y * N + p.x]; }

};

class Display {
    public :
        static void board (vector<int> grid) {
            int N = sqrt (grid.size());
            for (int i = 0; i < grid.size(); i++) {
                cout << setw(3) << grid[i];
                if (i % N == N - 1) cout << endl;
            }

            cout << endl;
        }
        static void point (const point &p) {
            cout << "[" << p.x << "," << p.y << "]\n";
        }
};

const vector<point> compass {{1,0},{0,1},{-1,0},{0,-1}};
point operator + (const point& a, const point& b) {
    return {a.x + b.x, a.y + b.y};
}

int distance (const point &a, const point &b) { return abs(a.x - b.x) + abs (a.y - b.y);}

int main () {

    vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};

    grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};

    board puzzle (grid);
    // int num = 1;
    cout << puzzle.locate(0) << "\n";
    // point dto = locate (puzzle, num);
    // point dest = {(num - 1) % puzzle.N, (num - 1) / puzzle.N};

    Display::board (puzzle.grid);

}
