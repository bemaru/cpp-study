// NotepadWindow.cpp
// Win32 Notepad clone - implements the main window with an Edit control,
// file open/save via common dialogs, and registry-based window position save/restore.
//
// Key fixes from the original version:
//   - ReadFile bug: the original read in a loop but overwrote the buffer each iteration,
//     so only the last chunk survived. Now accumulates all data with std::string.
//   - Fixed buffer sizes: replaced stack-allocated WCHAR[512]/WCHAR[1024] with
//     dynamic allocation via std::wstring / std::string.
//   - Replaced GetWindowLong/SetWindowLong with GetWindowLongPtr/SetWindowLongPtr
//     for 64-bit compatibility.

#include "NotepadWindow.h"
#include <commdlg.h>
#include <string>
#include <vector>

LPCTSTR CNotepadWindow::mClassName = L"NotepadWindowClass";
HINSTANCE CNotepadWindow::mhInstance = nullptr;

// ---------------------------------------------------------------------------
// Constructor: restore window position from registry, then create the window.
// ---------------------------------------------------------------------------
CNotepadWindow::CNotepadWindow()
{
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    int w = CW_USEDEFAULT;
    int h = CW_USEDEFAULT;

    // Restore last window position from the registry
    HKEY hKey = nullptr;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Notepad", 0, nullptr, 0,
                       KEY_ALL_ACCESS, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        DWORD type = REG_DWORD;
        DWORD size = sizeof(int);
        RegQueryValueEx(hKey, L"iWindowPosX",  nullptr, &type, reinterpret_cast<LPBYTE>(&x), &size);
        RegQueryValueEx(hKey, L"iWindowPosY",  nullptr, &type, reinterpret_cast<LPBYTE>(&y), &size);
        RegQueryValueEx(hKey, L"iWindowPosDX", nullptr, &type, reinterpret_cast<LPBYTE>(&w), &size);
        RegQueryValueEx(hKey, L"iWindowPosDY", nullptr, &type, reinterpret_cast<LPBYTE>(&h), &size);
        RegCloseKey(hKey);
    }

    mhWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
        mClassName, L"Untitled - Notepad",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        x, y, w, h,
        nullptr, nullptr, mhInstance, this);
}

