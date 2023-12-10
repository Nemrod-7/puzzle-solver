#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

struct point { int x,y ; };

int N = 0;

class Display {
    public :
        static string board (vector<int> &grid) {
            stringstream oss;

            for (int i = 0; i < grid.size(); i++) {
                oss << setw(3) << (int) grid[i];
                if (i % N == N - 1) oss << endl;
            }

            oss << endl;
            return oss.str();
        }
        static void point (const point &p) {
            cout << "[" << p.x << "," << p.y << "]\n";
        }
};
void print (string src) {
    fstream ios ("notes", std::ios::app);
    ios << src;
    ios.close();
}

point locate (const vector<int> &grid, int num) { // locate tile

    for (int i = 0; i < grid.size(); i++) {
        if (grid[i] == num) {
            return {i % N, i / N};
        }
    }

    return {0,0};
}

void lateral (vector<int> &grid, point &p, int col) {
    point nxt;

    while (p.x != col) {
        nxt = p;
        if (col < p.x) {
            p.x--;
        } else if (col > p.x) {
            p.x++;
        }
        swap(grid[p.y * N + p.x], grid[nxt.y * N + nxt.x]);
    }
}
void vertical (vector<int> &grid, point &now, int row) {
    point nxt;

    while (now.y != row) {
        nxt = now;
        if (row < now.y) {
            now.y--;
        } else if (row > now.y) {
            now.y++;
        }
        swap(grid[nxt.y * N + nxt.x], grid[now.y * N + now.x]);
    }
}

void moveup (vector<int> &grid, int tile) {
    point now = locate(grid, 0);
    point num = locate(grid, tile);
    int row = tile / N;

    while (num.y != row) {

      if (num.y < now.y) { // if the slider is under the tile
            if (num.x > 0) { // move to the left
                lateral (grid, now, num.x - 1);
            } else { // else to the right
                lateral (grid, now, num.x + 1);
            }
        } else if (num.y == now.y) {
            if (num.x > now.x) {
                lateral(grid, now, num.x - 1);
            } else {
                lateral(grid, now, num.x + 1);
            }
        }

        if (now.y > 0) { // move up
            vertical (grid, now, num.y - 1);
        }
        if (num.x != now.x) {
            lateral (grid, now, num.x);
        }
        vertical (grid, now, num.y);

        num = locate(grid,tile);
    }
}

void prototype (vector<int> grid) {

  vector<int> index (N * N);
  vector<point> p (N * N);
  for (int i = 0; i < grid.size(); i++) {
      index[grid[i]] = i;
      p[i].x = i % N, p[i].y = i / N;
  }
      int slide = 0;
      int x = p[index[slide]].x, y = p[index[slide]].y;

      int tile = 8;
      int col = p[index[tile]].x, row = p[index[tile]].y;

      while (x != col) {
          int nxt = 0;

          if (col < x) {
              index[slide]--;
              nxt = grid[index[slide]], index[nxt]++;
          } else if (col > x) {
              index[slide]++;
              nxt = grid[index[slide]], index[nxt]--;
          }

          x = p[index[0]].x;
          swap(grid[index[nxt]], grid[index[slide]]);
      }
}


int main () {

    N = 6;
    vector<int> grid = {16,12,1,14,0,13,5,27,29,17,21,23,31,26,33,2,20,4,15,8,30,32,35,24,28,9,19,3,34,7,6,10,18,22,11,25};

    vector<int> index (N * N);
    vector<point> p (N * N);
    vector<int> x (N * N), y (N * N);

    int half = (N * N) / 2 ;

    for (int i = 0; i < grid.size(); i++) {
        index[grid[i]] = i;
        x[i] = i % N, y[i] = i / N;
    }



    /*
    */
    cout << Display::board(grid);
    // cout << index[slide] << " " << "\n";

}
