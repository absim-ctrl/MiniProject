#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HEIGHT 24
#define WIDTH 80
#define MAXSIZE 100

// Flat structure representing any shape
typedef struct {
    int type; // 1: Line, 2: Rectangle, 3: Circle, 4: Triangle
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

char canvas[HEIGHT][WIDTH];
Shape shape[MAXSIZE];
int shapeCount = 0;

void clearPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = '_'; 
        }
    }
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = '*';
    }   
}

void displayPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", canvas[y][x]); 
        }
        printf("\n");
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    if (steps == 0) {
        setPixel(x1, y1);
        return;
    }
    
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = x1;
    float y = y1;
    
    for (int i = 0; i <= steps; i++) {
        setPixel((int)round(x), (int)round(y));
        x += xInc;
        y += yInc;
    }
}

void drawCircle(int cx, int cy, int radius) {
    float aspect_ratio = 2.0;
    int x_radius = (int)(radius * aspect_ratio);

    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - x_radius; x <= cx + x_radius; x++) {
            float dx = (x - cx) / aspect_ratio;
            float dy = y - cy;
            
            float distanceSquared = dx * dx + dy * dy;
            float radiusSquared = radius * radius;
            
            if (fabs(distanceSquared - radiusSquared) <= radius) {
                setPixel(x, y);
            }
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1); 
    drawLine(x2, y1, x2, y2);
    drawLine(x2, y2, x1, y2); 
    drawLine(x1, y2, x1, y1);
}

void renderShapes() {
    clearPicture();
    for (int i = 0; i < shapeCount; i++) {
        Shape s = shape[i];
        if (s.type == 1) {
            drawLine(s.x1, s.y1, s.x2, s.y2);
        } else if (s.type == 2) {
            drawRectangle(s.x1, s.y1, s.x2, s.y2);
        } else if (s.type == 3) {
            drawCircle(s.x1, s.y1, s.radius);
        } else if (s.type == 4) {
            drawTriangle(s.x1, s.y1, s.x2, s.y2, s.x3, s.y3);
        }
    }
}

int main() {
    int choice;
    int shapeType;

    while (1) {
        printf("2D Graphics Editor\n");
        printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
        printf("1. Add object\n");
        printf("2. Delete object\n");
        printf("3. Modify object\n");
        printf("4. Display picture\n");
        printf("5. List objects\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Goodbye.\n");
            break;
        }

        if (choice == 1) {
            if (shapeCount >= MAXSIZE) {
                printf("Max capacity reached.\n\n");
                continue;
            }
            printf("Choose shape type:\n");
            printf("1. Line\n");
            printf("2. Rectangle\n");
            printf("3. Circle\n");
            printf("4. Triangle\n");
            printf("Enter shape type: ");
            scanf("%d", &shapeType);

            Shape newShape;
            newShape.type = shapeType;

            if (shapeType == 1) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &newShape.x1, &newShape.y1, &newShape.x2, &newShape.y2);
            }
            else if (shapeType == 2) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &newShape.x1, &newShape.y1, &newShape.x2, &newShape.y2);
            }
            else if (shapeType == 3) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &newShape.x1, &newShape.y1, &newShape.radius);
            }
            else if (shapeType == 4) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d", &newShape.x1, &newShape.y1, &newShape.x2, &newShape.y2, &newShape.x3, &newShape.y3);
            }
            else {
                printf("Invalid shape type.\n\n");
                continue;
            }

            shape[shapeCount] = newShape;
            printf("Object added with index %d.\n\n", shapeCount);
            shapeCount++;
        }
        else if (choice == 2) {
            if (shapeCount == 0) {
                printf("No objects to delete.\n\n");
                continue;
            }
            int index;
            printf("Enter object index to delete: ");
            scanf("%d", &index);
            if (index < 0 || index >= shapeCount) {
                printf("Invalid index.\n\n");
                continue;
            }
            for (int i = index; i < shapeCount - 1; i++) {
                shape[i] = shape[i + 1];
            }
            shapeCount--;
            printf("Object deleted.\n\n");
        }
        else if (choice == 3) {
            if (shapeCount == 0) {
                printf("No objects to modify.\n\n");
                continue;
            }
            int index;
            printf("Enter object index to modify: ");
            scanf("%d", &index);
            if (index < 0 || index >= shapeCount) {
                printf("Invalid index.\n\n");
                continue;
            }
            
            Shape *s = &shape[index];
            if (s->type == 1) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2);
            }
            else if (s->type == 2) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2);
            }
            else if (s->type == 3) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &s->x1, &s->y1, &s->radius);
            }
            else if (s->type == 4) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d", &s->x1, &s->y1, &s->x2, &s->y2, &s->x3, &s->y3);
            }
            printf("Object modified.\n\n");
        }
        else if (choice == 4) {
            renderShapes();
            displayPicture();
            printf("\n");
        }
        else if (choice == 5) {
            if (shapeCount == 0) {
                printf("No objects.\n\n");
                continue;
            }
            printf("Objects list:\n");
            for (int i = 0; i < shapeCount; i++) {
                Shape s = shape[i];
                printf("Index %d: ", i);
                if (s.type == 1) {
                    printf("Line from (%d, %d) to (%d, %d)\n", s.x1, s.y1, s.x2, s.y2);
                } else if (s.type == 2) {
                    printf("Rectangle from (%d, %d) to (%d, %d)\n", s.x1, s.y1, s.x2, s.y2);
                } else if (s.type == 3) {
                    printf("Circle center (%d, %d) with radius %d\n", s.x1, s.y1, s.radius);
                } else if (s.type == 4) {
                    printf("Triangle vertices (%d, %d), (%d, %d), (%d, %d)\n", s.x1, s.y1, s.x2, s.y2, s.x3, s.y3);
                }
            }
            printf("\n");
        }
        else if (choice == 0) {
            printf("Goodbye.\n");
            break;
        }
        else {
            printf("Invalid choice.\n\n");
        }
    }
    return 0;
}