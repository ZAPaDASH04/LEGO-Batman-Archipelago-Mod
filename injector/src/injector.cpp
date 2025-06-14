
// /**
//  * @file injector.cpp
//  * @author UNCLEAR. given by: pasdalover on discord https://discord.com/channels/539431629718945793/765144687479750676/1381908151514697730 who claims 50% ai generated.
//  * @brief 
//  * @version 0.1
//  * @date 2025-06-11
//  * 
//  * @copyright Copyright (c) 2025
//  * 
//  */


// #include <stdlib.h> // Not strictly needed now, but std headers are good practice
// #include <stdio.h>  // For printf, wprintf
// #include <windows.h>
// #include <wchar.h>  // For wcslen

// int main(int argc, char *argv[]) {
//     // Initialize handles to NULL for better error checking later
//     HANDLE processHandle = NULL; // Handle for injection target
//     PVOID remoteBuffer = NULL;
//     HANDLE hThread = NULL;       // Handle for remote thread

//     // --- Configuration ---
//     // 1. Specify the executable to launch
//     wchar_t exePath[] = L"LEGOBatman.exe -windowed"; // <--- CHANGE THIS to the target executable

//     // 2. Specify the DLL to inject
//     // IMPORTANT: Use the FULL PATH to your DLL for reliability.
//     wchar_t dllPath[] = L"mod.dll"; // <--- USE YOUR DLL PATH

//     DWORD processID = 0; // Will be filled by CreateProcess
//     PROCESS_INFORMATION pi = {0}; // Receives info about the new process
//     STARTUPINFOW si = {0};        // Specifies startup properties
//     si.cb = sizeof(si);           // Must set the size field

//     // --- 1. Launch the Target Process ---
//     printf("[*] Attempting to launch executable: %ls\n", exePath);

//     // Create the process. NULL for app name, use command line.
//     // 0 for creation flags to run normally.
//     if (!CreateProcessW(
//             NULL,          // No module name (use command line)
//             exePath,       // Command line (path to executable)
//             NULL,          // Process handle not inheritable
//             NULL,          // Thread handle not inheritable
//             FALSE,         // Set handle inheritance to FALSE
//             0,             // No creation flags (run normally)
//             NULL,          // Use parent's environment block
//             NULL,          // Use parent's starting directory
//             &si,           // Pointer to STARTUPINFO structure
//             &pi)           // Pointer to PROCESS_INFORMATION structure (receives handles/IDs)
//        )
//     {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to create process '%ls'.\n", exePath);
//         printf(" L.. CreateProcessW Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         return 1; // Indicate failure
//     }

//     // Process created successfully
//     processID = pi.dwProcessId;
//     printf("[+] Successfully launched process. PID: %lu\n", processID);
//     printf("[*] Target process handle: %p, Target thread handle: %p\n", pi.hProcess, pi.hThread);

//     // --- We don't need the primary thread handle, and we'll get a new ---
//     // --- process handle with specific rights, so close these handles ---
//     CloseHandle(pi.hProcess);
//     CloseHandle(pi.hThread);
//     printf("[*] Closed initial process and thread handles returned by CreateProcess.\n");

//     // Optional: Add a small delay to allow the process to initialize a bit
//     // Sleep(1000); // Sleep for 1 second (1000 milliseconds)


//     // --- Now, Proceed with DLL Injection into the new PID ---
//     printf("[*] Attempting to inject DLL into newly created PID: %lu\n", processID);
//     // Print DLL path (using the safe conversion method)
//     char mbPath[MAX_PATH * 2];
//     if (WideCharToMultiByte(CP_ACP, 0, dllPath, -1, mbPath, sizeof(mbPath), NULL, NULL) > 0) {
//         printf("DLL Path: %s\n", mbPath);
//     } else {
//         wprintf(L"DLL Path: %s (Warning: multibyte conversion failed!)\n", dllPath);
//     }

