/**
 * execute_process.cpp
 *
 * Demonstrates how to launch an external process on Windows using:
 *   1. AssocQueryString  - resolve a file extension to its associated executable
 *   2. CreateProcess     - spawn the resolved executable with the target file
 *   3. WaitForSingleObject - optionally block until the child process exits
 *
 * A ShellExecuteEx fallback is also provided for cases where the caller just
 * wants the OS to open a file with its default handler (verb = "open").
 *
 * Originally based on WINAPI_runprocess practice code; cleaned up and
 * bug-fixed (inverted NULL check on cleanup path was corrected).
 */

#include <Windows.h>
#include <Shlwapi.h>   // AssocQueryString, PathFindExtension
#include <tchar.h>
#include <Strsafe.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

// ---------------------------------------------------------------------------
// ExecuteProcess
//
// Resolves the associated application for |path| via its file extension,
// then launches it with CreateProcess.  If |bWait| is TRUE the call blocks
// until the child terminates.
//
// Returns TRUE on success.
// ---------------------------------------------------------------------------
BOOL ExecuteProcess(const wchar_t* path, BOOL bWait)
{
    if (path == NULL)
    {
        _tprintf(_T("[ERROR] path is NULL\n"));
        return FALSE;
    }

    // --- 1. Copy path into a mutable buffer (CreateProcessW requirement) ---
    TCHAR mutablePath[_MAX_PATH] = {};
    _tcscpy_s(mutablePath, path);

    // --- 2. Resolve associated executable via file extension ---------------
    TCHAR app[_MAX_PATH] = {};
    DWORD size = _MAX_PATH;

    SetLastError(0);
    HRESULT hr = AssocQueryString(
        0, ASSOCSTR_EXECUTABLE,
        PathFindExtension(mutablePath),
        NULL, app, &size);

    if (hr != S_OK)
    {
        _tprintf(_T("[ERROR] AssocQueryString failed (hr=0x%08X, LastError=%d)\n"),
                 hr, GetLastError());
        return FALSE;
    }

    _tprintf(_T("[INFO] Associated application: %s\n"), app);

    // --- 3. CreateProcess --------------------------------------------------
    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(STARTUPINFO);

    BOOL bSuccess = CreateProcess(
        app,                    // lpApplicationName
        mutablePath,            // lpCommandLine (file to open)
        NULL, NULL,             // security attributes
        FALSE,                  // inherit handles
        NORMAL_PRIORITY_CLASS,  // creation flags
        NULL,                   // environment
        NULL,                   // current directory
        &si, &pi);

    if (!bSuccess)
    {
        _tprintf(_T("[ERROR] CreateProcess failed (LastError=%d)\n"), GetLastError());
        return FALSE;
    }

    // --- 4. Optionally wait for the child to finish -----------------------
    if (bWait)
    {
        _tprintf(_T("[INFO] Waiting for process to exit...\n"));
        DWORD waitResult = WaitForSingleObject(pi.hProcess, INFINITE);

        if (waitResult == WAIT_OBJECT_0)
        {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(pi.hProcess, &exitCode))
            {
                _tprintf(_T("[INFO] Process exited with code %lu\n"), exitCode);
            }
        }
        else
        {
            _tprintf(_T("[WARN] WaitForSingleObject returned %lu (LastError=%d)\n"),
                     waitResult, GetLastError());
        }
    }

    // --- 5. Cleanup -------------------------------------------------------
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return TRUE;
}

// ---------------------------------------------------------------------------
// ShellExecuteExFallback
//
// Opens |path| with the system default handler via ShellExecuteEx.
// If |bWait| is TRUE the call blocks until the launched process exits.
// ---------------------------------------------------------------------------
BOOL ShellExecuteExFallback(const wchar_t* path, BOOL bWait)
{
    if (path == NULL)
        return FALSE;

    SHELLEXECUTEINFO sei = {};
    sei.cbSize = sizeof(sei);
    sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
    sei.lpFile = path;
    sei.nShow  = SW_SHOW;

    if (!ShellExecuteEx(&sei))
    {
        _tprintf(_T("[ERROR] ShellExecuteEx failed (LastError=%d)\n"), GetLastError());
        return FALSE;
    }

    if (bWait && sei.hProcess != NULL)
    {
        _tprintf(_T("[INFO] Waiting for process (ShellExecuteEx)...\n"));
        WaitForSingleObject(sei.hProcess, INFINITE);

        DWORD exitCode = 0;
        if (GetExitCodeProcess(sei.hProcess, &exitCode))
        {
            _tprintf(_T("[INFO] Process exited with code %lu\n"), exitCode);
        }

        CloseHandle(sei.hProcess);
    }

    return TRUE;
}

// ---------------------------------------------------------------------------
// main - demonstration
// ---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        _tprintf(_T("Usage: execute_process <file_path> [wait]\n"));
        _tprintf(_T("  file_path  - path to a file (e.g. C:\\docs\\report.xlsx)\n"));
        _tprintf(_T("  wait       - if specified, block until process exits\n"));
        return 1;
    }

    const wchar_t* filePath = argv[1];
    BOOL bWait = (argc >= 3);

    _tprintf(_T("--- Method 1: AssocQueryString + CreateProcess ---\n"));
    if (!ExecuteProcess(filePath, bWait))
    {
        _tprintf(_T("--- Falling back to ShellExecuteEx ---\n"));
        ShellExecuteExFallback(filePath, bWait);
    }

    return 0;
}
