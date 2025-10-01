#include "process_utils.h"
#include "window_utils.h"
#include "cpu_utils.h"
#include "log_utils.h"
#include "notify_utils.h"

int main() {
    ShowStartupNotification();
    while (1) {
        MonitorProcesses(); // Fungsi utama yang menggabungkan semua logika
        Sleep(10000);       // Cek setiap 10 detik
    }
    return 0;
}
