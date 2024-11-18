#include "browser_manager.h"
#include <ncurses.h>
#include <iostream>

// Display processes using ncurses
void displayProcesses(WINDOW* win, const std::vector<Process>& processes, int highlight) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "PID     Name                CPU(%%)   Memory(MB)");
    mvwprintw(win, 2, 2, "-----------------------------------------------");

    int row = 3;
    for (size_t i = 0; i < processes.size(); ++i) {
        if (static_cast<int>(i) == highlight) // Fix: Cast i to int for comparison
            wattron(win, A_REVERSE);
        mvwprintw(win, row++, 2, "%-8d %-20s %-8.2f %-8ld",
                  processes[i].pid, processes[i].name.c_str(), processes[i].cpu_usage, processes[i].memory);
        if (static_cast<int>(i) == highlight) // Fix: Cast i to int for comparison
            wattroff(win, A_REVERSE);
    }

    wrefresh(win);
}

// Task manager for browser processes
void runBrowserTaskManager(const std::string& browser_name) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int highlight = 0;
    int choice;
    WINDOW* win = newwin(20, 60, 1, 1);
    keypad(win, TRUE);

    while (true) {
        std::vector<Process> processes = getProcesses(browser_name);
        if (processes.empty()) {
            endwin();
            std::cout << "No processes found for browser: " << browser_name << std::endl;
            return;
        }

        displayProcesses(win, processes, highlight);

        choice = wgetch(win);
        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? static_cast<int>(processes.size() - 1) : highlight - 1; // Fix: Cast size to int
                break;
            case KEY_DOWN:
                highlight = (highlight == static_cast<int>(processes.size() - 1)) ? 0 : highlight + 1; // Fix: Cast size to int
                break;
            case 10: // Enter key
                terminateProcess(processes[highlight].pid);
                break;
            case 'q': // Quit
                endwin();
                return;
        }
    }
}

