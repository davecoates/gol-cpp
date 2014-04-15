#include "rle.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class InvalidRleException : public std::runtime_error {
    public:
        InvalidRleException(const string msg) : std::runtime_error(msg) {}
};

enum struct HeaderType { unknown, comment, name, origin, rule, coord };

struct Header
{
    int             line_number;
    HeaderType     type;
    string          value;
};

/**
 * Parse a comment string
 */
shared_ptr<Header> parse_header(const string &line, int number) 
{
    shared_ptr<Header> comment(new Header());
    comment->line_number = number;

    if (line.length() < 3) {
        comment->type = HeaderType::unknown;
        comment->value = "";
        return comment;
    }

    comment->value = line.substr(3);

    switch (line[1]) {
        case 'N':
            comment->type = HeaderType::name;
            break;
        case 'O':
            comment->type = HeaderType::origin;
            break;
        case 'C':
        case 'c':
            comment->type = HeaderType::comment;
            break;
        case 'P':
        case 'R':
            comment->type = HeaderType::coord;
            break;
        case 'r':
            comment->type = HeaderType::rule;
            break;
        default:
            comment->type = HeaderType::unknown;
            break;
    }

    return comment;
}


int read_pattern_file(string fname)
{
    string line;
    vector<string> lines, comments;

    char first;
    bool is_header = true, is_data = false;
    int line_number = 1;
    shared_ptr<Header> header;
    ifstream f (fname);
    if (f.is_open()) {
        while (getline(f, line)) {
            first = line[0];
            if (is_data) {
                cout << line << '\n';
            } else {
                switch (first) {
                    case '#':
                        if (!is_header) {
                            throw InvalidRleException("Header lines must precede rule and data");
                        }
                        header = parse_header(line, line_number);
                        if (header->type == HeaderType::name) {
                            cout << "Header: " << header->line_number << " " << header->value << '\n';
                        }
                        break;
                    case 'x':
                        std::regex rgx("x[ ]?=[ ]?([0-9]+), y[ ]?=[ ]?([0-9]+)(,[ ]*rule[ ]?=[ ]?(.*)/(.*))?");
                        std::smatch match;
                        if (std::regex_search(line, match, rgx)) {
                            cout << "X: " << match[1] << " Y: " << match[2] << " Rule: " << match[4] << " / " << match[5] << '\n';
                        } else {
                            cout << "NO MATCH";
                        }
                        cout << line.find("x = ") << '\n';
                        cout << "rule";
                        is_data = true;
                        is_header = false;
                        break;
                }
            }
            lines.push_back(line);
            line_number++;
        }
        cout << "All lines" << '\n';
        for (auto line : lines) {
            cout << line << '\n';
        }

        f.close();
    } else {
        cout << "Unable to open file";
    }
    return 5;
}
