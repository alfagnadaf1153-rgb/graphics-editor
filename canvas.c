#include "canvas.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

void canvas_clear(Canvas *canvas) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            canvas->grid[y][x] = '_';
        }
    }
}

void canvas_set_pixel(Canvas *canvas, int x, int y, char ch) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas->grid[y][x] = ch;
    }
}

void canvas_draw_line(Canvas *canvas, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        canvas_set_pixel(canvas, x1, y1, '*');
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void canvas_draw_rect(Canvas *canvas, int x, int y, int width, int height) {
    if (width <= 0 || height <= 0) return;
    
    // Top and bottom borders
    for (int i = 0; i < width; i++) {
        canvas_set_pixel(canvas, x + i, y, '*');
        canvas_set_pixel(canvas, x + i, y + height - 1, '*');
    }
    // Left and right borders
    for (int i = 0; i < height; i++) {
        canvas_set_pixel(canvas, x, y + i, '*');
        canvas_set_pixel(canvas, x + width - 1, y + i, '*');
    }
}

void canvas_draw_circle(Canvas *canvas, int xc, int yc, int radius) {
    if (radius < 0) return;
    if (radius == 0) {
        canvas_set_pixel(canvas, xc, yc, '*');
        return;
    }

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    // Set first 8 points
    canvas_set_pixel(canvas, xc + x, yc + y, '*');
    canvas_set_pixel(canvas, xc - x, yc + y, '*');
    canvas_set_pixel(canvas, xc + x, yc - y, '*');
    canvas_set_pixel(canvas, xc - x, yc - y, '*');
    canvas_set_pixel(canvas, xc + y, yc + x, '*');
    canvas_set_pixel(canvas, xc - y, yc + x, '*');
    canvas_set_pixel(canvas, xc + y, yc - x, '*');
    canvas_set_pixel(canvas, xc - y, yc - x, '*');

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        canvas_set_pixel(canvas, xc + x, yc + y, '*');
        canvas_set_pixel(canvas, xc - x, yc + y, '*');
        canvas_set_pixel(canvas, xc + x, yc - y, '*');
        canvas_set_pixel(canvas, xc - x, yc - y, '*');
        canvas_set_pixel(canvas, xc + y, yc + x, '*');
        canvas_set_pixel(canvas, xc - y, yc + x, '*');
        canvas_set_pixel(canvas, xc + y, yc - x, '*');
        canvas_set_pixel(canvas, xc - y, yc - x, '*');
    }
}

void canvas_draw_triangle(Canvas *canvas, int x1, int y1, int x2, int y2, int x3, int y3) {
    canvas_draw_line(canvas, x1, y1, x2, y2);
    canvas_draw_line(canvas, x2, y2, x3, y3);
    canvas_draw_line(canvas, x3, y3, x1, y1);
}

void canvas_display(const Canvas *canvas) {
    // Print column guide: tens digits, then units digits
    ui_set_color(ANSI_COLOR_GRAY);
    printf("     ");
    for (int x = 0; x < COLS; x++) {
        if (x % 10 == 0) {
            printf("%d", (x / 10) % 10);
        } else {
            printf(" ");
        }
    }
    printf("\n     ");
    for (int x = 0; x < COLS; x++) {
        printf("%d", x % 10);
    }
    printf("\n");

    // Print top border
    printf("    +");
    for (int x = 0; x < COLS; x++) {
        printf("-");
    }
    printf("+\n");
    ui_reset_color();

    // Print grid with row indices and side borders
    for (int y = 0; y < ROWS; y++) {
        ui_set_color(ANSI_COLOR_GRAY);
        printf("%2d  |", y);
        ui_reset_color();

        for (int x = 0; x < COLS; x++) {
            char ch = canvas->grid[y][x];
            if (ch == '*') {
                ui_set_color(ANSI_COLOR_GREEN); // Highlight drawn objects in green
                printf("%c", ch);
                ui_reset_color();
            } else {
                printf("%c", ch);
            }
        }

        ui_set_color(ANSI_COLOR_GRAY);
        printf("|\n");
        ui_reset_color();
    }

    // Print bottom border
    ui_set_color(ANSI_COLOR_GRAY);
    printf("    +");
    for (int x = 0; x < COLS; x++) {
        printf("-");
    }
    printf("+\n");
    ui_reset_color();
}
