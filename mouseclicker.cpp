#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <windows.h>

#include "mouseclicker.h"

static HWND hwndWindow;
static RECT rcWindow;
static LPARAM position;
static HANDLE _stdin;
static DWORD from_stdin;
static bool paused = false;
static bool quit = false;
static long sleep_interval = DEFAULT_SLEEP_INTERVAL;

int main(int argc, char **argv) {
    // Handle getting sleep interval from args, if applicable.
    if (argc == 2) {
        /* If an argument was supplied, it should be the number of
           milliseconds to sleep. */
        sleep_interval = strtol(argv[1], NULL, 10);

        if (sleep_interval <= 1) {
            // Fail if below 0 or equal to 1 ms.
            std::cerr << "Sleep too low: " << sleep_interval << std::endl;
            return 1;
        } else {
            // Otherwise notify the user of success.
            std::cout << "Sleep supplied, will sleep " << sleep_interval
                      << " ms between clicks" << std::endl;
        }
    } else {
        // Otherwise notify the user of the default interval.
        std::cout << "No parameter supplied, will use default sleep "
                  << "interval of " << sleep_interval
                  << " ms between clicks." << std::endl;
    }

    // Introduction
    std::cout << "To pause, press ENTER/SPACE. To quit, press ESCAPE"
              << std::endl;

    // Get stdin for peeking later.
    _stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (_stdin == NULL) {
        std::cerr << "Uanble to get stdin" << std::endl;
        return 1;
    }

    // Fetch the window handle.
    hwndWindow = FindWindow(NULL, "Clicker Heroes");
    if (hwndWindow == NULL) {
        std::cerr << "Unable to get window handle! Is the application running ?"
                  << std::endl;
        return 1;
    }

    while (!quit) {
        // While we're not paused and do_click returns nothing, continue.
        if (!paused) {
            if (do_click() != 0) {
                return 1;
            }
        }

        // Sleep.
        from_stdin = WaitForSingleObject(_stdin, sleep_interval);
        // If we encounter any input from the wait, handle it.
        if (from_stdin == WAIT_OBJECT_0) {
            handle_input();
        }
    }

    // Notify the user that we're all done.
    std::cout << "All done!" << std::endl;
    return 0;
}

// Does the clicking in the Clicker Heroes window.
int do_click() {
    // Get the dimensions
    if (!GetWindowRect(hwndWindow, &rcWindow)) {
        std::cerr << "Unable to GetWindowRect, did you close the application ?"
                  << std::endl;
        return 1;
    }

    // Calculate where to click.
    position = MAKELPARAM(
        (LONG) ((double) (rcWindow.right - rcWindow.left) * 0.75),
        (LONG) ((double) (rcWindow.bottom - rcWindow.top) * 0.5)
    );
    if (position == 0) {
        std::cerr << "Position is NULL!" << std::cerr;
        return 1;
    }

    // Send messages to the window handle, using the position calculated.
    if (!PostMessage(hwndWindow, WM_LBUTTONDOWN, NULL, position)) {
        std::cerr << "Failed to PostMessage()" << std::endl;
        return 0;
    }
    PostMessage(hwndWindow, WM_LBUTTONUP, NULL, position);
    return 0;
}

// Handles input form stdin.
void handle_input() {
    INPUT_RECORD input;
    DWORD number_of_inputs = 0;

    // Fetch the input from stdin.
    ReadConsoleInput(_stdin, &input, 1, &number_of_inputs);

    // Check for specific keypresses.
    if (input.Event.KeyEvent.bKeyDown) {
        switch(input.Event.KeyEvent.wVirtualKeyCode) {
        case 13:  // VK_ENTER
        case 32:  // VK_SPACE
            paused = !paused;
            if (paused) {
                std::cout << "paused" << std::endl;
            } else {
                std::cout << "unpaused" << std::endl;
            }
            break;
        case 27:  // VK_ESCAPE
            quit = true;
            std::cout << "quitting!" << std::endl;
            break;
        }
    }

    // Flush stdin, and wait for new input.
    FlushConsoleInputBuffer(_stdin);
}
