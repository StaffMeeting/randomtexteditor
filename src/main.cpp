#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "display.hpp"
#include "global.h"
using namespace std;

int main(int argc, char* argv[]) {
	content;
	cursor_row;
	cursor_col;
	filename;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	mvprintw(0, 0, "Enter filename:");
	filename = get_filename();
	clear();
	fstream file(filename);
	if (file.good()) {
		string line;
		while (getline(file, line)) content.push_back(line);
	}
	if (content.empty()) content.push_back("");
	display();
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
			string currline = content[cursor_row];
			string newline = currline.substr(cursor_col);
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
		case '/':
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
		cursor_row = max(0, min(cursor_row, (int)content.size() - 1));
		cursor_col = max(0, min(cursor_col, (int)content[cursor_row].length()));
		display();
		ofstream out(filename);
		for (auto& line : content) out << line << '\n';
	}
	endwin();
	return EXIT_SUCCESS;
}
