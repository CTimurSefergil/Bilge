#include "logger.h"
#include "..\..\defines.h"

#include <io.h>
#include <fcntl.h>
#include <windows.h>

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static HANDLE stdout_handle;
static HANDLE stderr_handle;
static b8 console_allocated = FALSE;

b8 initialize_logging()
{
    // Check if we already have a console (Console Application)
    if (GetConsoleWindow() == NULL)
    {
        // No console exists, create one (Windows Application)
        if (!AllocConsole())
        {
            return FALSE;
        }
        console_allocated = TRUE;

        // Redirect stdout, stdin, stderr to the new console
        FILE* pCout;
        FILE* pCin;
        FILE* pCerr;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        freopen_s(&pCin, "CONIN$", "r", stdin);
        freopen_s(&pCerr, "CONOUT$", "w", stderr);
    }
    else
    {
        // Console already exists (Console Application)
        console_allocated = FALSE;
    }

    // Set console title
    SetConsoleTitleA("Engine Debug Output");

    // Get console handles for colored output
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

    if (stdout_handle == INVALID_HANDLE_VALUE || stderr_handle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    // Enable virtual terminal processing for ANSI color codes (Windows 10+)
    DWORD stdout_mode = 0;
    DWORD stderr_mode = 0;

    if (GetConsoleMode(stdout_handle, &stdout_mode))
    {
        SetConsoleMode(stdout_handle, stdout_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    if (GetConsoleMode(stderr_handle, &stderr_mode))
    {
        SetConsoleMode(stderr_handle, stderr_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    return TRUE;
}

void shutdown_logging()
{
    // Only free console if we allocated it
    if (console_allocated)
    {
        FreeConsole();
    }
}

void log_output(log_level level, const char* message, ...)
{
    const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: " };

    const char* level_colors[6] =
    {
        "\033[41;37m",  // FATAL: Red background, white text
        "\033[91m",     // ERROR: Bright red text
        "\033[93m",     // WARN:  Bright yellow text
        "\033[92m",     // INFO:  Bright green text
        "\033[96m",     // DEBUG: Bright cyan text
        "\033[95m"      // TRACE: Bright magenta text
    };

    const char* color_reset = "\033[0m";

    b8 is_error = level < LOG_LEVEL_WARN;

    const i32 msg_length = 4000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    // Format original message.
    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[msg_length];
    sprintf_s(out_message2, "%s%s%s%s\n",
        level_colors[level],    // Start color
        level_strings[level],   // Level string
        out_message,           // Formatted message
        color_reset);          // Reset color

    if (is_error)
    {
        fprintf(stderr, "%s", out_message2);
        fflush(stderr);
    }
    else
    {
        printf("%s", out_message2);
        fflush(stdout);
    }
}