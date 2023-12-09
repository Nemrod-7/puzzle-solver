#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

struct point { int x,y ; };

int N = 0;

void display (vector<int> grid) {
    for (int i = 0; i < grid.size(); i++) {
        cout << setw(3) << grid[i];
        if (i % N == N - 1) cout << endl;
    }

    cout << endl;
}
vector<int> generate () {
    vector<int> grid;

    for (int i = 1; i < N * N; i++) {
        grid.push_back(i);
    }
    grid.push_back(0);
    return grid;
}
point locate (const vector<int> &grid, int num) { // locate tile

    for (int i = 0; i < grid.size(); i++) {
        if (grid[i] == num) {
            return {i % N, i / N};
        }
    }

    return {0,0};
}

void lateral (vector<int> &grid, point &now, int col) {
    point nxt;

    while (now.x != col) {
        nxt = now;
        if (col < now.x) {
            now.x--;
        } else if (col > now.x) {
            now.x++;
        }
        swap(grid[nxt.y * N + nxt.x], grid[now.y * N + now.x]);
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

int main () {

    N = 6;
    vector<int> grid;// = generate();
    grid = {16,12,1,14,0,13,5,27,29,17,21,23,31,26,33,2,20,4,15,8,30,32,35,24,28,9,19,3,34,7,6,10,18,22,11,25};

    point now = locate(grid, 0);
    point nxt = locate(grid, 2); 


    //lateral (grid, now, 1);
    //vertical (grid, now, 1);

    display(grid);
}
