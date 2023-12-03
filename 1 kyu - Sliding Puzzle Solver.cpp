#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <tuple>

#include <random>
#include <chrono>
// grid = vector<vector<int>>
using namespace std;

vector<vector<int>> goal {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};

using point = pair<int,int>;
using board = vector<vector<int>>;
using vertex = tuple<int, point, vector<vector<int>>>;

class Display {
    public :
        static void board (vector<vector<int>> grid) {

            for (int y = 0; y < grid.size(); y++){
                for (int x = 0; x < grid.size(); x++) {
                    cout << setw(3) << grid[y][x];
                }
                cout << endl;
            }
            cout << endl;
        }
        static void tile (const vector<vector<int>> &grid, pair<int,int> p) {
            cout << "[" << grid[p.second][p.first] << "]\n";
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

int manhattan (const vector<vector<int>> &grid) { // sum of misplaced tiles distance
    int sum = 0, N = grid.size();

    for (int y = 0; y < N; y++){
        for (int x = 0; x < N; x++) {

            if (grid[y][x] != 0) {
                int it = grid[y][x] - 1;
                sum += abs (x - it % N) + abs (y - it / N);
                //point p = make_pair (it % N, it / N);
                //sum += distance ({x,y}, p);
            }
        }
    }
    return sum;
}

bool is_complete (const vector<vector<int>> &grid, int y) { // completed line
    int index = y * grid.size() + 1;

    for (int x = 0; x < grid.size(); x++) {
        if (grid[y][x] != index++) return false;
    }

    return true;
}
bool is_valid (const vector<vector<int>> &grid) { // complete puzzle
  int index = 1;

  for (int y = 0; y < grid.size(); y++){
      if (is_complete (grid, y) == false) return false;
  }
  return true;
}

point locate (const vector<vector<int>> &grid, int num) { // locate tile

    for (int y = 0; y < grid.size(); y++){
        for (int x = 0; x < grid.size(); x++) {
            if (grid[y][x] == num) return {x,y};
        }
    }
    return {0,0};
}
bool is_inside (point p, int size) {
    return p.first >= 0 && p.second >= 0 && p.first < size && p.second < size;
}


int hamming (const vector<vector<int>> &grid) { // number misplaced tiles
  int index = 1, cnt = 0;
  for (int y = 0; y < grid.size(); y++){
    for (int x = 0; x < grid.size(); x++) {
      if (grid[y][x] != index++) cnt++;
    }
  }
  return cnt - 1;
}
int linearcflt (const vector<vector<int>> &grid) {
    int N = grid.size(), cnt = 0;
    int x, y;
    vector<bool> incol (N * N), inrow (N * N);

    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            int i = y * N + x;
            int bx = (grid[y][x] - 1) % N, by = (grid[y][x] - 1) / N;

            if (bx == x) incol[i] = true;
            if (by == y) inrow[i] = true;
        }
    }

    for (y = 0; y < N; y++) { // check rows
        for (x = 0; x < N; x++) {
            int j = y * N + x, num = j + 1;

            for (int r = x + 1; r < N; r++) {
                int k = y * N + r;

                if (grid[y][x] && grid[y][r] && grid[y][x] != j + 1 && grid[y][r] != k + 1 && inrow[j] && inrow[k]) {
                    if (grid[y][x] > grid[y][r] && j < k) cnt++;
                }

            }
        }
    }

    for (x = 0; x < N; x++) { // check col
        for (y = 0; y < N; y++) {
            int j = y * N + x, num = j + 1;
            for (int r = y + 1; r < N; r++) {
                int k = r * N + x;

                if (grid[y][x] && grid[r][x] && grid[y][x] != j + 1 && grid[r][x] != k + 1 && incol[j] && incol[k]) {
                    if (grid[y][x] > grid[r][x] && j < k) cnt++;
                }
            }
        }
    }

    return cnt;
}

int nLinearConflicts (const board &a, const board &b) {
  int N = a.size(), conflicts = 0;
  int pR[(N * N) + 1], pC[(N * N) + 1];

  for (int r = 0; r < N; r++) {
      for (int c = 0; c < N; c++) {
          pR[a[r][c]] = r;
          pC[a[r][c]] = c;
      }
  }

  // row conflicts - @checked_okay
  for (int r = 0; r < N; r++) {
      for (int cl = 0; cl < N; cl++) {
          //int num = r * N + cl + 1;
          for (int cr = cl + 1; cr < N; cr++) {
              //cout << b[r][cr] << ' ';
              if (b[r][cl] && b[r][cr] && r == pR[b[r][cl]] && pR[b[r][cl]] == pR[b[r][cr]] &&
                pC[b[r][cl]] > pC[b[r][cr]]) {

                  conflicts++;
              }
          }
      }
  }

    // column conflicts -
    for (int c = 0; c < N; c++) {
      for (int rU = 0; rU < N; rU++) {
        for (int rD = rU + 1; rD < N; rD++) {
          if (b[rU][c] && b[rD][c] && c == pC[b[rU][c]] && pC[b[rU][c]] == pC[b[rD][c]] &&
            pR[b[rU][c]] > pR[b[rD][c]]) {

              conflicts++;
            }
          }
        }
      }

      return conflicts;
    }

