#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

void ui_init(void) {
#ifdef _WIN32
    // Enable ANSI escape sequence rendering in Windows console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void ui_clear_screen(void) {
    // ANSI clear screen and move cursor to (1,1)
    printf("\033[2J\033[H");
    fflush(stdout);
}

void ui_set_color(const char *color_ansi) {
    printf("%s", color_ansi);
}

void ui_reset_color(void) {
    printf("%s", ANSI_COLOR_RESET);
}

void ui_print_header(const char *title) {
    ui_set_color(ANSI_COLOR_CYAN);
    printf("================================================================================\n");
    printf("  %s%s%s  \n", ANSI_STYLE_BOLD, title, ANSI_COLOR_CYAN);
    printf("================================================================================\n");
    ui_reset_color();
}

void ui_print_success(const char *format, ...) {
    va_list args;
    va_start(args, format);
    ui_set_color(ANSI_COLOR_GREEN);
    printf("[+] ");
    vprintf(format, args);
    ui_reset_color();
    printf("\n");
    va_end(args);
}

void ui_print_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    ui_set_color(ANSI_COLOR_RED);
    printf("[!] ");
    vprintf(format, args);
    ui_reset_color();
    printf("\n");
    va_end(args);
}

void ui_print_warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    ui_set_color(ANSI_COLOR_YELLOW);
    printf("[*] ");
    vprintf(format, args);
    ui_reset_color();
    printf("\n");
    va_end(args);
}

void ui_print_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    ui_set_color(ANSI_COLOR_BLUE);
    printf("[i] ");
    vprintf(format, args);
    ui_reset_color();
    printf("\n");
    va_end(args);
}

int ui_prompt_int(const char *prompt, int min_val, int max_val) {
    int value;
    char buffer[256];
    while (1) {
        ui_set_color(ANSI_COLOR_YELLOW);
        printf("%s (%d to %d): ", prompt, min_val, max_val);
        ui_reset_color();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Parse integer
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        
        // Check if parsing succeeded and value is within bounds
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            ui_print_error("Invalid input. Please enter a valid integer.");
            continue;
        }

        if (val < min_val || val > max_val) {
            ui_print_error("Value out of bounds. Must be between %d and %d.", min_val, max_val);
            continue;
        }

        value = (int)val;
        break;
    }
    return value;
}

void ui_prompt_string(const char *prompt, char *buffer, int max_len) {
    while (1) {
        ui_set_color(ANSI_COLOR_YELLOW);
        printf("%s: ", prompt);
        ui_reset_color();

        if (fgets(buffer, max_len, stdin) == NULL) {
            continue;
        }

        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == 0) {
            ui_print_error("Input cannot be empty.");
            continue;
        }

        break;
    }
}

double ui_prompt_double(const char *prompt, double min_val, double max_val) {
    double value;
    char buffer[256];
    while (1) {
        ui_set_color(ANSI_COLOR_YELLOW);
        printf("%s (%.2f to %.2f): ", prompt, min_val, max_val);
        ui_reset_color();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        char *endptr;
        double val = strtod(buffer, &endptr);

        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            ui_print_error("Invalid input. Please enter a valid number.");
            continue;
        }

        if (val < min_val || val > max_val) {
            ui_print_error("Value out of bounds. Must be between %.2f and %.2f.", min_val, max_val);
            continue;
        }

        value = val;
        break;
    }
    return value;
}

int ui_prompt_confirm(const char *prompt) {
    char buffer[256];
    while (1) {
        ui_set_color(ANSI_COLOR_YELLOW);
        printf("%s (y/n): ", prompt);
        ui_reset_color();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        char c = buffer[0];
        if (c == 'y' || c == 'Y') {
            return 1;
        } else if (c == 'n' || c == 'N') {
            return 0;
        }

        ui_print_error("Invalid input. Please enter 'y' or 'n'.");
    }
}
