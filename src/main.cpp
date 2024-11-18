#include "browser_manager.h"
#include <iostream>

int main() {
    std::string browser_name;
    std::cout << "Enter the browser name (e.g., firefox, brave): ";
    std::cin >> browser_name;

    runBrowserTaskManager(browser_name);
    return 0;
}

