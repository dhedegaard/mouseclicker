#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <windows.h>

#define DEFAULT_SLEEP_INTERVAL 25

int do_click();
void handle_input();
void do_sleep();

static HWND hwndWindow;
static RECT rcWindow;
static LPARAM position;
static HANDLE _stdin;
static bool paused = false;
static bool quit = false;
static unsigned long sleep_interval = DEFAULT_SLEEP_INTERVAL;

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
        }
    }

    // Introduction
    std::cout << "Keybinds are as follows:" << std::endl
              << "ENTER/SPACE:    Pause" << std::endl
              << "ESCAPE/Q:       Exit" << std::endl
              << "UP:             Decrease sleep interval by 1 ms" << std::endl
              << "DOWN:           Increase sleep interval by 1 ms" << std::endl
              << std::endl
              << "Sleep interval is: " << sleep_interval << " ms" << std::endl;

    // Get stdin for peeking later.
    _stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (_stdin == NULL) {
        std::cerr << "Uanble to get stdin" << std::endl;
        return 1;
    }

    // Fetch the window handle.
    hwndWindow = FindWindowA(NULL, "Clicker Heroes");
    if (hwndWindow == NULL) {
        std::cerr << "Unable to get window handle! Is the application running ?"
                  << std::endl;
        return 1;
    }

    std::cout << "clicking" << std::endl;
    while (!quit) {
        // While we're not paused and do_click returns nothing, continue.
        if (!paused) {
            if (do_click() != 0) {
                return 1;
            }
        }

        do_sleep();
    }

    // All done!
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
        std::cerr << "Position is NULL!" << std::endl;
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

void do_sleep() {
    // Get the time before the sleep.
    DWORD before_sleep = timeGetTime();

    // Handle input (if any).
    handle_input();

    // Do sleep.
    DWORD diff_ms = timeGetTime() - before_sleep;
    if (diff_ms < sleep_interval) {
        Sleep(sleep_interval - diff_ms);
    }
}

// Handles input form stdin.
void handle_input() {
    INPUT_RECORD input;
    DWORD number_of_events = 0;

    // While fetching the number of events do not fail.
    while (GetNumberOfConsoleInputEvents(_stdin, &number_of_events)) {

        // Stop if there are no events to handle.
        if (number_of_events <= 0) {
            break;
        }

        // Fetch an event from the input.
        ReadConsoleInput(_stdin, &input, 1, &number_of_events);

        // Check for specific keypresses.
        if (input.Event.KeyEvent.bKeyDown) {
            switch(input.Event.KeyEvent.wVirtualKeyCode) {
            case 13:  // VK_ENTER
            case 32:  // VK_SPACE
                paused = !paused;
                if (paused) {
                    std::cout << "paused" << std::endl;
                } else {
                    std::cout << "clicking" << std::endl;
                }
                break;
            case 27:  // VK_ESCAPE
            case 81:  // VK_Q
                quit = true;
                std::cout << "quitting!" << std::endl;
                break;
            case 38:  // VK_UP
                if (sleep_interval > 1) {
                    sleep_interval--;
                    std::cout << "Decreased sleep interval to "
                              << sleep_interval << std::endl;
                } else {
                    std::cout << "ERROR: Unable to decrease interval below 1 ms"
                              << std::endl;
                }
                break;
            case 40:  // VK_DOWN
                sleep_interval++;
                std::cout << "Increased sleep interval to "
                          << sleep_interval << std::endl;
                break;
            }
        }
    }
}
