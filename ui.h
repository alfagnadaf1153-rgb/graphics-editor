#ifndef UI_H
#define UI_H

// ANSI Escape Code Constants for Styling
#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR_BLACK   "\033[0;30m"
#define ANSI_COLOR_RED     "\033[1;31m"
#define ANSI_COLOR_GREEN   "\033[1;32m"
#define ANSI_COLOR_YELLOW  "\033[1;33m"
#define ANSI_COLOR_BLUE    "\033[1;34m"
#define ANSI_COLOR_MAGENTA "\033[1;35m"
#define ANSI_COLOR_CYAN    "\033[1;36m"
#define ANSI_COLOR_WHITE   "\033[1;37m"
#define ANSI_COLOR_GRAY    "\033[0;90m"

#define ANSI_BG_BLUE       "\033[44m"
#define ANSI_STYLE_BOLD    "\033[1m"

// UI System Functions
void ui_init(void);
void ui_clear_screen(void);
void ui_set_color(const char *color_ansi);
void ui_reset_color(void);

void ui_print_header(const char *title);
void ui_print_success(const char *format, ...);
void ui_print_error(const char *format, ...);
void ui_print_warning(const char *format, ...);
void ui_print_info(const char *format, ...);

// Input Validation Helpers
int ui_prompt_int(const char *prompt, int min_val, int max_val);
void ui_prompt_string(const char *prompt, char *buffer, int max_len);
double ui_prompt_double(const char *prompt, double min_val, double max_val);
int ui_prompt_confirm(const char *prompt);

#endif // UI_H
