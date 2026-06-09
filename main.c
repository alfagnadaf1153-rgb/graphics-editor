#include "ui.h"
#include "canvas.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_active_shapes(const ObjectList *ol) {
    if (ol->count == 0) {
        ui_print_info("No shapes drawn yet. Select 'Add Graphical Object' to start drawing!");
        return;
    }
    
    ui_set_color(ANSI_COLOR_CYAN);
    printf("--- Active Shapes list ---\n");
    ui_reset_color();
    
    char desc[128];
    for (int i = 0; i < ol->count; i++) {
        object_describe(&ol->list[i], desc, sizeof(desc));
        printf("  [ID: %2d] %s\n", ol->list[i].id, desc);
    }
    printf("\n");
}

void handle_add_object(ObjectList *ol, Canvas *canvas) {
    ui_clear_screen();
    ui_print_header("ADD GRAPHICAL OBJECT");
    
    printf("Select shape type:\n");
    printf("  1. Line\n");
    printf("  2. Rectangle\n");
    printf("  3. Circle\n");
    printf("  4. Triangle\n");
    printf("  5. Back to Main Menu\n\n");
    
    int type_choice = ui_prompt_int("Enter selection", 1, 5);
    if (type_choice == 5) return;
    
    GraphicObject obj;
    obj.type = (ObjectType)type_choice;
    
    switch (obj.type) {
        case OBJ_LINE:
            printf("\n--- Draw a Line ---\n");
            obj.params.line.x1 = ui_prompt_int("Start X", 0, COLS - 1);
            obj.params.line.y1 = ui_prompt_int("Start Y", 0, ROWS - 1);
            obj.params.line.x2 = ui_prompt_int("End X", 0, COLS - 1);
            obj.params.line.y2 = ui_prompt_int("End Y", 0, ROWS - 1);
            break;
            
        case OBJ_RECTANGLE:
            printf("\n--- Draw a Rectangle ---\n");
            obj.params.rect.x = ui_prompt_int("Top-Left X", 0, COLS - 1);
            obj.params.rect.y = ui_prompt_int("Top-Left Y", 0, ROWS - 1);
            obj.params.rect.width = ui_prompt_int("Width", 1, COLS - obj.params.rect.x);
            obj.params.rect.height = ui_prompt_int("Height", 1, ROWS - obj.params.rect.y);
            break;
            
        case OBJ_CIRCLE:
            printf("\n--- Draw a Circle ---\n");
            obj.params.circle.xc = ui_prompt_int("Center X", 0, COLS - 1);
            obj.params.circle.yc = ui_prompt_int("Center Y", 0, ROWS - 1);
            
            // Calculate max radius to stay inside canvas limits
            int max_r = obj.params.circle.xc;
            if (COLS - 1 - obj.params.circle.xc < max_r) max_r = COLS - 1 - obj.params.circle.xc;
            if (obj.params.circle.yc < max_r) max_r = obj.params.circle.yc;
            if (ROWS - 1 - obj.params.circle.yc < max_r) max_r = ROWS - 1 - obj.params.circle.yc;
            
            if (max_r < 0) max_r = 0;
            
            printf("Recommended max radius to stay fully inside screen: %d\n", max_r);
            obj.params.circle.radius = ui_prompt_int("Radius", 0, 40);
            break;
            
        case OBJ_TRIANGLE:
            printf("\n--- Draw a Triangle ---\n");
            obj.params.triangle.x1 = ui_prompt_int("Vertex 1 X", 0, COLS - 1);
            obj.params.triangle.y1 = ui_prompt_int("Vertex 1 Y", 0, ROWS - 1);
            obj.params.triangle.x2 = ui_prompt_int("Vertex 2 X", 0, COLS - 1);
            obj.params.triangle.y2 = ui_prompt_int("Vertex 2 Y", 0, ROWS - 1);
            obj.params.triangle.x3 = ui_prompt_int("Vertex 3 X", 0, COLS - 1);
            obj.params.triangle.y3 = ui_prompt_int("Vertex 3 Y", 0, ROWS - 1);
            break;
    }
    
    int added_id = object_list_add(ol, obj);
    if (added_id != -1) {
        object_list_redraw(ol, canvas);
        ui_print_success("Shape added successfully with ID: %d!", added_id);
    } else {
        ui_print_error("Failed to add shape. Maximum capacity reached.");
    }
    
    printf("\nPress Enter to return to main menu...");
    getchar();
}

