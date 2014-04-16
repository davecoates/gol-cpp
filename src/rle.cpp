#include "rle.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;


/**
 * Parse a comment string
 */
shared_ptr<Header> parse_header(const string &line, int number) 
{
    shared_ptr<Header> header(new Header());
    header->line_number = number;

    if (line.length() < 3) {
        header->type = HeaderType::unknown;
        header->value = "";
        return header;
    }

    header->value = line.substr(3);

    switch (line[1]) {
        case 'N':
            header->type = HeaderType::name;
            break;
        case 'O':
            header->type = HeaderType::origin;
            break;
        case 'C':
        case 'c':
            header->type = HeaderType::comment;
            break;
        case 'P':
        case 'R':
            header->type = HeaderType::coord;
            break;
        case 'r':
            header->type = HeaderType::rule;
            break;
        default:
            header->type = HeaderType::unknown;
            break;
    }

    return header;
}

std::ostream& operator<<(std::ostream& s, const Rule &rule) {
    s << "Width: " << rule.width << " Height: " << rule.height;

    cout << " Survival counts: ";
    for (auto count : rule.survival_counts) {
        cout << count << " ";
    }

    cout << " Birth counts: ";
    for (auto count : rule.birth_counts) {
        cout << count << " ";
    }

    return s;
}


Rule::Rule(const uint x, const uint y, const string &s, const string &b) {
    width = x;
    height = y;

    string survival = s, birth = b;

    // Check to see if parameters were reversed
    if (survival[0] == 'B') {
        survival.swap(birth);
    }

    // Check if rates include optional S / B prefix
    if (survival[0] == 'S') {
        survival.erase(0, 1);
    }

    if (birth[0] == 'B') {
        birth.erase(0, 1);
    }

    if (s.length() == 0)  {
        survival_counts.push_back(0);
    } else {
        for (auto c : survival) {
            survival_counts.push_back(c - '0');
        }
    }
    if (birth.length() == 0) {
        throw InvalidRleException("Birth rate must be specified");
    }
    int rate;
    for (auto c : birth) {
        rate = c - '0';
        if (rate == 0) {
            throw InvalidRleException("Birth rate of zero not supported");
        }
        birth_counts.push_back(rate);
    }

}

std::ostream& operator<<(std::ostream& s, const Pattern &pattern) {
    s << *pattern.rule << '\n';
    for (auto header : pattern.headers) {
        cout << header->type << ": " << header->value << '\n';
    }
    return s;
}

/**
 * Parse a rule from a string of form x = #, y = #, rule = ##/##
 *
 * rule = #/# is optional
 */
shared_ptr<Rule> parse_rule(const string &line)
{
    std::regex rgx("x[ ]?=[ ]?([0-9]+), y[ ]?=[ ]?([0-9]+)(,[ ]*rule[ ]?=[ ]?(.*)/(.*))?");
    std::smatch match;

    if (!std::regex_search(line, match, rgx)) {
        throw InvalidRleException("Invalid rule specification");
    }
    uint x, y;

    x = atoi(match[1].str().c_str());
    y = atoi(match[2].str().c_str());

    string s, b;
    if (match[3].length()) {
        s = match[4];
        b = match[5];
    } else {
        // Default survival & birth rates to use
        s = "23";
        b = "3";
    }

    shared_ptr<Rule> rule(new Rule(x, y, s, b));

    return rule;
}

shared_ptr<Pattern> read_pattern_file(const std::string fname)
{
    string line;
    vector<string> lines;
    vector<shared_ptr<Header>> headers;

    char first;
    bool is_header = true, is_data = false;
    int line_number = 1;
    shared_ptr<Rule>  rule;
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
                        headers.push_back(header);
                        break;
                    case 'x':
                        is_data = true;
                        is_header = false;
                        rule = parse_rule(line);
                        break;
                }
            }
            lines.push_back(line);
            line_number++;
        }

        f.close();
    } else {
        throw InvalidRleException("Unable to open file");
    }
    return shared_ptr<Pattern>(new Pattern(rule, headers));
}
