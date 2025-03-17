#include <ncurses.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "global.h"

std::vector<std::string> listfile(const std::string& path) {
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string name = ent->d_name;

            std::string full_path = path + "/" + name;

            if (access(full_path.c_str(), F_OK) == 0) {
                files.push_back(name);
            }
        }
        closedir(dir);

        std::sort(files.begin(), files.end());
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

        if (files.empty()) {
            mvprintw(1, 0, "Directory is empty. Press ESC to exit.");
        }

        int choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? files.size() - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == files.size() - 1) ? 0 : highlight + 1;
                break;
            case 10: //enter
                if (files.empty()) {
                    break;
                }
                if (files[highlight] == "..") {
                    size_t pos = current_path.find_last_of('/');
                    if (pos != std::string::npos) {
                        current_path = current_path.substr(0, pos);
                    } else {
                        current_path = "/";
                    }
                } else if (files[highlight] != ".") {
                    std::string selected_path = current_path + "/" + files[highlight];
                    std::vector<std::string> sub_files = listfile(selected_path);
                    if (!sub_files.empty()) {
                        current_path = selected_path;
                        files = sub_files;
                        highlight = 0;
                        continue;
                    } else if (std::ifstream(selected_path)) {
                        endwin();
                        return selected_path;
                    } else {
                        mvprintw(row - 1, 0, "Invalid selection. Press any key to continue.");
                        getch();
                    }
                }
                files = listfile(current_path);
                highlight = 0;
                break;
            case 27: //esc
                endwin();
                return "";
        }
    }
}
