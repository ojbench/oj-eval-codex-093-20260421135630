#include <bits/stdc++.h>
using namespace std;

struct State {
    int y, x, dir;
    array<uint64_t, 2> pel;
    bool operator==(State const& other) const {
        return y == other.y && x == other.x && dir == other.dir && pel == other.pel;
    }
};

struct StateHash {
    size_t operator()(State const& s) const noexcept {
        // FNV-1a like mixing
        uint64_t h = 1469598103934665603ull;
        auto mix = [&](uint64_t v){ h ^= v; h *= 1099511628211ull; };
        mix((uint64_t)((uint64_t)(uint16_t)s.y << 48) ^ ((uint64_t)(uint16_t)s.x << 32) ^ (uint64_t)s.dir);
        mix(s.pel[0]);
        mix(s.pel[1]);
        return (size_t)h;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int H = 10, W = 10;
    vector<vector<int>> grid(H, vector<int>(W));
    int sy = -1, sx = -1;
    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            if(!(cin >> grid[i][j])) return 0;
            if(grid[i][j] == 4){ sy = i; sx = j; grid[i][j] = 1; }
        }
    }
    if(sy == -1) return 0;

    // Directions: 0 up, 1 right, 2 down, 3 left
    const int dy[4] = {-1, 0, 1, 0};
    const int dx[4] = { 0, 1, 0,-1};

    // Pellet bitset stored in two uint64s (0..63, 64..99)
    array<uint64_t,2> pel = {0ull, 0ull};
    auto set_bit = [&](int y, int x){ int k = y*W + x; if(k<64) pel[0] |= (1ull<<k); else pel[1] |= (1ull<<(k-64)); };
    auto test_bit = [&](int y, int x){ int k=y*W+x; if(k<64) return (pel[0]>>k)&1ull; else return (pel[1]>>(k-64))&1ull; };
    auto clear_bit = [&](int y, int x){ int k=y*W+x; if(k<64) pel[0] &= ~(1ull<<k); else pel[1] &= ~(1ull<<(k-64)); };

    for(int i=0;i<H;i++) for(int j=0;j<W;j++) if(grid[i][j]==2) set_bit(i,j);

    int y = sy, x = sx;
    int dir = 0; // start facing up
    long long score = 0;

    unordered_set<State, StateHash> vis;
    vis.reserve(100000);

    auto inb = [&](int yy, int xx){ return yy>=0 && yy<H && xx>=0 && xx<W; };

    while(true){
        State s{y,x,dir,pel};
        if(vis.find(s) != vis.end()){
            cout << "Silly Pacman\n";
            return 0;
        }
        vis.insert(s);

        // Choose next dir by right-hand rule: right > straight > left > uturn
        int prio[4] = {(dir+1)&3, dir, (dir+3)&3, (dir+2)&3};
        int nd = -1; int ny = y, nx = x;
        for(int k=0;k<4;k++){
            int cand = prio[k];
            int ty = y + dy[cand];
            int tx = x + dx[cand];
            if(!inb(ty,tx)) continue;
            if(grid[ty][tx] == 0) continue; // wall
            nd = cand; ny = ty; nx = tx;
            break;
        }
        if(nd == -1){
            cout << "Silly Pacman\n";
            return 0;
        }

        if(grid[ny][nx] == 3){
            score -= 500;
            cout << score << "\n";
            return 0;
        }

        y = ny; x = nx; dir = nd;
        if(test_bit(y,x)){
            score += 2;
            clear_bit(y,x);
        }
    }
}
