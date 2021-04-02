#include <windows.h>
#include <wtypes.h>
#include <math.h>


int color_sum(HDC device_handle, int x, int y) {
    // get COLORREF data from windows api
    COLORREF rgb = GetPixel(device_handle, x, y);
    // return sum of r, g, and b channel values
    return GetRValue(rgb) + GetGValue(rgb) + GetBValue(rgb);
}


void click() {
    INPUT Input = { 0 };

    // mouse down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

    // mouse up
    ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}


int main() {

    // get window and device handles
    HWND desktop_window = GetDesktopWindow();
    HDC device_handle = GetDC(desktop_window);

    // get desktop screen dimensions
    RECT desktop;
    GetWindowRect(desktop_window, &desktop);
    int WIDTH = desktop.right;
    int HEIGHT = desktop.bottom;

    // calculate sample pixel coordinates
    int check_x = WIDTH / 2 + 2;
    int check_y = HEIGHT / 2 + 2;

    // macro settings
    int key = 0x54; // 't' key virtual key code
    int threshold = 30;

    // initialize disengagement boolean
    bool disengaged = 0;

    // define sum variables
    int start_color_sum, last_color_sum;

    // macro loop
    while (1) {
        // wait for key to be pressed and released
        while (!GetAsyncKeyState(key)) {}
        while (GetAsyncKeyState(key)) {}

        // get the current color sum
        start_color_sum = color_sum(device_handle, check_x, check_y);

        // wait for either the key to be pressed again or the color sum of the sample pixel to change passed the threshold
        last_color_sum = color_sum(device_handle, check_x, check_y);
        disengaged = 0;
        while (std::abs(last_color_sum - start_color_sum) < threshold) {
            last_color_sum = color_sum(device_handle, check_x, check_y);
            if (GetAsyncKeyState(key)) {
                disengaged = 1;
                break;
            }
        }

        if (disengaged) {
            // wait for the key to be released before continuing macro loop
            while (GetAsyncKeyState(key)) {}
        } else {
            // click
            click();
        }

    }

    return 0;
}
