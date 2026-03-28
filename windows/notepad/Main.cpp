// Main.cpp
// Win32 Notepad clone - entry point.
//
// Demonstrates:
//   - Win32 window class registration and message loop
//   - Hosting an Edit control for multiline text editing
//   - File open/save with GetOpenFileName / GetSaveFileName
//   - Registry-based window position save/restore
//   - Programmatic menu creation (no .rc resource file needed)

#include <Windows.h>
#include "NotepadWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                   LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    if (!CNotepadWindow::RegisterWindowClass(hInstance))
        return 1;

    CNotepadWindow wnd;
    if (wnd.get_HWND())
    {
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CNotepadWindow::UnregisterWindowClass();
    return 0;
}
