#include <iostream>
#include <queue>

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
        return (a.move == b.move) ?  a.dist > b.dist : a.move > b.move;
    }
};

point operator + (const point& a, const point& b) {
    return {a.x + b.x, a.y + b.y};
}
bool operator < (const point &a, const point &b) {
    return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

int getInvCount (int arr[]) {
    int inv_count = 0;

    for (int i = 0; i < 9 - 1; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (arr[j] && arr[i] &&  arr[i] > arr[j]) {
                inv_count++;
            }
        }
    }

    return inv_count;
}
 

int main () {

    priority_queue<vertex, vector<vertex>, comp> q1;

    vertex start {0, 0, {0,0}, ""};
    q1.push(start);
    
    int N = 0;
    vector<vector<int>> grid;

}
