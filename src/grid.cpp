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

BlockType GridManager::check(int worldx, int worldy)const {
    int gridx = divRoundDown(worldx, GRID_SIZE);
    int gridy = divRoundDown(worldy, GRID_SIZE);
    int ingridx = modRoundDown(worldx, GRID_SIZE);
    int ingridy = modRoundDown(worldy, GRID_SIZE);
    GridPos gridpos = {gridx, gridy};
    auto ptr = grids.find(GridPos{worldx, worldy});
    if (ptr == grids.end()) {
        return air;
    }
    return ptr->second.blocks[ingridy * GRID_SIZE + ingridx];
}

std::vector<float> makeTexturedBuffer(const Grid& grid) {
    std::vector<float> buffer;
    buffer.reserve(GRID_SIZE * GRID_SIZE * 6 * 4);
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            buffer.insert(buffer.end(), {
                x + 0.0f, y + 0.0f, 0.0f, 0.0f,
                x + 0.0f, y + 1.0f, 0.0f, 1.0f,
                x + 1.0f, y + 1.0f, 1.0f, 1.0f,
                x + 1.0f, y + 1.0f, 1.0f, 1.0f,
                x + 1.0f, y + 0.0f, 1.0f, 0.0f,
                x + 0.0f, y + 0.0f, 0.0f, 0.0f
            });
        }
    }
    return buffer;
}

