
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int main(void) {
    // bump us to real-time priority
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    // prepare a DOWN+UP pair in one syscall
    INPUT inputs[2] = {0};
    inputs[0].type           = INPUT_MOUSE;
    inputs[0].mi.dwFlags     = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type           = INPUT_MOUSE;
    inputs[1].mi.dwFlags     = MOUSEEVENTF_LEFTUP;

    // high-res timer setup
    LARGE_INTEGER freq, last, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&last);

    unsigned long long count = 0;
    printf("Max-speed autoclicker + CPS meter.\n");
    printf("Press ESC to stop.\n");

    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
        // send both events (one “click”)
        SendInput(2, inputs, sizeof(INPUT));
        count++;

        // check if one second elapsed
        QueryPerformanceCounter(&now);
        if ((now.QuadPart - last.QuadPart) >= freq.QuadPart) {
            // print & reset
            printf("CPS = %llu\n", count);
            count = 0;
            last  = now;
        }
    }
    return 0;
}
