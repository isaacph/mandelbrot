#include "grid.h"
#include "util.h"
#include <glm/glm.hpp>
#include <random>

BlockType GridManager::set(BlockType type, int worldx, int worldy) {
    int gridx = divRoundDown(worldx, GRID_SIZE);
    int gridy = divRoundDown(worldy, GRID_SIZE);
    int ingridx = modRoundDown(worldx, GRID_SIZE);
    int ingridy = modRoundDown(worldy, GRID_SIZE);
    GridPos gridpos = {gridx, gridy};
    auto ptr = grids.find(gridpos);
    if (ptr == grids.end()) {
        ptr = grids.insert({gridpos, Grid()}).first;
    }
    BlockType prevType = ptr->second.blocks[ingridy * GRID_SIZE + ingridx];
    ptr->second.blocks[ingridy * GRID_SIZE + ingridx] = type;
    if (true || prevType != type) gridChanges.emit({{gridx, gridy}, ptr->second});
    return prevType;
}

void GridManager::setGrid(Grid grid, int gridX, int gridY) {
    GridPos pos = {gridX, gridY};
    grids[pos] = grid;
    gridChanges.emit({pos, grid});
}

BlockType GridManager::check(int worldx, int worldy)const {
    int gridx = divRoundDown(worldx, GRID_SIZE);
    int gridy = divRoundDown(worldy, GRID_SIZE);
    int ingridx = modRoundDown(worldx, GRID_SIZE);
    int ingridy = modRoundDown(worldy, GRID_SIZE);
    GridPos gridpos = {gridx, gridy};
    auto ptr = grids.find(gridpos);
    if (ptr == grids.end()) {
        return air;
    }
    return ptr->second.blocks[ingridy * GRID_SIZE + ingridx];
}

std::pair<glm::vec2, glm::vec2> getSpriteSheetCoordinates(int sheetTilesX, int sheetTilesY, int index) {
    int x = modRoundDown(index, sheetTilesX);
    int y = modRoundDown(divRoundDown(index, sheetTilesX), sheetTilesY);
    float tileWidth = 1.0f / sheetTilesX, tileHeight = 1.0f / sheetTilesY;
    float topLeftX = x * tileWidth, topLeftY = y * tileWidth;
    return {{topLeftX, topLeftY}, {topLeftX + tileWidth, topLeftY + tileWidth}};
}

std::vector<float> makeTexturedBuffer(const Grid& grid) {
    std::vector<float> buffer;
    buffer.reserve(GRID_SIZE * GRID_SIZE * 6 * 4);
    int sheetTilesX = 4, sheetTilesY = 4;
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int index = grid.blocks[y * GRID_SIZE + x];
            auto c = getSpriteSheetCoordinates(sheetTilesX, sheetTilesY, index);
            buffer.insert(buffer.end(), {
                (float) (x + 0) / GRID_SIZE, (float) (y + 0) / GRID_SIZE, c.first.x, c.first.y,
                (float) (x + 0) / GRID_SIZE, (float) (y + 1) / GRID_SIZE, c.first.x, c.second.y,
                (float) (x + 1) / GRID_SIZE, (float) (y + 1) / GRID_SIZE, c.second.x, c.second.y,
                (float) (x + 1) / GRID_SIZE, (float) (y + 1) / GRID_SIZE, c.second.x, c.second.y,
                (float) (x + 1) / GRID_SIZE, (float) (y + 0) / GRID_SIZE, c.second.x, c.first.y,
                (float) (x + 0) / GRID_SIZE, (float) (y + 0) / GRID_SIZE, c.first.x, c.first.y,
            });
        }
    }
    return buffer;
}

Grid randomGrid() {
    Grid grid;
    std::default_random_engine random;
    std::uniform_int_distribution<BlockType> distribution(0, 1);

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            grid.blocks[y * GRID_SIZE + x] = distribution(random);
        }
    }
    return grid;
}

