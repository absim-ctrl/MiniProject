#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HEIGHT 24
#define WIDTH 80
#define MAXSIZE 100

typedef enum {
    LINE = 1,
    RECTANGLE = 2,
    CIRCLE = 3,
    TRIANGLE = 4
} ShapeType;

typedef struct {
    int cx;
    int cy;
    int radius;
}Circle;

typedef struct {
    int x1, y1, x2, y2, x3, y3;
} Triangle;

typedef struct {
    int x1, y1, x2, y2;
} Rectngle;

typedef struct{
    int x1,x2,y1,y2;
}Line;

typedef struct{
    int id;
    ShapeType type; 
    union {
        Rectngle rectangle;
        Circle circle;
        Triangle triangle;
        Line line;
    } data;
} Shape;

char canvas[HEIGHT][WIDTH];
int shapeCount = 0;
Shape shape[MAXSIZE];

void clearPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = '_'; 
        }
    }
}
void setPixel(int x,int y){
    if(x>=0 && x<WIDTH  && y>=0 && y<HEIGHT){
        canvas[y][x]='*';
    }   
}
void displayPicture(){
    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            printf("%c",canvas[y][x]); 
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
        switch (s.type) {
            case LINE:
                drawLine(s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                break;
            case RECTANGLE:
                drawRectangle(s.data.rectangle.x1, s.data.rectangle.y1, s.data.rectangle.x2, s.data.rectangle.y2);
                break;
            case CIRCLE:
                drawCircle(s.data.circle.cx, s.data.circle.cy, s.data.circle.radius);
                break;
            case TRIANGLE:
                drawTriangle(s.data.triangle.x1, s.data.triangle.y1, s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3);
                break;
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
            newShape.type = (ShapeType)shapeType;

            if (shapeType == LINE) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &newShape.data.line.x1, &newShape.data.line.y1, &newShape.data.line.x2, &newShape.data.line.y2);
            }
            else if (shapeType == RECTANGLE) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &newShape.data.rectangle.x1, &newShape.data.rectangle.y1, &newShape.data.rectangle.x2, &newShape.data.rectangle.y2);
            }
            else if (shapeType == CIRCLE) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &newShape.data.circle.cx, &newShape.data.circle.cy, &newShape.data.circle.radius);
            }
            else if (shapeType == TRIANGLE) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d", &newShape.data.triangle.x1, &newShape.data.triangle.y1, &newShape.data.triangle.x2, &newShape.data.triangle.y2, &newShape.data.triangle.x3, &newShape.data.triangle.y3);
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
            if (s->type == LINE) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &s->data.line.x1, &s->data.line.y1, &s->data.line.x2, &s->data.line.y2);
            }
            else if (s->type == RECTANGLE) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &s->data.rectangle.x1, &s->data.rectangle.y1, &s->data.rectangle.x2, &s->data.rectangle.y2);
            }
            else if (s->type == CIRCLE) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &s->data.circle.cx, &s->data.circle.cy, &s->data.circle.radius);
            }
            else if (s->type == TRIANGLE) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d", &s->data.triangle.x1, &s->data.triangle.y1, &s->data.triangle.x2, &s->data.triangle.y2, &s->data.triangle.x3, &s->data.triangle.y3);
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
                switch (s.type) {
                    case LINE:
                        printf("Line from (%d, %d) to (%d, %d)\n", s.data.line.x1, s.data.line.y1, s.data.line.x2, s.data.line.y2);
                        break;
                    case RECTANGLE:
                        printf("Rectangle from (%d, %d) to (%d, %d)\n", s.data.rectangle.x1, s.data.rectangle.y1, s.data.rectangle.x2, s.data.rectangle.y2);
                        break;
                    case CIRCLE:
                        printf("Circle center (%d, %d) with radius %d\n", s.data.circle.cx, s.data.circle.cy, s.data.circle.radius);
                        break;
                    case TRIANGLE:
                        printf("Triangle vertices (%d, %d), (%d, %d), (%d, %d)\n", s.data.triangle.x1, s.data.triangle.y1, s.data.triangle.x2, s.data.triangle.y2, s.data.triangle.x3, s.data.triangle.y3);
                        break;
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