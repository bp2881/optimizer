#include <windows.h>
#include <stdio.h>

DWORD WINAPI BatteryMonitorThread(LPVOID lpParam) {
    SYSTEM_POWER_STATUS status;

    while (1) {
        if (GetSystemPowerStatus(&status)) {
            int batteryLevel = status.BatteryLifePercent;
            int isCharging = status.ACLineStatus == 1; 

            if (batteryLevel >= 80 && isCharging) {
                MessageBox(NULL, "Battery is above 80% and still charging. Please unplug the charger.",
                           "Battery Notification", MB_OK | MB_ICONINFORMATION);
            } else if (batteryLevel < 40 && !isCharging) {
                MessageBox(NULL, "Battery is below 40% and not charging. Please plug in the charger.",
                           "Battery Notification", MB_OK | MB_ICONWARNING);
            }
        }
        // Sleep for 5 min
        Sleep(300000); 
    }

    return 0;
}

int main() {
    DWORD threadId;
    HANDLE hThread = CreateThread(
        NULL,        
        0,           
        BatteryMonitorThread, 
        NULL,        
        0,           
        &threadId);  

    if (hThread == NULL) {
        MessageBox(NULL, "Failed to create thread.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    return 0;
}