//     // --- 2. Open Target Process (with necessary injection rights) ---
//     processHandle = OpenProcess(
//         PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
//         PROCESS_VM_WRITE | PROCESS_VM_READ,
//         FALSE, processID);

//     if (processHandle == NULL) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to open the newly created process for injection.\n");
//         printf(" L.. OpenProcess Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         // Process might have terminated quickly, or other permission issues.
//         return 1; // Indicate failure
//     }
//     printf("[+] Successfully opened target process for injection.\n");

//     // --- 3. Allocate Memory in Target Process ---
//     SIZE_T dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
//     remoteBuffer = VirtualAllocEx(processHandle, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
//     if (remoteBuffer == NULL) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to allocate memory in target process.\n");
//         printf(" L.. VirtualAllocEx Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         CloseHandle(processHandle);
//         return 1;
//     }
//     printf("[+] Successfully allocated memory in target process at address: %p (Size: %llu bytes)\n", remoteBuffer, (unsigned long long)dllPathSize);

//     // --- 4. Write DLL Path to Target Process Memory ---
//     if (!WriteProcessMemory(processHandle, remoteBuffer, dllPath, dllPathSize, NULL)) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to write DLL path to target process memory.\n");
//         printf(" L.. WriteProcessMemory Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
//         CloseHandle(processHandle);
//         return 1;
//     }
//     printf("[+] Successfully wrote DLL path to target process memory.\n");

//     // --- 5. Get Address of LoadLibraryW ---
//     HMODULE hKernel32 = GetModuleHandleW(L"Kernel32");
//     if (hKernel32 == NULL) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to get handle for Kernel32.dll.\n");
//         printf(" L.. GetModuleHandleW Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
//         CloseHandle(processHandle);
//         return 1;
//      }
//     LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryW");
//     if (loadLibraryAddr == NULL) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to get the address of LoadLibraryW function.\n");
//         printf(" L.. GetProcAddress Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
//         CloseHandle(processHandle);
//         return 1;
//     }
//     printf("[+] Successfully got address of LoadLibraryW: %p\n", loadLibraryAddr);

//     // --- 6. Create Remote Thread to Execute LoadLibraryW ---
//     hThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteBuffer, 0, NULL);
//     if (hThread == NULL) {
//         DWORD errorCode = GetLastError();
//         printf("[-] ERROR: Failed to create remote thread in target process.\n");
//         printf(" L.. CreateRemoteThread Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//         VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
//         CloseHandle(processHandle);
//         return 1;
//     }
//     printf("[+] Successfully created remote thread. DLL injection initiated.\n");
//     printf("[*] Waiting for remote thread to finish (LoadLibraryW call)...\n");

//     // --- 7. Wait for LoadLibraryW to Complete and Check Result ---
//     WaitForSingleObject(hThread, INFINITE); // Wait indefinitely

//     DWORD remoteThreadExitCode = 0;
//     if (GetExitCodeThread(hThread, &remoteThreadExitCode)) {
//         printf("[*] Remote thread finished.\n");
//         printf(" L.. Exit Code (LoadLibraryW Result HMODULE / 0 on failure): %lu (0x%lX)\n",
//                remoteThreadExitCode, remoteThreadExitCode);
//         if (remoteThreadExitCode == 0) {
//              printf("[-] >>> LoadLibraryW FAILED within the target process! <<<\n");
//              printf("     Common Checks:\n");
//              // Use conversion for path printing
//              char mbPathCheck[MAX_PATH * 2];
//              if (WideCharToMultiByte(CP_ACP, 0, dllPath, -1, mbPathCheck, sizeof(mbPathCheck), NULL, NULL) > 0) {
//                  printf("       - DLL Path Correct? (Try full path: \"%s\")\n", mbPathCheck);
//              } else {
//                  printf("       - DLL Path Correct? (Try full path: [Conversion Failed])\n");
//              }
//              printf("       - Architecture Mismatch? (Injector/Target/DLL must ALL be 32-bit or ALL 64-bit)\n");
//              printf("       - Missing DLL Dependencies? (Use Dependencies tool on the DLL)\n");
//              printf("       - Filesystem Permissions? (Can target process *read* the DLL?)\n");
//              printf("       - Security Software? (AV/EDR/Anti-Cheat blocking the load?)\n");
//         } else {
//              printf("[+] >>> LoadLibraryW SUCCEEDED in the target process! (HMODULE: 0x%lX) <<<\n", remoteThreadExitCode);
//              printf("     If the DLL's effect isn't visible, the issue is likely within the DLL's DllMain or other internal logic.\n");
//         }
//     } else {
//          DWORD errorCode = GetLastError();
//          printf("[-] ERROR: Failed to get remote thread exit code.\n");
//          printf(" L.. GetExitCodeThread Error Code: %lu (0x%lX)\n", errorCode, errorCode);
//     }

