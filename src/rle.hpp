#ifndef __RLE_H_
#define __RLE_H_

#include <string>
#include <vector>

namespace RLE {

    class InvalidRleException : public std::runtime_error {
        public:
            InvalidRleException(const std::string msg) : std::runtime_error(msg) {}
    };


    /**
     * Class to track rule settings.
     *
     * width is width of pattern
     * heigh is height of pattern
     *
     * survival_counts is the number of neighbours live cell requires to stay alive
     * birth_counts is the number neighbours dead cell requires to come alive
     */
    class Rule
    {
        private:
            unsigned int width, height;
            friend std::ostream& operator<<(std::ostream&, const Rule&);

            std::vector<unsigned int> survival_counts,
                                      birth_counts;

        public:
            Rule(const unsigned int x, const unsigned int y, 
                 const std::string &s, const std::string &b);

            unsigned int getWidth() { return width; }
            unsigned int getHeight() { return height; }
            
    };

    enum struct HeaderType { unknown, comment, name, origin, rule, coord };

    struct Header
    {
        int             line_number;
        HeaderType      type;
        std::string     value;
    };



    struct Position {
        int x, y;
    };


    class Pattern
    {
        private:
            std::shared_ptr<Rule> rule;
            std::vector<std::shared_ptr<Header> > headers;
            std::vector<std::shared_ptr<Position> > seed_cells;
            
            friend std::ostream& operator<<(std::ostream&, const Pattern&);
            
        public:
            Pattern(std::shared_ptr<Rule> r,
                    std::vector<std::shared_ptr<Header> > h,
                    std::vector<std::shared_ptr<Position> > cells
                    ) {
                rule = r;
                headers = h;
                seed_cells = cells;
            }

            std::vector<std::shared_ptr<Position> > getSeedCells() { return seed_cells; }

            unsigned int getPatternWidth() {
                return rule->getWidth();
            }

            unsigned int getPatternHeight() {
                return rule->getHeight();
            }
                    
    };


    std::shared_ptr<Pattern> read_pattern_file(const std::string fname);

}
#endif // __RLE_H_
