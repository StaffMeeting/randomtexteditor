#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "display.hpp"
#include "global.h"
#include "getfile.hpp"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    filename = choosefile("/");;
    clear();
    std::fstream file(filename);
    if (file.good()) {
        std::string line;
        while (std::getline(file, line)) {
            content.push_back(line);
        }
    }
    if (content.empty()) {
        content.push_back(" ");
    }
    display();
    int ch;
    while ((ch = getch()) != KEY_F(12)) {
        switch(ch) {
        case KEY_UP: {
            if (cursor_row > 0) {
                cursor_row--;
            }
            break;
        }
        case KEY_DOWN: {
            if (cursor_row < content.size() - 1) {
                cursor_row++;
            }
            break;
        }
        case KEY_LEFT: {
            if (cursor_col > 0) {
                cursor_col--;
            }
            break;
        }
        case KEY_RIGHT: {
            if (cursor_col < content[cursor_row].length()) {
                cursor_col++;
            }
            break;
        }
        case KEY_ENTER:
        case '\n':
        case '\r': {
            std::string currline = content[cursor_row];
            std::string newline = currline.substr(cursor_col);
            content[cursor_row] = currline.substr(0, cursor_col);
            content.insert(content.begin() + cursor_row + 1, newline);
            cursor_row++;
            cursor_col = 0;
            break;
        }
        case KEY_BACKSPACE:
        case 127: {
            if (cursor_col > 0) {
                content[cursor_row].erase(cursor_col-1, 1);
                cursor_col--;
            } else if (cursor_row > 0) {
                cursor_col = content[cursor_row - 1].length();
                content[cursor_row - 1] += content[cursor_row];
                content.erase(content.begin() + cursor_row);
                cursor_row--;
            }
            break;
        }
        case KEY_RESIZE: {
            break;
        }
        case KEY_F(6): {
            cmdmode();
            break;
        }
        default:
            if (isprint(ch)) {
                content[cursor_row].insert(cursor_col, 1, ch);
                cursor_col++;
            }
            break;
        }
        cursor_row = std::max(0, std::min(cursor_row, (int)content.size() - 1));
        cursor_col = std::max(0, std::min(cursor_col, (int)content[cursor_row].length()));
        display();
        std::ofstream out(filename);
        for (const auto& line : content) {
            out << line << '\n';
        }
    }
    endwin();
    return 0;
}
