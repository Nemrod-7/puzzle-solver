#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
//#include <cmath>
#include <stack>
#include <queue>
#include <tuple>

#include <random>
#include <chrono>

using namespace std;
// grid = std::vector<int> v;
struct point { int x, y; };
// using point = pair<int,int>;

using vertex = tuple<int, point, vector<int>>;

class Display {
    public :
        static void board (vector<int> grid) {
            int N = sqrt (grid.size());
            for (int i = 0; i < grid.size(); i++) {
                if (grid[i] == 0) {
                  cout << setw(3) << 'X';
                } else {
                  cout << setw(3) << grid[i];
                }
                if (i % N == N - 1) cout << endl;
            }

            cout << endl;
        }
        static void point (const point &p) {
            cout << "[" << p.x << "," << p.y << "]\n";
        }
};
////////////////////////////////////////////////////////////////////////////////
const vector<point> compass {{1,0},{0,1},{-1,0},{0,-1}};
const point operator + (const point& a, const point& b) { return {a.x + b.x, a.y + b.y}; }
const bool operator < (const point &a, const point &b) { return (a.x == b.x) ? a.y < b.y : a.x < b.x; }

int N = 0;

int distance (const point &a, const point &b) { /* manhattan distanc e*/ return abs (a.x - b.x) + abs (a.y - b.y); }
bool is_inside (point p) { return p.x >= 0 && p.y >= 0 && p.x < N && p.y < N; }

point locate (const vector<int> &curr, int num) { // locate tile

    for (int i = 0; i < curr.size(); i++) {
        if (curr[i] == num) {
            return {i % N, i / N};
        }
    }

    return {0,0};
}

int hamming (const vector<int> &curr) { // sum of misplaced tiles manhattan distance
    int sum = 0;

    for (int i = 0; i < curr.size(); i++) {
        if (curr[i] != 0) {
            int j = curr[i] - 1;
            sum += abs (i % N - j % N) + abs (i / N - j / N);
            //point a = make_pair (i % N, i / N), b = make_pair (j % N, j / N);
            //sum += distance a, b);
        }
    }

    return sum;
}
int linearcflt2 (const vector<int> &grid) {
    int size = grid.size(), cnt = 0;
    int x, y;
    int icol = 0, irow = 0;

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            int i = y * N + x;
            int bx = (grid[i] - 1) % N, by = (grid[i] - 1) / N;

            if (bx == x) icol |= 1 << i;
            if (by == y) irow |= 1 << i;
        }
    }

    for (y = 0; y < N; y++) { // check rows
        for (x = 0; x < N; x++) {
            int j = y * N + x, num = j + 1;

            for (int r = x + 1; r < N; r++) {
                int k = y * N + r;

                if (grid[j] && grid[k] && grid[j] != j + 1 && grid[k] != k + 1
                    && (irow >> j &1) && (irow >> k &1) ) {
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
                    && (icol >> j &1)  && (icol >> k &1) ) {

                    if (grid[j] > grid[k] && j < k) cnt++;
                }
            }

        }
    }

    return cnt;
}

bool is_line (const vector<int> &grid, int sweep) { // check for complete line
    int size = grid.size(), y = sweep * N;

    for (int x = 0; x < N; x++) {
        if (grid[y + x] != (y + x + 1))
            return false;
    }

    return true;
}

void a_star (vertex start) {

    int cycle = 0, line = 0;
    // vertex start = {hamming (maze), locate (maze, 0), maze};
    priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
    map<vector<int>,bool> visited;

    q1.push(start);

    while (!q1.empty()) {
        auto [state, now, grid] = q1.top();
        q1.pop();

        if (is_line (grid, line)) {
            line++;
            // Display::board (grid);
            // cout << "::" << cycle << "::\n";
            // break;
        }
        if (now.y < line) continue;

        if (state == 0) {
            Display::board (grid);
            cout << "::" << cycle << "::\n";
            break;
        }

        visited[grid] = true;
        cycle++;

        for (auto &dir : compass) {
            point nxt = now + dir;

            if (is_inside (nxt)) {
                int ida = now.y * N + now.x, idb = nxt.y * N + nxt.x;

                swap (grid[ida], grid[idb]);

                int alt = hamming (grid);//  + linearcflt2 (grid) /* * 1.5 */;
                // alt = hamming(grid) + visited.size();

                if (!visited[grid]) {
                    vertex nextv {alt, nxt, grid};
                    q1.push(nextv);
                }

                swap (grid[ida], grid[idb]);
            }
        }
    }
}
vector<int> sliding (vector<vector<int>> input) {
    N = input.size();
    vector<int> grid, solution;
    int it = 0;

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            grid.push_back (input[y][x]);
        }
    }

    // Display::board(grid);
    int num = 1;
    point slider = locate (grid, 0);

    vertex start = {hamming (grid), locate (grid, 0), grid};
    // cout << linearcflt1 (grid) << ' ' << linearcflt2 (grid) << "\n";
    // cout << hamming (grid);

    a_star (start);

    return solution;
}

int main () {
    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};
    grid = {{3,8,5},{0,7,1},{2,6,4}};

    sliding(grid);

    //grid = {{1,8,7}, {3,0,5},{4,6,2}};
    grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};
    // sliding (grid);
    grid = {{1,14,2,4,6,18},{9,13,3,17,11,33},{19,7,16,10,5,12},{8,26,20,15,22,24},{21,31,27,29,23,30},{25,0,32,28,34,35}};
    // sliding (grid);

    grid = {{15,6,18,24,8}, {9,0,12,17,16}, {13,5,3,22,7}, {2,21,11,4,19}, {23,14,10,20,1}};


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}

int linearcflt1 (const vector<int> &grid) {
    int size = grid.size(), cnt = 0;
    int x, y;
    vector<bool> incol (size), inrow (size);

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            int i = y * N + x;
            int bx = (grid[i] - 1) % N, by = (grid[i] - 1) / N;

            if (bx == x) incol[i] = true;
            if (by == y) inrow[i] = true;
        }
    }

    for (y = 0; y < N; y++) { // check rows
        for (x = 0; x < N; x++) {
            int j = y * N + x, num = j + 1;

            for (int r = x + 1; r < N; r++) {
                int k = y * N + r;

                if (grid[j] && grid[k] && grid[j] != j + 1 && grid[k] != k + 1 && inrow[j] && inrow[k]) {

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

                if (grid[j] && grid[k] && grid[j] != j + 1 && grid[k] != k + 1 && incol[j] && incol[k]) {
                    if (grid[j] > grid[k] && j < k) cnt++;
                }
            }

        }

    //cout << endl;
    }
    return cnt;
}
