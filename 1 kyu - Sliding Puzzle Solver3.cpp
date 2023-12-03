#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <tuple>
#include <limits>

#include <random>
#include <chrono>

using namespace std;
//grid = string v;
using point = pair<int,int>;
using vertex = tuple<int, point, string>;

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
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
};

const vector<point> compass {{1,0},{0,1},{-1,0},{0,-1}};
const int found = -2, infinity = numeric_limits<int>::max();

point operator+ (const point& a, const point& b) {
    return {a.first + b.first, a.second + b.second};
}

int N = 0;

bool is_inside (const point &p) { // check if point is inside the grid
    return p.first >= 0 && p.second >= 0 && p.first < N && p.second < N;
  }
bool is_line (const string &grid, int sweep) { // check for complete line
    int size = grid.size(), y = sweep * N;

    for (int x = 0; x < N; x++) {
        if (grid[y + x] != (y + x + 1))
            return false;
    }

    return true;
}

point locate (const string &curr, int num) { // locate tile

    for (int i = 0; i < curr.size(); i++) {
        if (curr[i] == num) {
            return {i % N, i / N};
        }
    }

    return {0,0};
}

int distance (const point &a, const point &b) {
  return abs (a.first - b.first) + abs (a.second - b.second);
}
int hamming (const string &curr) { // sum of misplaced tiles distance
    int sum = 0;

    for (int i = 0; i < curr.size(); i++) {
        if (curr[i] != 0) {
            int j = curr[i] - 1;
            sum += abs (i % N - j % N) + abs (i / N - j / N);
            //point a = make_pair (i % N, i / N), b = make_pair (j % N, j / N);
            //sum += distance (a, b);
        }
    }

    return sum;
}
int linearcflt (const string &grid) {
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
    /*
    for (int i = 0; i < size; i++) {
        if (incol[i]) cout << grid[i] << " " ;
    }
    */
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
            /*
            */
            }

        }

    //cout << endl;
    }
    return cnt;
}

void a_star (vertex start) {

    int cycle = 0, line = 0;
    //vertex start = {hamming (maze), locate (maze, 0), maze};
    priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
    map<string, bool> visited;

    q1.push(start);

    while (!q1.empty()) {
        auto [state, u, grid] = q1.top();
        q1.pop();

        if (is_line (grid, line)) line++;
        if (u.second < line) continue;

        if (state == 0) {
            Display::board (grid);
            cout << "::" << cycle << "::\n";
            break;
        }

        visited[grid] = true;
        cycle++;

        for (auto &dir : compass) {
            point nxt = u + dir;

            if (is_inside (nxt)) {
                int ida = u.second * N + u.first, idb = nxt.second * N + nxt.first;

                swap (grid[ida], grid[idb]);

                int alt = hamming (grid)  + linearcflt (grid) * 1.5 ;

                if (!visited[grid]) {
                    vertex nextv {alt, nxt, grid};
                    q1.push(nextv);
                }

                swap (grid[ida], grid[idb]);
            }
        }
    }

}

int search (stack<vertex> &path, int g, int maxdepth) {

    auto [state,last,grid] = path.top();
    int h = hamming (grid);
    int f = g + h;

    if (f > maxdepth) return f;
    if (h == 0) {

        return f;
    }

    Display::board(grid);

    int minv = numeric_limits<int>::max();

    for (auto &dir : compass) {
        point nxt = last + dir;

        if (is_inside (nxt)) {

            int ida = last.second * N + last.first, idb = nxt.second * N + nxt.first;

            swap (grid[ida], grid[idb]);

            int alt = hamming (grid);

            vertex nextv {alt, nxt, grid};
            //f = search (nextv, g + alt, maxdepth);
            if (f < minv) minv = f;

            swap (grid[ida], grid[idb]);
        }
    }

    return minv;
}
int ida_star (const string &grid) {

    int maxdepth = hamming (grid);
    vertex start = {maxdepth, locate (grid, 0), grid};
    stack<vertex> path;

    path.push (start);

    int cycle = 5;


    while (cycle-->0) {
        int t = search (path, 0, maxdepth);
        //cout << visit.size() << " ";
        if (t == -1) break;
        if (t == numeric_limits<int>::max()) return -1;
        maxdepth = t;
    }

    /*
    */
    return -1;
}

vector<int> sliding (vector<vector<int>> input) {
    N = input.size();
    vector<int> solution;
    string grid;
    point slider;

    for (int y = 0; y < N; y++){
        for (int x = 0; x < N; x++) {
            grid += (input[y][x]);
            if (input[y][x] == 0) slider = {x,y};
        }
    }
    vertex start = {hamming (grid), slider, grid};
    int max_depth = 3;

    a_star (start);
    return solution;
}

int main () {
    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};
    // sliding (grid);
    //grid = {{1,8,7}, {3,0,5},{4,6,2}};
    grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};
    sliding (grid);

    grid = {{1,14,2,4,6,18},{9,13,3,17,11,33},{19,7,16,10,5,12},{8,26,20,15,22,24},{21,31,27,29,23,30},{25,0,32,28,34,35}};
    // sliding (grid);

    /*
    */
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