// ---------------------------------------------------------------------------
// Destructor: persist the current window rectangle to the registry.
// ---------------------------------------------------------------------------
CNotepadWindow::~CNotepadWindow()
{
    HKEY hKey = nullptr;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Notepad", 0, nullptr, 0,
                       KEY_ALL_ACCESS, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        int value = mWindowRect.left;
        RegSetValueEx(hKey, L"iWindowPosX", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        value = mWindowRect.top;
        RegSetValueEx(hKey, L"iWindowPosY", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        value = mWindowRect.right - mWindowRect.left;
        RegSetValueEx(hKey, L"iWindowPosDX", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        value = mWindowRect.bottom - mWindowRect.top;
        RegSetValueEx(hKey, L"iWindowPosDY", 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        RegCloseKey(hKey);
    }
}

HWND CNotepadWindow::get_HWND() const
{
    return mhWnd;
}

// ---------------------------------------------------------------------------
// Build the application menu programmatically (replaces the .rc resource).
// ---------------------------------------------------------------------------
HMENU CNotepadWindow::CreateMainMenu()
{
    HMENU hMenuBar  = ::CreateMenu();
    HMENU hFileMenu = ::CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING,    IDM_FILE_NEW,    L"&New");
    AppendMenu(hFileMenu, MF_STRING,    IDM_FILE_OPEN,   L"&Open...");
    AppendMenu(hFileMenu, MF_STRING,    IDM_FILE_SAVE,   L"&Save");
    AppendMenu(hFileMenu, MF_STRING,    IDM_FILE_SAVEAS, L"Save &As...");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hFileMenu, MF_STRING,    IDM_FILE_EXIT,   L"E&xit");

    AppendMenu(hMenuBar, MF_POPUP, reinterpret_cast<UINT_PTR>(hFileMenu), L"&File");
    return hMenuBar;
}

// ---------------------------------------------------------------------------
// Register the window class with a programmatic menu.
// ---------------------------------------------------------------------------
bool CNotepadWindow::RegisterWindowClass(HINSTANCE hInstance)
{
    mhInstance = hInstance;

    WNDCLASSEX wc = {};
    wc.cbSize        = sizeof(wc);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance     = hInstance;
    wc.lpszClassName = mClassName;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    // No lpszMenuName - we attach the menu in WM_CREATE instead.

    return RegisterClassEx(&wc) != 0;
}

void CNotepadWindow::UnregisterWindowClass()
{
    UnregisterClass(mClassName, mhInstance);
}

// ---------------------------------------------------------------------------
// Window procedure
// ---------------------------------------------------------------------------
LRESULT CALLBACK CNotepadWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CNotepadWindow* wnd = reinterpret_cast<CNotepadWindow*>(
        GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Store the CNotepadWindow pointer for later retrieval
        auto cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA,
                         reinterpret_cast<LONG_PTR>(cs->lpCreateParams));

        // Attach the programmatic menu
        SetMenu(hWnd, CreateMainMenu());

        // Create the Edit control
        if (!CreateWindowEx(
                0, L"Edit", nullptr,
                WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE |
                    WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
                0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                hWnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_EDIT)),
                reinterpret_cast<HINSTANCE>(
                    GetWindowLongPtr(hWnd, GWLP_HINSTANCE)),
                nullptr))
        {
            return -1; // fail creation
        }
        return 0;
    }

    case WM_SETFOCUS:
        SetFocus(GetDlgItem(hWnd, IDC_EDIT));
        return 0;

    case WM_SIZE:
        MoveWindow(GetDlgItem(hWnd, IDC_EDIT),
                   0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        if (wnd)
            GetWindowRect(hWnd, &wnd->mWindowRect);
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        // ---- File > New ----
        case IDM_FILE_NEW:
            SetDlgItemText(hWnd, IDC_EDIT, L"");
            SendMessage(GetDlgItem(hWnd, IDC_EDIT), EM_SETMODIFY, FALSE, 0);
            return 0;

        // ---- File > Open ----
        case IDM_FILE_OPEN:
        {
            WCHAR lpstrFile[MAX_PATH] = L"";
            OPENFILENAME ofn = {};
            ofn.lStructSize  = sizeof(OPENFILENAME);
            ofn.hwndOwner    = hWnd;
            ofn.lpstrFilter  = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile    = lpstrFile;
            ofn.nMaxFile     = MAX_PATH;
            ofn.lpstrInitialDir = L".";
            ofn.Flags        = OFN_FILEMUSTEXIST;

            if (!GetOpenFileName(&ofn))
                return 0;

            HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ,
                                      nullptr, OPEN_EXISTING, 0, nullptr);
            if (hFile == INVALID_HANDLE_VALUE)
                return 0;

            // Read the entire file into a byte buffer (fixes the original
            // bug where only the last chunk was kept).
            DWORD fileSize = GetFileSize(hFile, nullptr);
            std::string fileData;
            if (fileSize != INVALID_FILE_SIZE && fileSize > 0)
            {
                fileData.resize(fileSize);
                DWORD totalRead = 0;
                while (totalRead < fileSize)
                {
                    DWORD nread = 0;
                    if (!ReadFile(hFile, &fileData[totalRead],
                                  fileSize - totalRead, &nread, nullptr) || nread == 0)
                        break;
                    totalRead += nread;
                }
                fileData.resize(totalRead);
            }
            CloseHandle(hFile);

            // Convert from the file's byte content to a wide string.
            // Assume the file is encoded in the system default codepage (ANSI).
            int wideLen = MultiByteToWideChar(CP_ACP, 0, fileData.data(),
                                              static_cast<int>(fileData.size()), nullptr, 0);
            std::wstring wideText(wideLen, L'\0');
            MultiByteToWideChar(CP_ACP, 0, fileData.data(),
                                static_cast<int>(fileData.size()), &wideText[0], wideLen);

            SetDlgItemText(hWnd, IDC_EDIT, wideText.c_str());
            return 0;
        }

        // ---- File > Save ----
        case IDM_FILE_SAVE:
        {
            WCHAR lpstrFile[MAX_PATH] = L"";
            OPENFILENAME sfn = {};
            sfn.lStructSize  = sizeof(OPENFILENAME);
            sfn.hwndOwner    = hWnd;
            sfn.lpstrFilter  = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            sfn.lpstrFile    = lpstrFile;
            sfn.nMaxFile     = MAX_PATH;
            sfn.lpstrInitialDir = L".";
            sfn.Flags        = OFN_OVERWRITEPROMPT;

            if (!GetSaveFileName(&sfn))
                return 0;

            // Get the text length and allocate accordingly (fixes fixed-size buffer)
            int textLen = GetWindowTextLength(GetDlgItem(hWnd, IDC_EDIT));
            std::wstring text(textLen + 1, L'\0');
            GetDlgItemText(hWnd, IDC_EDIT, &text[0], textLen + 1);

            HANDLE hFile = CreateFile(sfn.lpstrFile, GENERIC_WRITE, 0,
                                      nullptr, CREATE_ALWAYS, 0, nullptr);
            if (hFile == INVALID_HANDLE_VALUE)
                return 0;

            DWORD nwrite = 0;
            WriteFile(hFile, text.c_str(),
                      static_cast<DWORD>(textLen * sizeof(WCHAR)), &nwrite, nullptr);
            CloseHandle(hFile);
            return 0;
        }

        // ---- File > Exit ----
        case IDM_FILE_EXIT:
            DestroyWindow(hWnd);
            return 0;

        case IDC_EDIT:
            // Notification from the Edit control - pass to default
            break;
        }
        break;
    }

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
