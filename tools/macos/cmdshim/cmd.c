/* Minimal cmd.exe for wibo: runs "cmd.exe /c <command line>" by CreateProcessA.
   armcc spawns armasm through %COMSPEC% for embedded __asm functions. */
typedef void *HANDLE;
typedef struct { unsigned cb; char *r1, *desk, *title; unsigned x, y, xs, ys, xc, yc, fill, flags;
                 unsigned short show, r2; void *r3; HANDLE in, out, err; } STARTUPINFOA;
typedef struct { HANDLE proc, thread; unsigned pid, tid; } PROCESS_INFORMATION;
__declspec(dllimport) char *__stdcall GetCommandLineA(void);
__declspec(dllimport) int __stdcall CreateProcessA(const char *, char *, void *, void *, int, unsigned,
                                                  void *, const char *, STARTUPINFOA *, PROCESS_INFORMATION *);
__declspec(dllimport) unsigned __stdcall WaitForSingleObject(HANDLE, unsigned);
__declspec(dllimport) int __stdcall GetExitCodeProcess(HANDLE, unsigned *);
__declspec(dllimport) void __stdcall ExitProcess(unsigned);

void mainCRTStartup(void) {
    char *p = GetCommandLineA();
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi;
    unsigned code = 1;
    /* skip to "/c" or "/C" */
    for (; *p; p++) {
        if (p[0] == '/' && (p[1] == 'c' || p[1] == 'C') && (p[2] == ' ' || p[2] == '\t')) {
            p += 3;
            break;
        }
    }
    while (*p == ' ' || *p == '\t') p++;
    if (!*p) ExitProcess(1);
    si.cb = sizeof(si);
    if (!CreateProcessA(0, p, 0, 0, 1, 0, 0, 0, &si, &pi)) ExitProcess(1);
    WaitForSingleObject(pi.proc, 0xFFFFFFFF);
    GetExitCodeProcess(pi.proc, &code);
    ExitProcess(code);
}
