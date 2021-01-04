#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define SNAKE_COLOR 102
#define APPLE_COLOR 224

/* Linked list-like structure for segments in snake */
typedef struct Segment {
    int x, y;
    struct Segment *next;
} Segment;

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

typedef struct Apple {
    int x, y;
} Apple;

void cell_draw(int x, int y);
int snake_prepend(Segment **snake, enum Direction direction, Apple apple);
void snake_remove(Segment *snake);
int is_snake(Segment *snake, int x, int y);
Apple apple_spawn(Segment *snake);

int main(void) {
    int game_over = 0;
    sk_key_t key;
    Segment *snake = (Segment *) malloc(sizeof(Segment));
    enum Direction direction = RIGHT;

    snake->x = 0;
    snake->y = 0;
    snake->next = NULL;

    srand(rtc_Time());

    Apple apple = apple_spawn(snake);

    gfx_Begin();
    gfx_ZeroScreen();
    gfx_SetColor(SNAKE_COLOR);

    while (!game_over) {
        delay(45);
        key = os_GetCSC();

        switch (key) {
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
                game_over = 1;
                break;
        }

        switch (snake_prepend(&snake, direction, apple)) {
            case 0:
                snake_remove(snake);
                break;
            case 1:
                game_over = 1;
                break;
            case 2:
                apple = apple_spawn(snake);
                break;
        }

        cell_draw(snake->x, snake->y);
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

int snake_prepend(Segment **snake, enum Direction direction, Apple apple) {
    Segment *new_segment = (Segment *) malloc(sizeof(Segment));

    switch (direction) {
        case UP:
            new_segment->x = (*snake)->x;
            new_segment->y = (*snake)->y - 1;
            break;

        case DOWN:
            new_segment->x = (*snake)->x;
            new_segment->y = (*snake)->y + 1;
            break;

        case LEFT:
            new_segment->x = (*snake)->x - 1;
            new_segment->y = (*snake)->y;
            break;

        case RIGHT:
            new_segment->x = (*snake)->x + 1;
            new_segment->y = (*snake)->y;
            break;
    }

    if (is_snake(*snake, new_segment->x, new_segment->y) ||
            new_segment->x > (LCD_WIDTH / TILE_WIDTH) - 1 ||
            new_segment->y > (LCD_HEIGHT / TILE_HEIGHT) - 1 ||
            new_segment->x < 0 ||
            new_segment->y < 0)
        {
        return 1;
    }

    new_segment->next = *snake;
    *snake = new_segment;

    if (new_segment->x == apple.x && new_segment->y == apple.y) {
        return 2;
    }

    return 0;
}

void snake_remove(Segment *snake) {
    Segment *pointer = snake;

    while (pointer->next->next != NULL) {
        pointer = pointer->next;
    }

    gfx_SetColor(0);
    cell_draw(pointer->next->x, pointer->next->y);
    gfx_SetColor(SNAKE_COLOR);

    free(pointer->next);
    pointer->next = NULL;
}

int is_snake(Segment *snake, int x, int y) {
    Segment *pointer = snake;

    while (pointer->next != NULL) {
        if (pointer->x == x && pointer->y == y) {
            return 1;
        }

        pointer = pointer->next;
    }

    return 0;
}

Apple apple_spawn(Segment *snake) {
    Apple apple;

    do {
        apple.x = randInt(0, (LCD_WIDTH / TILE_WIDTH) - 1);
        apple.y = randInt(0, (LCD_HEIGHT / TILE_HEIGHT) - 1);
    } while (is_snake(snake, apple.x, apple.y));

    gfx_SetColor(APPLE_COLOR);
    cell_draw(apple.x, apple.y);
    gfx_SetColor(SNAKE_COLOR);

    return apple;
}