//     // --- 8. Cleanup ---
//     printf("[*] Cleaning up handles and memory...\n");
//     if (hThread != NULL) CloseHandle(hThread); // Close remote thread handle
//     if (remoteBuffer != NULL) VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE); // Free remote memory
//     if (processHandle != NULL) CloseHandle(processHandle); // Close injection process handle

//     printf("[*] Injector finished.\n");
//     return 0; // Indicate success
// }


#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>

// Modify as needed
const wchar_t* TARGET_PROCESS = L"LEGOBatman.exe";
const wchar_t* DLL_PATH = L"mod.dll"; // Use full absolute path?

DWORD FindProcessId(const wchar_t* procName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(entry);

    DWORD pid = 0;
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, procName) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}


bool IsMainModuleLoaded(DWORD pid, const wchar_t* moduleName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32W me;
    me.dwSize = sizeof(me);

    if (Module32FirstW(hSnapshot, &me)) {
        do {
            if (_wcsicmp(me.szModule, moduleName) == 0) {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Module32NextW(hSnapshot, &me));
    }

    CloseHandle(hSnapshot);
    return false;
}

int main() {
    std::wcout << L"[*] Waiting for process: " << TARGET_PROCESS << std::endl;

    DWORD pid = 0;
    while ((pid = FindProcessId(TARGET_PROCESS)) == 0) {
        Sleep(500); // Wait until the game is running
    }

    std::wcout << L"[+] Found process PID: " << pid << std::endl;
    std::wcout << L"[+] Waiting for module: LEGOBatman.exe" << std::endl;

    while (!IsMainModuleLoaded(pid, L"LEGOBatman.exe")) {
        std::wcout << L"[*] Waiting for main module...\n";
        Sleep(500);
    }
    std::wcout << L"[+] Main module is loaded" << std::endl;

    // TODO: Test this
    while (!IsMainModuleLoaded(pid, L"xinput1_3.dll")) {
        std::wcout << L"[*] Waiting for final module...\n";
        Sleep(500);
    }

    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid);

    if (!hProcess) {
        std::cerr << "[-] Failed to open target process." << std::endl;
        return 1;
    }

    size_t pathLen = (wcslen(DLL_PATH) + 1) * sizeof(wchar_t);
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        std::cerr << "[-] Failed to allocate memory in target process." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, remoteMem, DLL_PATH, pathLen, NULL)) {
        std::cerr << "[-] Failed to write DLL path to target process memory." << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HMODULE hKernel32 = GetModuleHandleW(L"Kernel32.dll");
    FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryW");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr,
        remoteMem, 0, NULL);

    if (!hThread) {
        std::cerr << "[-] Failed to create remote thread." << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] DLL injection thread created." << std::endl;

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);
    std::cout << "[*] LoadLibraryW return (HMODULE): 0x" << std::hex << exitCode << std::endl;

    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "[*] Done." << std::endl;
    return 0;
}