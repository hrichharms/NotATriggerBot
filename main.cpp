#include <windows.h>
#include <wtypes.h>
#include <iostream>
#include <math.h>

//#include <chrono>

int color_sum(HDC device_handle, int x, int y) {
    COLORREF rgb = GetPixel(device_handle, x, y);
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

    // int key = 0x46; // f key
    int key = 0x43;
    int start_color_sum, last_color_sum;
    int threshold = 20;
    //int key = 0x43; // c key
    //std::cout << GetAsyncKeyState(key) << std::endl;

    while (1) {
        // wait for key to be pressed and released
        while (!GetAsyncKeyState(key)) {}
        while (GetAsyncKeyState(key)) {}

        // get the current color sum
        start_color_sum = color_sum(device_handle, check_x, check_y);

        // wait for either the key to be pressed again or the color sum of the sample pixel to change passed the threshold
        last_color_sum = color_sum(device_handle, check_x, check_y);
        while (!GetAsyncKeyState(key) && std::abs(last_color_sum - start_color_sum) < threshold) {
            last_color_sum = color_sum(device_handle, check_x, check_y);
        }

        // click
        click();

    }

    //using std::chrono::high_resolution_clock;
    //using std::chrono::duration_cast;
    //using std::chrono::duration;
    //using std::chrono::milliseconds;
    //auto t1 = high_resolution_clock::now();
    //color_sum(device_handle, check_x, check_y);
    //click();
    //auto t2 = high_resolution_clock::now();
    //auto ms_int = duration_cast<milliseconds>(t2 - t1);
    //duration<double, std::milli> ms_double = t2 - t1;
    //std::cout << ms_int.count() << "ms\n";
    //std::cout << ms_double.count() << "ms\n";

    return 0;
}
