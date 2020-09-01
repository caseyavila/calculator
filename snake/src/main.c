#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include <debug.h>

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
void snake_clear_tail(Segment *head, int length);
Segment* snake_get_tail(Segment *head, int length);
Segment* snake_head_prepend(int x, int y, enum Direction direction, Segment *head);
Apple apple_spawn();

void print_list(Segment *head) {
    Segment *current = head;
    
    dbg_sprintf(dbgout, "------\n");
    while (current != NULL) {
        dbg_sprintf(dbgout, "(%d, %d)\n", current->x, current->y);
        current = current->next;
    }
}

int main(void) {

    sk_key_t key;
    Segment *head = (Segment*)malloc(sizeof(Segment));
    Apple apple;
    int length = 0;
    int draws = 0;
    enum Direction direction = RIGHT;

    head->x = 0;
    head->y = 0;
    head->next = NULL;


    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(0);

    do {
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

            case sk_Prgm:
                dbg_sprintf(dbgout, "x:%d, y:%d, length:%d\n", snake_get_tail(head, length)->x, snake_get_tail(head, length)->y, length);
                break;

            case sk_8:
                print_list(head);
                break;

            default:
                break;
        }

        /* Move the snake every five screen draws */
        if (draws % 4 == 0) {
            head = snake_head_prepend(head->x, head->y, direction, head);

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
        cell_draw(apple.x, apple.y);

        gfx_SwapDraw();

    } while (key != sk_Enter);

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

void snake_clear_tail(Segment *head, int length) {
    int i, j;
    Segment *tail;

    tail = snake_get_tail(head, length);

    /* Set color to white before drawing, then revert */
    gfx_SetColor(255);
    for (i = tail->x * TILE_WIDTH; i < (tail->x + 1) * TILE_WIDTH; i++) {
        for (j = tail->y * TILE_HEIGHT; j < (tail->y + 1) * TILE_HEIGHT; j++) {
            gfx_SetPixel(i, j);
        }
    }
    gfx_SetColor(0);
}

Segment* snake_get_tail(Segment *head, int length) {
    Segment *current = head;
    int i;

    /* Traverse the linked list depending on length provided */
    for (i = 0; i < length; i++) {
        current = current->next;
    }
    
    return current;
}

Segment* snake_head_prepend(int x, int y, enum Direction direction, Segment *head) {
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

    new_segment->next = head;

    return new_segment;
}

Apple apple_spawn() {
    Apple apple_new;

    srand(rtc_Time());

    apple_new.x = randInt(0, (LCD_WIDTH / TILE_WIDTH) - 1);
    apple_new.y = randInt(0, (LCD_HEIGHT / TILE_HEIGHT) - 1);

    return apple_new;
}
