#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <limits>

#include <random>
#include <chrono>

using namespace std;

struct point {
    int x,y ;
};
struct vertex {
    int dist, move;
    point p;
    string grid;
};

struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return (a.dist == b.dist) ?  a.move > b.move : a.dist > b.dist;
    }
};

const point operator + (const point& a, const point& b) { return {a.x + b.x, a.y + b.y}; }
const bool operator < (const point &a, const point &b) { return (a.x == b.x) ? a.y < b.y : a.x < b.x; }
const vector<point> compass {{1,0},{0,1},{-1,0},{0,-1}};

int N = 0;

class Display {
    public :
        static void board (string grid) {
            int N = sqrt (grid.size());
            for (int i = 0; i < grid.size(); i++) {
                cout << setw(3) << (int) grid[i];
                if (i % N == N - 1) cout << endl;
            }

            cout << endl;
        }
        static void point (const point &p) {
            cout << "[" << p.x << "," << p.y << "]\n";
        }
};

bool is_inside (const point &p) { /* check if point is inside the grid */ return p.x >= 0 && p.y >= 0 && p.x < N && p.y < N; }
int distance (const point &a, const point &b) { /* manhattan distance */ return abs (a.x - b.x) + abs (a.y - b.y); }

point locate (const string &grid, int num) { // locate tile

    for (int i = 0; i < grid.size(); i++) {
        if (grid[i] == num) {
            return {i % N, i / N};
        }
    }

    return {0,0};
}

int inversions (const string &grid) { // inversion
    int inv = 0;

    for (int i = 0; i < grid.size() - 1; i++) {
        if (grid[i] == 0) continue;

        for (int j = i + 1; j < grid.size(); j++) {
            if (grid[j] == 0) continue;
            if (grid[i] > grid[j]) inv++;
        }
    }

    return inv;
}

int hamming (const string &grid) { // sum of misplaced tiles distances
    int sum = 0, dist;

    for (int i = 0; i < grid.size(); i++) {
        if (grid[i] != 0) {
            int j = grid[i] - 1;
            dist = abs (i % N - j % N) + abs (i / N - j / N);
            // cout <<  (N - i / N) << ' ';
            sum += dist * (N - i / N);
        }
    }

    return sum;
}
int linearcflt (const string &grid) {
    int x, y;
    int size = grid.size(), cnt = 0;
    unsigned long int icol = 0, irow = 0;

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            int i = y * N + x;
            int bx = (grid[i] - 1) % N, by = (grid[i] - 1) / N;

            if (bx == x) icol |= 1UL << i;
            if (by == y) irow |= 1UL << i;
        }
    }

    for (y = 0; y < N; y++) { // check rows
        for (x = 0; x < N; x++) {
            int j = y * N + x, num = j + 1;

            for (int r = x + 1; r < N; r++) {
                int k = y * N + r;

                if (grid[j] && grid[k] && grid[j] != j + 1 && grid[k] != k + 1
                        && (irow >> j &1UL) && (irow >> k &1UL) ) {

                    if (grid[j] > grid[k] && j < k) cnt++;
                }
            }
        }
    }

    for (x = 0; x < N; x++) { // check col
        for (y = 0; y < N; y++) {
            int j = y * N + x, num = j + 1;

            for (int r = y + 1; r < N; r++) {
                int k = r * N + x;

                if (grid[j] && grid[k] && grid[j] != j + 1 && grid[k] != k + 1
                        && (icol >> j &1UL)  && (icol >> k &1UL) ) {

                    if (grid[j] > grid[k] && j < k) cnt++;
                }
            }
        }
    }

    return cnt;
}

void a_star (vertex start) {
    int cycle = 0, line = 0;
    priority_queue<vertex, vector<vertex>, comp> q1;
    map<string, bool> visited;

    q1.push(start);

    while (!q1.empty()) {
        auto [state, moves, u, grid] = q1.top();
        q1.pop();

        if (state == 0) {
            Display::board (grid);
            cout << " -> cycles : " << cycle << " moves : " << moves <<  "\n";
            break;
        }
        cycle++;

        for (auto &dir : compass) {
            point nxt = u + dir;

            if (is_inside (nxt)) {
                int ida = u.y * N + u.x, idb = nxt.y * N + nxt.x;

                swap (grid[ida], grid[idb]);

                int alt = hamming (grid) /* + linearcflt (grid) * 1.5 */ ;

                if (!visited[grid]) {
                    vertex nextv {alt, moves + 1, nxt, grid};
                    visited[grid] = true;
                    q1.push(nextv);
                }

                swap (grid[ida], grid[idb]);
            }
        }
    }

}

bool is_solvable (string grid) {
    int inv = inversions(grid);

    if (N % 2 == 1) {
        return inv % 2;
    }

    return locate(grid, 0).y % 2 != inv % 2;
}
vector<int> sliding (vector<vector<int>> input) {
    vector<int> solution;
    string grid;
    point slider;

    N = input.size();
    for (int y = 0; y < N; y++){
        for (int x = 0; x < N; x++) {
            grid += (input[y][x]);
            if (input[y][x] == 0) slider = {x,y};
        }
    }

    if (!is_solvable(grid)) {
        //return {};
    }

    //Display::board(exp);
    vertex start = {hamming(grid), 0, slider, grid};
    a_star (start);

    return solution;
}

int main () {
    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};
    grid = {{1,8,7}, {3,0,5},{4,6,2}};

     grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};
     sliding (grid);

    // grid = {{1,14,2,4,6,18},{9,13,3,17,11,33},{19,7,16,10,5,12},{8,26,20,15,22,24},{21,31,27,29,23,30},{25,0,32,28,34,35}};
    // sliding (grid);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}

bool is_complete (const string &grid, int sweep) { // check for complete line
    int size = grid.size(), y = sweep * N;

    for (int x = 0; x < N; x++) {
        if (grid[y + x] != (y + x + 1))
            return false;
    }

    return true;
}

int hamming2 (const string &grid) {
  int sum = 0, dist, coef;

  for (int i = 0; i < grid.size(); i++) {
      if (grid[i] != 0) {
          int j = grid[i] - 1;
          dist = abs (i % N - j % N) + abs (i / N - j / N);

          // if (i / size == phase || i % size == phase) {
          //     coef = 2;
          // } else {
          //     coef = 1;
          // }
          sum += dist * coef;
      }
  }

  return sum;
}
