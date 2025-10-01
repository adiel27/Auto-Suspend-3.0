#include "window_utils.h"

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    DWORD windowPid;
    GetWindowThreadProcessId(hwnd, &windowPid);

    // Cocokkan PID dan pastikan jendela terlihat
    if ((DWORD)lParam == windowPid && IsWindowVisible(hwnd)) {
        return FALSE; // Ditemukan, hentikan enumerasi
    }
    return TRUE; // Lanjutkan enumerasi
}

BOOL HasVisibleWindow(DWORD pid) {
    return !EnumWindows(EnumWindowsCallback, (LPARAM)pid);
}
