#include "gol.hpp"
#include "rle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace GOL;

CellMap renderables;

int main()
{
    std::shared_ptr<RLE::Pattern> pattern = RLE::read_pattern_file("../patterns/10enginecordership.rle");
    sf::Clock clock;

    std::cout << *pattern << '\n';

    uint window_w = 800, window_h = 600;
    float width = std::max(pattern->getPatternWidth(), pattern->getPatternHeight()) * 2.f;
    float height = width * (window_h * 1.f / window_w * 1.f);

    sf::RenderWindow window(sf::VideoMode(window_w, window_h), "Game of Life");
    sf::View view;
    sf::FloatRect viewRect(-width/2.f + pattern->getPatternWidth()/2.f, 
                           -height/2.f + pattern->getPatternHeight()/2.f, 
                           width, height);
    view.reset(viewRect);
    window.setFramerateLimit(60);
    window.setView(view);

    // Create initial cells based on pattern
    for (auto position : pattern->getSeedCells()) {
        std::shared_ptr<Cell> cell(new Cell(position->x, position->y));
        Point pos(position->x, position->y);
        renderables[pos] = cell;
    }

    // Zoom velocity
    float zoom_v = 0.1f;
    // Movement velocity
    float move_v = 5.0f;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                std::cout << event.key.code << "\n";
                switch (event.key.code) {
                    case  sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::PageDown:
                        view.zoom(1.f - zoom_v);
                        window.setView(view);
                        break;
                    case sf::Keyboard::PageUp:
                        view.zoom(1.f + zoom_v);
                        window.setView(view);
                        break;
                    case sf::Keyboard::Left:
                        view.move(-move_v, 0.f);                        
                        window.setView(view);
                        break;
                    case sf::Keyboard::Right:
                        view.move(move_v, 0.f);                        
                        window.setView(view);
                        break;
                    case sf::Keyboard::Up:
                        view.move(0.f, -move_v);                        
                        window.setView(view);
                        break;
                    case sf::Keyboard::Down:
                        view.move(0.f, move_v);                        
                        window.setView(view);
                        break;
                    default:
                        break;
                }
            }
        }

        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asMilliseconds() >= 10) {
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
