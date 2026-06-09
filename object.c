#include "object.h"
#include "canvas.h"
#include <stdio.h>
#include <string.h>

void object_list_init(ObjectList *ol) {
    ol->count = 0;
    ol->next_id = 1;
}

int object_list_add(ObjectList *ol, GraphicObject obj) {
    if (ol->count >= MAX_OBJECTS) {
        return -1;
    }
    obj.id = ol->next_id++;
    ol->list[ol->count] = obj;
    ol->count++;
    return obj.id;
}

int object_list_find_index(const ObjectList *ol, int id) {
    for (int i = 0; i < ol->count; i++) {
        if (ol->list[i].id == id) {
            return i;
        }
    }
    return -1;
}

int object_list_delete(ObjectList *ol, int id) {
    int idx = object_list_find_index(ol, id);
    if (idx == -1) {
        return 0; // Not found
    }
    // Shift elements left
    for (int i = idx; i < ol->count - 1; i++) {
        ol->list[i] = ol->list[i + 1];
    }
    ol->count--;
    return 1; // Success
}

int object_list_modify(ObjectList *ol, int id, GraphicObject new_obj) {
    int idx = object_list_find_index(ol, id);
    if (idx == -1) {
        return 0; // Not found
    }
    new_obj.id = id; // Retain original ID
    ol->list[idx] = new_obj;
    return 1; // Success
}

void object_list_redraw(const ObjectList *ol, Canvas *canvas) {
    canvas_clear(canvas);
    for (int i = 0; i < ol->count; i++) {
        const GraphicObject *obj = &ol->list[i];
        switch (obj->type) {
            case OBJ_LINE:
                canvas_draw_line(canvas, obj->params.line.x1, obj->params.line.y1, 
                                 obj->params.line.x2, obj->params.line.y2);
                break;
            case OBJ_RECTANGLE:
                canvas_draw_rect(canvas, obj->params.rect.x, obj->params.rect.y, 
                                 obj->params.rect.width, obj->params.rect.height);
                break;
            case OBJ_CIRCLE:
                canvas_draw_circle(canvas, obj->params.circle.xc, obj->params.circle.yc, 
                                   obj->params.circle.radius);
                break;
            case OBJ_TRIANGLE:
                canvas_draw_triangle(canvas, obj->params.triangle.x1, obj->params.triangle.y1, 
                                     obj->params.triangle.x2, obj->params.triangle.y2, 
                                     obj->params.triangle.x3, obj->params.triangle.y3);
                break;
        }
    }
}

void object_describe(const GraphicObject *obj, char *buffer, int buffer_size) {
    switch (obj->type) {
        case OBJ_LINE:
            snprintf(buffer, buffer_size, "Line: (%d, %d) -> (%d, %d)", 
                     obj->params.line.x1, obj->params.line.y1, 
                     obj->params.line.x2, obj->params.line.y2);
            break;
        case OBJ_RECTANGLE:
            snprintf(buffer, buffer_size, "Rectangle: Top-Left (%d, %d), Width %d, Height %d", 
                     obj->params.rect.x, obj->params.rect.y, 
                     obj->params.rect.width, obj->params.rect.height);
            break;
        case OBJ_CIRCLE:
            snprintf(buffer, buffer_size, "Circle: Center (%d, %d), Radius %d", 
                     obj->params.circle.xc, obj->params.circle.yc, 
                     obj->params.circle.radius);
            break;
        case OBJ_TRIANGLE:
            snprintf(buffer, buffer_size, "Triangle: P1(%d, %d), P2(%d, %d), P3(%d, %d)", 
                     obj->params.triangle.x1, obj->params.triangle.y1, 
                     obj->params.triangle.x2, obj->params.triangle.y2, 
                     obj->params.triangle.x3, obj->params.triangle.y3);
            break;
        default:
            snprintf(buffer, buffer_size, "Unknown object");
            break;
    }
}

int object_list_save(const ObjectList *ol, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        return 0; // Failure
    }
    
    // Write count of objects
    fprintf(fp, "%d\n", ol->count);
    
    for (int i = 0; i < ol->count; i++) {
        const GraphicObject *obj = &ol->list[i];
        fprintf(fp, "%d ", obj->type);
        switch (obj->type) {
            case OBJ_LINE:
                fprintf(fp, "%d %d %d %d\n", obj->params.line.x1, obj->params.line.y1, 
                                             obj->params.line.x2, obj->params.line.y2);
                break;
            case OBJ_RECTANGLE:
                fprintf(fp, "%d %d %d %d\n", obj->params.rect.x, obj->params.rect.y, 
                                             obj->params.rect.width, obj->params.rect.height);
                break;
            case OBJ_CIRCLE:
                fprintf(fp, "%d %d %d\n", obj->params.circle.xc, obj->params.circle.yc, 
                                          obj->params.circle.radius);
                break;
            case OBJ_TRIANGLE:
                fprintf(fp, "%d %d %d %d %d %d\n", obj->params.triangle.x1, obj->params.triangle.y1, 
                                                   obj->params.triangle.x2, obj->params.triangle.y2, 
                                                   obj->params.triangle.x3, obj->params.triangle.y3);
                break;
        }
    }
    
    fclose(fp);
    return 1; // Success
}

int object_list_load(ObjectList *ol, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0; // Failure
    }
    
    int load_count = 0;
    if (fscanf(fp, "%d", &load_count) != 1) {
        fclose(fp);
        return 0;
    }
    
    if (load_count < 0 || load_count > MAX_OBJECTS) {
        fclose(fp);
        return 0;
    }
    
    object_list_init(ol);
    
    for (int i = 0; i < load_count; i++) {
        int type_val;
        if (fscanf(fp, "%d", &type_val) != 1) {
            fclose(fp);
            return 0;
        }
        
        GraphicObject obj;
        obj.type = (ObjectType)type_val;
        
        switch (obj.type) {
            case OBJ_LINE:
                if (fscanf(fp, "%d %d %d %d", &obj.params.line.x1, &obj.params.line.y1, 
                                              &obj.params.line.x2, &obj.params.line.y2) != 4) {
                    fclose(fp);
                    return 0;
                }
                break;
            case OBJ_RECTANGLE:
                if (fscanf(fp, "%d %d %d %d", &obj.params.rect.x, &obj.params.rect.y, 
                                              &obj.params.rect.width, &obj.params.rect.height) != 4) {
                    fclose(fp);
                    return 0;
                }
                break;
            case OBJ_CIRCLE:
                if (fscanf(fp, "%d %d %d", &obj.params.circle.xc, &obj.params.circle.yc, 
                                           &obj.params.circle.radius) != 3) {
                    fclose(fp);
                    return 0;
                }
                break;
            case OBJ_TRIANGLE:
                if (fscanf(fp, "%d %d %d %d %d %d", &obj.params.triangle.x1, &obj.params.triangle.y1, 
                                                    &obj.params.triangle.x2, &obj.params.triangle.y2, 
                                                    &obj.params.triangle.x3, &obj.params.triangle.y3) != 6) {
                    fclose(fp);
                    return 0;
                }
                break;
            default:
                // Unknown type, exit with error
                fclose(fp);
                return 0;
        }
        
        object_list_add(ol, obj);
    }
    
    fclose(fp);
    return 1; // Success
}
