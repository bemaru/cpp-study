// NotepadWindow.h
// Win32 Notepad clone - demonstrates window class registration, Edit control,
// registry-based window position persistence, and file I/O with common dialogs.
//
// Original: 2017, migrated and cleaned up 2026.

#pragma once

#include <Windows.h>
#include <string>

// Control and menu command IDs (originally from resource.h / notepad.rc)
#define IDC_EDIT            10001
#define IDM_FILE_NEW        40008
#define IDM_FILE_OPEN       40009
#define IDM_FILE_SAVE       40010
#define IDM_FILE_SAVEAS     40011
#define IDM_FILE_EXIT       40014

class CNotepadWindow
{
public:
    CNotepadWindow();
    ~CNotepadWindow();

    HWND get_HWND() const;

    static bool RegisterWindowClass(HINSTANCE hInstance);
    static void UnregisterWindowClass();

private:
    static LPCTSTR mClassName;
    static HINSTANCE mhInstance;

    HWND mhWnd = nullptr;
    RECT mWindowRect = {};

    // Build the File menu programmatically (no .rc needed)
    static HMENU CreateMainMenu();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
