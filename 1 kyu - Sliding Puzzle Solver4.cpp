#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

#include <stack>
#include <queue>
#include <tuple>

#include <random>
#include <chrono>

using namespace std;

using point = pair<int,int>;
using vertex = tuple<int, point, vector<int>>;

class board {

      public :
          int N;
          point slider;
          vector<int> path, grid;

      board (vector<vector<int>> data = {}) {
          N = data.size();

          for (int y = 0; y < N; y++) {
              for (int x = 0; x < N; x++) {
                  grid.push_back(data[y][x]);
                  if (data[y][x] == 0) slider = {x,y};
              }
          }
      }

      bool is_inside (point p) {
          return p.first >= 0 && p.second >= 0 && p.first < N && p.second < N;
      }
      int &operator [] (point p) {
          return grid[p.second * N + p.first];
      }
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
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
};

const vector<point> compass {{1,0},{0,1},{-1,0},{0,-1}};
point operator+ (const point& a, const point& b) {
    return {a.first + b.first, a.second + b.second};
}

int distance (const point &a, const point &b) { return abs(a.first - b.first) + abs (a.second - b.second);}

point locate (const board &curr, int num) { // locate tile

    for (int i = 0; i < curr.grid.size(); i++) {
        if (curr.grid[i] == num) {
            return {i % curr.N, i / curr.N};
        }
    }

    return {0,0};
  }
void move (board &puzzle, point dest, int num) {

    int N = puzzle.N;
    priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
    map<point, bool> visited;

    q1.push({999, puzzle.slider, puzzle.grid});

    while (!q1.empty()) {
        auto [dist, u, grid] = q1.top ();
        q1.pop();

        if (u == dest) {
            Display::board (grid);
            break;
        }
        visited[u] = true;

        for (auto &pos : compass) {
            point nxt = u + pos;
            int alt = distance (nxt, dest);
            int ida = u.second * N + u.first, idb = nxt.second * N + nxt.first;

            if (puzzle.is_inside (nxt) && grid[idb] != num) {
                vector<int> tmp = grid;
                swap (tmp[ida], tmp[idb]);
                q1.push ({alt, nxt, tmp});
            }
        }
    }
}
int main () {
    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};

    //grid = {{1,8,7}, {3,0,5},{4,6,2}};
    grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};

    board puzzle (grid);

    int num = 1;
    point dto = locate (puzzle, num);
    point dest = {(num - 1) % puzzle.N, (num - 1) / puzzle.N};

    move (puzzle, dest, num);
    //Display::board (puzzle.grid);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
