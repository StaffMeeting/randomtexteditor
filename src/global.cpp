#include <vector>
#include <string>
#include "global.h"

int cursor_row = 0;
int cursor_col = 0;
std::string filename;
std::vector<std::string> content;
std::string cmdlist;
int ch;
int max_y;
int max_x;
std::string strg;
int scroll_offset = 0;