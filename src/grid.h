#ifndef SRC_GRID_H_INCLUDED
#define SRC_GRID_H_INCLUDED

const int GRID_SIZE = 32;
using BlockType = char;

class Grid {
public:
    BlockType blocks[GRID_SIZE * GRID_SIZE];
};

#endif
