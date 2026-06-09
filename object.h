#ifndef OBJECT_H
#define OBJECT_H

#include "canvas.h"

#define MAX_OBJECTS 100

typedef enum {
    OBJ_LINE = 1,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_TRIANGLE
} ObjectType;

typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;

typedef struct {
    int x, y;
    int width, height;
} RectParams;

typedef struct {
    int xc, yc;
    int radius;
} CircleParams;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleParams;

typedef struct {
    int id;
    ObjectType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } params;
} GraphicObject;

typedef struct {
    GraphicObject list[MAX_OBJECTS];
    int count;
    int next_id;
} ObjectList;

void object_list_init(ObjectList *ol);
int object_list_add(ObjectList *ol, GraphicObject obj);
int object_list_delete(ObjectList *ol, int id);
int object_list_modify(ObjectList *ol, int id, GraphicObject new_obj);
int object_list_find_index(const ObjectList *ol, int id);
void object_list_redraw(const ObjectList *ol, Canvas *canvas);
void object_describe(const GraphicObject *obj, char *buffer, int buffer_size);

// File serialization
int object_list_save(const ObjectList *ol, const char *filename);
int object_list_load(ObjectList *ol, const char *filename);

#endif // OBJECT_H
