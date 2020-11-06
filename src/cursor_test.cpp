#include <iostream>
#include <ncurses.h>

int main()
{
    initscr();
    cbreak();
    noecho();

    std::string line;
    int ch;

    while (1)
    {
        ch = getch();
        if (ch == KEY_ENTER)
        {
            std::cout << (char)ch;
            break;
        }
        if (ch == KEY_LEFT)
        {
            std::cout << "\033[1D";
        }
        if (ch == KEY_RIGHT)
        {
            std::cout << "\033[1C";
        }
    }
    std::cout << line << std::endl;
}
