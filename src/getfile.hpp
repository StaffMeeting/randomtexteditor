#include <ncurses.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

std::vector<std::string> listfile(const std::string& path) {
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            files.push_back(ent->d_name);
        }
        closedir(dir);
    } else {
        perror("opendir");
    }

    return files;
}

std::string choosefile(const std::string& start_path) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int row, col;
    getmaxyx(stdscr, row, col);

    std::string current_path = start_path;
    std::vector<std::string> files = listfile(current_path);
    int highlight = 0;

    while (true) {
        clear();
        mvprintw(0, 0, "Select a file (Use arrow keys and Enter):");

        for (int i = 0; i < files.size(); ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 0, files[i].c_str());
            attroff(A_REVERSE);
        }

        int choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? files.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == files.size() - 1) ? 0 : highlight + 1;
                break;
            case KEY_BACKSPACE:
                if (files[highlight] == "..") {
                    size_t pos = current_path.find_last_of('/');
                    current_path = current_path.substr(0, pos);
                } else if (files[highlight] == ".") {
                    continue;
                } else if (current_path != "/") {
                    current_path += "/" + files[highlight];
                } else {
                    current_path += files[highlight];
                }
                if (listfile(current_path).size() > 0) {
                    files = listfile(current_path);
                    highlight = 0;
                } else {
                    endwin();
                    return current_path;
                }
                break;
            case 27:
                endwin();
                return "";
        }
    }
}
