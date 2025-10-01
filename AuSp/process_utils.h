#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <windows.h>

//Menangguhkan semua thread dalam proses
void SuspendProcess(DWORD pid);

//Melanjutkan thread
void ResumeProcess(DWORD pid);

//Cek apakah whitelist
BOOL IsExcluded(const char* processName);

//Fungsi memantau dan mengelola
void MonitorProcesses();

#endif // PROCESS_UTILS_H