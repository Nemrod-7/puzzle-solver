#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

struct point { int x,y ; };

int N = 0;

void print (string src) {
    fstream ios ("notes", std::ios::app);
    ios << src;
    ios.close();
}
string display (vector<int> grid) {
    stringstream oss;
    for (int i = 0; i < grid.size(); i++) {
        oss << setw(3) << grid[i];
        if (i % N == N - 1) oss << endl;
    }

    oss << endl;
    return oss.str();
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
    int now, nxt;

    while (p.x != col) {
        now = p.y * N + p.x;
        if (col < p.x) {
            p.x--;
        } else if (col > p.x) {
            p.x++;
        }
        nxt = p.y * N + p.x;
        swap(grid[nxt], grid[now]);
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

void moveup (vector<int> &grid, point p) {

}
int main () {

    N = 6;
    vector<int> grid;// = generate();
    grid = {16,12,1,14,0,13,5,27,29,17,21,23,31,26,33,2,20,4,15,8,30,32,35,24,28,9,19,3,34,7,6,10,18,22,11,25};

    point now = locate(grid, 0);
    point num = locate(grid, 2);

    int index = num.y * N + num.x;

    if (grid[index] != index + 1) {
        if (num.x < now.x && num.y > now.y) {
          lateral (grid, now, num.x);
          vertical (grid, now, num.y);
        }
    }


    num = locate(grid, 3);
    index = num.y * N + num.x;

    if (grid[index] != index + 1) {
        if (num.x < now.x) {
            lateral (grid, now, num.x);
        }
        if (num.y > now.y) {
            vertical (grid, now, num.y);
        }
    }

    num = locate(grid, 3);
    if (grid[index] != index + 1) {

        if (num.y > now.y) {
            // vertical (grid, now, num.y);
        } else { // move slider up the number
            if (num.x > 0) { // move by the left
                lateral (grid, now, num.x-1);
            } else {
                lateral (grid, now, num.x+1);
            }

            if (now.y > 0) {
                vertical (grid, now, num.y - 1);
            }
            lateral (grid, now, num.x);
        }
    }

    cout << display(grid);
}