void handle_delete_object(ObjectList *ol, Canvas *canvas) {
    ui_clear_screen();
    ui_print_header("DELETE GRAPHICAL OBJECT");
    
    if (ol->count == 0) {
        ui_print_warning("No shapes are available to delete.");
        printf("\nPress Enter to return to main menu...");
        getchar();
        return;
    }
    
    show_active_shapes(ol);
    
    int id = ui_prompt_int("Enter the ID of the shape to delete (0 to cancel)", 0, ol->next_id);
    if (id == 0) return;
    
    if (object_list_delete(ol, id)) {
        object_list_redraw(ol, canvas);
        ui_print_success("Shape with ID %d has been deleted.", id);
    } else {
        ui_print_error("Shape with ID %d not found.", id);
    }
    
    printf("\nPress Enter to return to main menu...");
    getchar();
}

void handle_modify_object(ObjectList *ol, Canvas *canvas) {
    ui_clear_screen();
    ui_print_header("MODIFY GRAPHICAL OBJECT");
    
    if (ol->count == 0) {
        ui_print_warning("No shapes are available to modify.");
        printf("\nPress Enter to return to main menu...");
        getchar();
        return;
    }
    
    show_active_shapes(ol);
    
    int id = ui_prompt_int("Enter the ID of the shape to modify (0 to cancel)", 0, ol->next_id);
    if (id == 0) return;
    
    int idx = object_list_find_index(ol, id);
    if (idx == -1) {
        ui_print_error("Shape with ID %d not found.", id);
        printf("\nPress Enter to return to main menu...");
        getchar();
        return;
    }
    
    GraphicObject current_obj = ol->list[idx];
    char desc[128];
    object_describe(&current_obj, desc, sizeof(desc));
    ui_print_info("Modifying shape: %s", desc);
    
    GraphicObject new_obj;
    new_obj.type = current_obj.type;
    
    switch (new_obj.type) {
        case OBJ_LINE:
            new_obj.params.line.x1 = ui_prompt_int("New Start X", 0, COLS - 1);
            new_obj.params.line.y1 = ui_prompt_int("New Start Y", 0, ROWS - 1);
            new_obj.params.line.x2 = ui_prompt_int("New End X", 0, COLS - 1);
            new_obj.params.line.y2 = ui_prompt_int("New End Y", 0, ROWS - 1);
            break;
            
        case OBJ_RECTANGLE:
            new_obj.params.rect.x = ui_prompt_int("New Top-Left X", 0, COLS - 1);
            new_obj.params.rect.y = ui_prompt_int("New Top-Left Y", 0, ROWS - 1);
            new_obj.params.rect.width = ui_prompt_int("New Width", 1, COLS - new_obj.params.rect.x);
            new_obj.params.rect.height = ui_prompt_int("New Height", 1, ROWS - new_obj.params.rect.y);
            break;
            
        case OBJ_CIRCLE:
            new_obj.params.circle.xc = ui_prompt_int("New Center X", 0, COLS - 1);
            new_obj.params.circle.yc = ui_prompt_int("New Center Y", 0, ROWS - 1);
            new_obj.params.circle.radius = ui_prompt_int("New Radius", 0, 40);
            break;
            
        case OBJ_TRIANGLE:
            new_obj.params.triangle.x1 = ui_prompt_int("New Vertex 1 X", 0, COLS - 1);
            new_obj.params.triangle.y1 = ui_prompt_int("New Vertex 1 Y", 0, ROWS - 1);
            new_obj.params.triangle.x2 = ui_prompt_int("New Vertex 2 X", 0, COLS - 1);
            new_obj.params.triangle.y2 = ui_prompt_int("New Vertex 2 Y", 0, ROWS - 1);
            new_obj.params.triangle.x3 = ui_prompt_int("New Vertex 3 X", 0, COLS - 1);
            new_obj.params.triangle.y3 = ui_prompt_int("New Vertex 3 Y", 0, ROWS - 1);
            break;
    }
    
    if (object_list_modify(ol, id, new_obj)) {
        object_list_redraw(ol, canvas);
        ui_print_success("Shape with ID %d updated successfully.", id);
    } else {
        ui_print_error("Failed to update shape with ID %d.", id);
    }
    
    printf("\nPress Enter to return to main menu...");
    getchar();
}

