#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

int TILE_WIDTH = 8;
int TILE_HEIGHT = 8;

/* Linked list-like structure for segments in snake */
typedef struct Segment {
    int x, y;
    struct Segment *next;
} Segment;

typedef struct Apple {
    int x, y;
} Apple;

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

void cell_draw(int x, int y);
void snake_clear_tail();
Segment* snake_get_tail();
Segment* snake_head_prepend(int x, int y);
int is_snake(int x, int y);
Apple apple_spawn();

Segment *head;
Apple apple;
int length = 0;
int gameOver = 0;
enum Direction direction = RIGHT;

int main(void) {

    sk_key_t key;
    int draws = 0;

    head = (Segment*)malloc(sizeof(Segment));
    head->x = 0;
    head->y = 0;
    head->next = NULL;

    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(100);

    while(!gameOver) {
        key = os_GetCSC();

        switch(key) {
            case sk_Up:
                if (direction != DOWN) {
                    direction = UP;
                }
                break;

            case sk_Down:
                if (direction != UP) {
                    direction = DOWN;
                }
                break;

            case sk_Left:
                if (direction != RIGHT) {
                    direction = LEFT;
                }
                break;

            case sk_Right:
                if (direction != LEFT) {
                    direction = RIGHT;
                }
                break;

            case sk_Enter:
            case sk_Clear:
                gameOver = 1;
                break;

            default:
                break;
        }

        /* Move the snake every four screen draws */
        if (draws % 4 == 0) {
            head = snake_head_prepend(head->x, head->y);

            /* Increase the snake's length on the first move */
            if (length < 1) {
                apple = apple_spawn();
                length++;
            }
            
            if (head->x == apple.x && head->y == apple.y) {
                length++;
                apple = apple_spawn();
            }

            /* Assign new tail and deallocate memory for previous tail */
            free(snake_get_tail(head, length)->next);
            snake_get_tail(head, length)->next = NULL;

            /* Reset draw counter to prevent an integer overflow */
            draws = 0;
        }

        draws++;

        snake_clear_tail(head, length);
        cell_draw(head->x, head->y);

        gfx_SetColor(200);
        cell_draw(apple.x, apple.y);
        gfx_SetColor(255);

        gfx_SwapDraw();

    }

    gfx_End();
    return 0;
}

void cell_draw(int x, int y) {
    int i, j;

    for (i = x * TILE_WIDTH; i < (x + 1) * TILE_WIDTH; i++) {
        for (j = y * TILE_HEIGHT; j < (y + 1) * TILE_HEIGHT; j++) {
            gfx_SetPixel(i, j);
        }
    }
}

void snake_clear_tail() {
    /* Set color to background color before drawing */
    gfx_SetColor(255);
    cell_draw(snake_get_tail(head, length)->x, snake_get_tail(head, length)->y);
    gfx_SetColor(100);
}

Segment* snake_get_tail() {
    Segment *current = head;
    int i;

    /* Traverse the linked list depending on length */
    for (i = 0; i < length; i++) {
        current = current->next;
    }
    
    return current;
}

Segment* snake_head_prepend(int x, int y) {
    Segment *new_segment = (Segment*)malloc(sizeof(Segment));

    switch (direction) {
        case(UP):
            new_segment->x = x;
            new_segment->y = y - 1;
            break;
        
        case(DOWN):
            new_segment->x = x;
            new_segment->y = y + 1;
            break;

        case(LEFT):
            new_segment->x = x - 1;
            new_segment->y = y;
            break;

        case(RIGHT):
            new_segment->x = x + 1;
            new_segment->y = y;
            break;
    }

    /* If snake is either intersecting or out of bounds */
    if (is_snake(new_segment->x, new_segment->y) ||
            new_segment->x > (LCD_WIDTH / TILE_WIDTH) - 1 ||
            new_segment->y > (LCD_HEIGHT / TILE_HEIGHT) - 1 ||
            new_segment->x < 0 ||
            new_segment->y < 0)
        {

        gameOver = 1;
    }

    new_segment->next = head;

    return new_segment;
}

/* Check if the current coordinate is occupied by the snake */
int is_snake(int x, int y) {
    Segment *current = head;
    int i;

    for (i = 0; i < length - 1; i++) {
        if (current->x == x && current->y == y) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

Apple apple_spawn() {
    Apple apple_new;

    do {
        srand(rtc_Time());

        apple_new.x = randInt(0, (LCD_WIDTH / TILE_WIDTH) - 1);
        apple_new.y = randInt(0, (LCD_HEIGHT / TILE_HEIGHT) - 1);
    } while (is_snake(apple_new.x, apple_new.y));

    return apple_new;
}

