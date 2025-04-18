#include <ncurses.h>
#include <vector>
#include <string>
#include <algorithm>
#include "global.h"
#include <iostream>

void display() {
    clear();
    getmaxyx(stdscr, max_y, max_x);
    if (cursor_row < scroll_offset) {
        scroll_offset = cursor_row;
    } else if (cursor_row >= scroll_offset + max_y - 1) {
        scroll_offset = cursor_row - max_y + 2;
    }

    for (int i = 0; i < max_y - 1 && i + scroll_offset < content.size(); ++i) {
        mvprintw(i, 0, "%i", i + 1 + scroll_offset);
        mvprintw(i, content.size() / 10, "|%s", content[i + scroll_offset].c_str());
        if (i + scroll_offset == cursor_row) {
            int draw_col = std::min(cursor_col, (int)content[i + scroll_offset].length()) + content.size() / 10;
            move(i, draw_col);
            addch(content[i + scroll_offset][cursor_col - 1] | A_REVERSE);
        }
    }
    refresh();

    attron(A_REVERSE);
    mvprintw(max_y - 1, 0, " file: %s| line:%d col:%d | F12:exit ", filename.c_str(), cursor_row, cursor_col);

    clrtoeol();
    attroff(A_REVERSE);

    refresh();
}

void cmdmode() {
    while ((ch = getch()) != KEY_F(6)) {
        switch (tolower(ch)) {
            case KEY_UP:
            case 'w': {
                if (cursor_row > 0) {
                    cursor_row--;
                }
                break;
            }
            case KEY_LEFT:
            case 'a': {
                if (cursor_col > 0) {
                    cursor_col--;
                }
                break;
            }
            case KEY_DOWN:
            case 's': {
                if (cursor_row < content.size() - 1) {
                    cursor_row++;
                }
                break;
            }
            case KEY_RIGHT:
            case 'd': {
                if (cursor_col < content[cursor_row].length()) {
                    cursor_col++;
                }
                break;
            }
            case 'c': {
                echo();
                curs_set(1);
                std::string input;
                getstr(input.c_str());
                noecho();
                curs_set(0);
                system(input.c_str());
                break;
            }
        }
        cursor_row = std::max(0, std::min(cursor_row, (int)content.size() - 1));
        cursor_col = std::max(0, std::min(cursor_col, (int)content[cursor_row].length()));
        display();
        }
    }
}
