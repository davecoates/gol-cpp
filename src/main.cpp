#include "gol.hpp"
#include "rle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


CellMap renderables;

int main()
{
    std::shared_ptr<Pattern> pattern = read_pattern_file("../patterns/glider.rle");
    sf::Clock clock;

    /*std::vector<Point> neighbours = getNeighbours(0, 0);*/

    //for (auto p : neighbours) {
        //std::cout << p.x << ", " << p.y << " | ";
    //}
    /*std::cout << '\n';*/


    std::cout << *pattern << '\n';
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game of Life");
    window.setFramerateLimit(60);

    for (auto position : pattern->getSeedCells()) {
        std::shared_ptr<Cell> cell(new Cell(position->x+20, position->y+15));
        Point pos(position->x, position->y);
        renderables[pos] = cell;
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    tick(renderables);
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Return) {
                    renderables.clear();
                    for (auto position : pattern->getSeedCells()) {
                        std::shared_ptr<Cell> cell(new Cell(position->x+20, position->y+15));
                        Point pos(position->x, position->y);
                        renderables[pos] = cell;
                    }
                }
            }
        }

        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asMilliseconds() >= 100) {
            clock.restart();
            tick(renderables);
        }

        window.clear();
        for (auto renderable : renderables) {
            window.draw(renderable.second->getRenderable());
        }
        window.display();
    }
    return 0;
}