void a_star (vector<vector<int>> input) {
    const int N = input.size();
    int line = 0;
    vertex start {manhattan (input), locate (input, 0), input};

    //priority_queue<vertex> q1;
    priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
    map<vector<vector<int>>, bool> visited;


    q1.push(start);

    int cycle = 1;
    while (!q1.empty()) {
        auto [state, u, grid] = q1.top ();
        q1.pop();

        if (is_complete (grid, line)) line++;
        if (u.second < line || visited[grid]) continue;

        if (manhattan (grid) == 0) {
            Display::board(grid);
            cout << "::" << cycle << "::\n";
            break;
        }

        cycle++;
        visited[grid] = true;
        //cout << state << " ";
        for (auto &dir : compass) {
            point nxt = u + dir;

            if (is_inside (nxt, N)) {
                swap (grid[u.second][u.first], grid[nxt.second][nxt.first]);

                int alt = manhattan (grid) + linearcflt (grid) * 1.5 ;

                if (!visited[grid]) {
                    vertex nextv = {alt, nxt, grid};
                    q1.push (nextv);
                }

                swap (grid[u.second][u.first], grid[nxt.second][nxt.first]);
            }
        }
    }
    /*
*/
}

vector<int> sliding (vector<vector<int>> input) {
    const int N = input.size();
    vector<int> solution;
    int node = 0, distance = manhattan (input);

    //distance = dfs (start, node, 0, distance);
    a_star (input);
    /*
    for (distance = dfs (start, node, 0, distance); 1; node = 0, distance = dfs (start, node, 0, distance)) {
        cout << distance;
    }
    */
    return solution;
}

int main () {
  auto start = std::chrono::high_resolution_clock::now();

  vector<vector<int>> grid {{15,10,11,5},{14,1,3,2},{6,12,13,9},{8,7,4,0}};
  sliding(grid);
  //linearcflt (grid);
  grid = {{16,12,1,14,0,13},{5,27,29,17,21,23},{31,26,33,2,20,4},{15,8,30,32,35,24},{28,9,19,3,34,7},{6,10,18,22,11,25}};
  sliding(grid);


  grid = {{1,14,2,4,6,18},{9,13,3,17,11,33},{19,7,16,10,5,12},{8,26,20,15,22,24},{21,31,27,29,23,30},{25,0,32,28,34,35}};
  sliding(grid);

  // cntline (grid, sweep)
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}

/*
int findDistance(vector<vector<int>> board) {
  	int dist = 0;

  	for (int x = 0; x < 4; x++)
    		for (int y = 0; y < 4; y++) {
      			if (board[x][y])
      				  dist += abs (((board[x][y] - 1) / 4) - x) + abs(((board[x][y] - 1) % 4) - y);
      			else
      				  dist += abs (3 - x) + abs(3 - y);
    		}

  	return dist;
}
double ManHattan2 (const board &a, const board &b) {
		int N = a.size(), sum = 0;

		int pR[(N * N) + 1];
		int pC[(N * N) + 1];

		for (int r = 0; r < N; r++) {
  			for (int c = 0; c < N; c++) {
    				pR[a[r][c]] = r;
    				pC[a[r][c]] = c;
  			}
		}

		for (int r = 0; r < N; r++)
  			for (int c = 0; c < N; c++)
  				  if (b[r][c])
  					      sum += abs(pR[b[r][c]] - r) + abs(pC[b[r][c]] - c);
		return sum;
}
double HammingDistance2 (const board &a, const board &b) {
    int conflicts = 0;
    for (int i = 0; i < a.size() ; i++)
        for (int j = 0; j < a.size() ; j++)
            if (a[i][j] && a[i][j] != b[i][j]) conflicts++;
    return conflicts;
}
int dfs (vertex curr, int &node, int cost, int maxdepth) {
    node++;
    auto [dist, g,slid, grid] = curr;
    int estimate = cost + dist;

    if (estimate > maxdepth) return estimate;

    if (manhattan (grid) == 0) {
        Display::graph(grid);
        return -cost;
    }

    int minv = numeric_limits<int>::max(), depth;

    for (auto &dir : compass) {
        point nxt = slid + dir;

        if (is_inside (nxt, grid.size())) {

            swap (grid[slid.second][slid.first], grid[nxt.second][nxt.first]);
            int alt = manhattan (grid);

            depth = dfs ({alt, nxt, grid}, node, cost + 1, maxdepth);

            if (depth <= 0) return depth;
            else if (depth < minv) minv = depth;

            swap (grid[slid.second][slid.first], grid[nxt.second][nxt.first]);
        }
    }
    return minv;
}
*/
