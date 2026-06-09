#ifndef CANVAS_H
#define CANVAS_H

#define ROWS 24
#define COLS 80

typedef struct {
    char grid[ROWS][COLS];
} Canvas;

void canvas_clear(Canvas *canvas);
void canvas_set_pixel(Canvas *canvas, int x, int y, char ch);
void canvas_draw_line(Canvas *canvas, int x1, int y1, int x2, int y2);
void canvas_draw_rect(Canvas *canvas, int x, int y, int width, int height);
void canvas_draw_circle(Canvas *canvas, int xc, int yc, int radius);
void canvas_draw_triangle(Canvas *canvas, int x1, int y1, int x2, int y2, int x3, int y3);
void canvas_display(const Canvas *canvas);

#endif // CANVAS_H
