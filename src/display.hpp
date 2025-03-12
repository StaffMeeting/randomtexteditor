#include <ncurses.h>
#include <vector>
#include <string>
#include <algorithm>
#include "global.h"
#include <iostream>

std::string get_filename() {
    echo();
    curs_set(1);

    char input[69];
    getstr(input);

    noecho();
    curs_set(0);

    return std::string(input);
}

void display() {
    clear();
    getmaxyx(stdscr, max_y, max_x);
    for (int i = 0; i < max_y - 1 && i < content.size(); ++i) {
        mvprintw(i, 0, "%i", i + 1);
        mvprintw(i, content.size() / 10, "|%s", content[i].c_str());
        if (i == cursor_row) {
            int draw_col = std::min(cursor_col, (int)content[i].length()) + content.size() / 10;
            move(i, draw_col);
            addch(content[i][cursor_col - 1] | A_REVERSE);
        }
    }
    refresh();

    attron(A_REVERSE);
    mvprintw(max_y - 1, 0, " Editing: %s| Row: %d Col: %d | F12: Save/Exit\n", filename.c_str(), cursor_row + 1, cursor_col);

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
                std::string cmdlist = get_filename();
                if (cmdlist == "col") {
                    strg = content[cursor_row];
                    display();
                } else if (cmdlist == "coc") {
                    strg = content[cursor_row][cursor_col - 1];
                    display();
                } else if (cmdlist == "ps") {
                    for (char c : strg) {
                        content[cursor_row].insert(cursor_col, 1, c);
                        cursor_col++;
                        display();
                    }
                } else if (cmdlist == "cul") {
                    strg = content[cursor_row];
                    content[cursor_row].clear();
                    cursor_row++;
                }
                break;
            }
        }
        cursor_row = std::max(0, std::min(cursor_row, (int)content.size() - 1));
        cursor_col = std::max(0, std::min(cursor_col, (int)content[cursor_row].length()));
        display();
    }
}

