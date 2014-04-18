#include "gol.hpp"

/**
 * Get neighbours of a point
 */
std::vector<Point> getNeighbours(int x, int y) 
{
    int offsets[3] = {-1, 0, 1};
    int dx, dy;
    std::vector<Point> neighbours;
    for (uint i=0;i < 3;i++) {
        dx = offsets[i];
        for (uint j=0;j < 3;j++) {
            dy = offsets[j];
            if (dx == 0 && dy == 0) continue;
            neighbours.push_back(Point(x + dx, y + dy));
        }
    }

    return neighbours;
}

/**
 * Tick over to next generation
 */
void tick(CellMap &renderables) {
    std::map<Point, uint> cell_frequencies;
    for (auto it : renderables) {
        std::vector<Point> neighbours = getNeighbours(it.first.x, it.first.y);
        for (auto neighbour : neighbours) {
            cell_frequencies[neighbour]++;
        }
    }
    //renderables.clear();
    bool exists = false;
    for (auto it : cell_frequencies) {
        exists = renderables.count(it.first) > 0;
        if (exists) {
            if (it.second < 2 || it.second > 3) {
                renderables[it.first]->die();
                renderables.erase(it.first);
            }
        } else if (it.second == 3) {
            std::shared_ptr<Cell> cell(new Cell(it.first.x+20, it.first.y+15));
            renderables[it.first] = cell;
        }
    }

    std::vector<Point> to_erase;
    for (auto it : renderables) {
        if (cell_frequencies.count(it.first) == 0) {
            to_erase.push_back(it.first);
        }
    }
    for (auto point  : to_erase) {
        renderables.erase(point);
    }
}
