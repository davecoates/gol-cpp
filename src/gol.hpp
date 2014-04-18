#ifndef __GOL_H_
#define __GOL_H_

#include <SFML/Graphics.hpp>
#include <iostream>

class Cell {
    private:
        const float size_x = 10.0f, size_y = 10.0f;
        int pos_x, pos_y;

        sf::RectangleShape shape;

    public:
        Cell(int x, int y) {
            pos_x = x;
            pos_y = y;
            sf::Vector2f size(size_x, size_y);
            shape.setFillColor(sf::Color::Green);
            shape.setSize(size);
            shape.setPosition(x*size_x, y*size_y);
        }

        int getX() { return pos_x; }
        int getY() { return pos_y; }

        void die() { shape.setFillColor(sf::Color::Red); }

        const sf::RectangleShape& getRenderable() { return shape; }
};

struct Point
{
      int x, y;

      Point(int x, int y) { this->x = x; this->y = y; }

      bool operator<( const Point& other) const
      {
          if ( x == other.x )
          {
              return y < other.y;
          }

          return x < other.x;
      }
};

/**
 * Get neighbours of a point
 */
std::vector<Point> getNeighbours(int x, int y) ;

typedef std::map<Point, std::shared_ptr<Cell>> CellMap;

/**
 * Tick over to next generation
 */
void tick(CellMap &renderables);

#endif // __GOL_H_
