#ifndef SRC_GRID_H_INCLUDED
#define SRC_GRID_H_INCLUDED
#include <map>

const int GRID_SIZE = 32;
using BlockType = char;

class Grid {
public:
    BlockType blocks[GRID_SIZE * GRID_SIZE];
};

class GridPos {
public:
    int x, y;
    inline bool operator<(const GridPos& other) const {
        return x == other.x ? y < other.y : x < other.x;
    }
};

class GridManager {
public:
    BlockType& at(int worldx, int worldy);
    BlockType check(int worldx, int worldy);
    std::map<GridPos, Grid> grids;
};


#endif
