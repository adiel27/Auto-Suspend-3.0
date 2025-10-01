#include "process_utils.h"
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include "window_utils.h"
#include "cpu_utils.h"
#include "log_utils.h"

// Daftar proses yang tidak boleh dibekukan
const char* excluded[] = {
    "System", "svchost.exe", "explorer.exe", "wininit.exe", "csrss.exe", "services.exe", "lsass.exe",
    "igfx.exe", "amddvr.exe", "RadeonSoftware.exe", "DolbyDAX2.exe", "DolbyCPL.exe", "MsMpEng.exe", "AuSp.exe"
};

BOOL IsExcluded(const char* processName) {
    for (int i = 0; i < sizeof(excluded) / sizeof(excluded[0]); i++) {
        if (_stricmp(processName, excluded[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void SuspendProcess(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    if (Thread32First(hSnapshot, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                if (hThread) {
                    SuspendThread(hThread);
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    CloseHandle(hSnapshot);
}

void ResumeProcess(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    if (Thread32First(hSnapshot, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                if (hThread) {
                    ResumeThread(hThread);
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    CloseHandle(hSnapshot);
}

void MonitorProcesses() {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (IsExcluded(pe32.szExeFile)) continue;

            if (!HasVisibleWindow(pe32.th32ProcessID)) continue;

            double cpu = GetCPUUsage(pe32.th32ProcessID);
            BOOL idle = cpu < 0.1;

            if (idle) {
                SuspendProcess(pe32.th32ProcessID);
                LogAction("Suspended", pe32.szExeFile, pe32.th32ProcessID);
            } else {
                ResumeProcess(pe32.th32ProcessID);
                LogAction("Resumed", pe32.szExeFile, pe32.th32ProcessID);
            }

        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
}
