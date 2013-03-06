#import "stdlib.h"
#import "stdio.h"

#define DIR_N 0
#define DIR_S 1
#define DIR_E 2
#define DIR_W 3

typedef struct maze_point_t_ {
  int x;
  int y;
  struct maze_point_t_ *prev;
  struct maze_point_t_ *next;
} maze_point_t;

typedef struct maze_t_ {
  int width;
  int height;
  char wall;
  char path;
  int *array;
  int step;
  struct maze_point_t_ *root_point;
  struct maze_point_t_ *last_point;
} maze_t;

void arr_shuffle(int *array, size_t n);
int maze_index_for(maze_t *maze, int x, int y);
int maze_value_for(maze_t *maze, int x, int y);
void maze_set_value_for(maze_t *maze, int x, int y, int v);
void maze_destroy_point(maze_point_t *point);
maze_point_t *maze_create_point(int x, int y, maze_point_t *prev);
maze_t *maze_create(int width, int height, char wall, char path, int x, int y);
void maze_destroy(maze_t *maze);
void maze_next(maze_t *maze);
int maze_move_to_dir(maze_t *maze, int dir);
void maze_set(maze_t *maze, int v);
void maze_print(maze_t *maze);
void maze_draw(maze_t *maze);
void maze_move_back(maze_t *maze);