void handle_save_file(const ObjectList *ol) {
    ui_clear_screen();
    ui_print_header("SAVE CANVAS DRAWING TO FILE");
    
    char filename[128];
    ui_prompt_string("Enter filename to save (e.g. drawing.txt)", filename, sizeof(filename));
    
    if (object_list_save(ol, filename)) {
        ui_print_success("Drawing saved successfully to '%s'.", filename);
    } else {
        ui_print_error("Failed to save drawing to '%s'. Make sure folder paths exist.", filename);
    }
    
    printf("\nPress Enter to return to main menu...");
    getchar();
}

void handle_load_file(ObjectList *ol, Canvas *canvas) {
    ui_clear_screen();
    ui_print_header("LOAD CANVAS DRAWING FROM FILE");
    
    char filename[128];
    ui_prompt_string("Enter filename to load (e.g. drawing.txt)", filename, sizeof(filename));
    
    if (object_list_load(ol, filename)) {
        object_list_redraw(ol, canvas);
        ui_print_success("Drawing loaded successfully from '%s'.", filename);
    } else {
        ui_print_error("Failed to load drawing from '%s'. File might not exist or is corrupted.", filename);
    }
    
    printf("\nPress Enter to return to main menu...");
    getchar();
}

int main(void) {
    ui_init();
    
    Canvas canvas;
    ObjectList ol;
    
    canvas_clear(&canvas);
    object_list_init(&ol);
    
    int running = 1;
    while (running) {
        ui_clear_screen();
        ui_print_header("2D VECTOR GRAPHICS CANVAS EDITOR");
        
        // Render Canvas
        canvas_display(&canvas);
        printf("\n");
        
        // Show Active Shapes Summary
        show_active_shapes(&ol);
        
        // Menu Options
        printf("--- Main Menu Choices ---\n");
        printf("  1. Add Graphical Object (Line, Rect, Circle, Triangle)\n");
        printf("  2. Delete Graphical Object\n");
        printf("  3. Modify Graphical Object\n");
        printf("  4. Clear Canvas (Remove All Objects)\n");
        printf("  5. Save Canvas to Text File\n");
        printf("  6. Load Canvas from Text File\n");
        printf("  7. Exit Application\n\n");
        
        int choice = ui_prompt_int("Enter menu choice", 1, 7);
        switch (choice) {
            case 1:
                handle_add_object(&ol, &canvas);
                break;
            case 2:
                handle_delete_object(&ol, &canvas);
                break;
            case 3:
                handle_modify_object(&ol, &canvas);
                break;
            case 4:
                if (ui_prompt_confirm("Are you sure you want to clear all objects from the canvas?")) {
                    object_list_init(&ol);
                    canvas_clear(&canvas);
                    ui_print_success("Canvas cleared.");
                }
                break;
            case 5:
                handle_save_file(&ol);
                break;
            case 6:
                handle_load_file(&ol, &canvas);
                break;
            case 7:
                if (ui_prompt_confirm("Are you sure you want to exit?")) {
                    running = 0;
                }
                break;
        }
    }
    
    ui_clear_screen();
    ui_print_header("THANK YOU FOR USING THE GRAPHICS CANVAS EDITOR");
    printf("Goodbye!\n\n");
    return 0;
}
