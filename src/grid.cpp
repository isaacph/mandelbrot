#include "grid.h"
#include "util.h"

BlockType& GridManager::at(int worldx, int worldy) {
    int gridx = divRoundDown(worldx, GRID_SIZE);
    int gridy = divRoundDown(worldy, GRID_SIZE);
    int ingridx = modRoundDown(worldx, GRID_SIZE);
    int ingridy = modRoundDown(worldy, GRID_SIZE);
    GridPos gridpos = {gridx, gridy};
    auto ptr = grids.find(GridPos{worldx, worldy});
    if (ptr == grids.end()) {
        ptr = grids.insert({gridpos, Grid()}).first;
    }
    return ptr->second.blocks[ingridy * GRID_SIZE + ingridx];
}
