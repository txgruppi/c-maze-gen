#import "stdlib.h"
#import "stdio.h"
#import "string.h"
#import "time.h"
#import "maze.h"

int *dir;

void arr_shuffle(int *array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = rand() % n;
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

int maze_index_for(maze_t *maze, int x, int y) {
  int idx = (x * maze->height) + y;
  if (idx > (maze->width * maze->height) - 1)
    idx = (maze->width * maze->height) - 1;
  return idx;
}

int maze_value_for(maze_t *maze, int x, int y) {
  return maze->array[maze_index_for(maze, x, y)];
}

void maze_set_value_for(maze_t *maze, int x, int y, int v) {
  maze->array[maze_index_for(maze, x, y)] = v;
}

void maze_destroy_point(maze_point_t *point) {
  if (point->next != NULL)
    maze_destroy_point(point->next);
  if (point->prev)
    point->prev->next = NULL;
  free(point);
}

maze_point_t *maze_create_point(int x, int y, maze_point_t *prev) {
  maze_point_t *point = malloc(sizeof(maze_point_t));
  point->x = x;
  point->y = y;
  point->next = NULL;
  point->prev = NULL;
  if (prev != NULL) {
    point->prev = prev;
    prev->next = point;
  }
  return point;
}

maze_t *maze_create(int width, int height, char wall, char path, int x, int y) {
  if (width % 2 == 0)
    width++;
  if (height % 2 == 0)
    height++;
  maze_t *maze = malloc(sizeof(maze_t));
  maze->width = width;
  maze->height = height;
  maze->wall = wall;
  maze->path = path;
  maze->step = 0;
  maze->array = malloc(sizeof(int) * maze->width * maze->height);
  maze->root_point = maze_create_point(x, y, NULL);
  maze->last_point = maze->root_point;
  return maze;
}

void maze_destroy(maze_t *maze) {
  maze_destroy_point(maze->root_point);
  free(maze->array);
  free(maze);
}

void maze_generate(maze_t *maze) {
  while (1) {
    arr_shuffle(dir, 4);
    int move = maze_move_to_dir(maze, dir[0]) || maze_move_to_dir(maze, dir[1]) || maze_move_to_dir(maze, dir[2]) || maze_move_to_dir(maze, dir[3]);
    if (move == 0)
      maze_move_back(maze);
    else if (maze->step > 0) {
      printf("\n");
      maze_draw(maze);
      usleep(maze->step);
    }
    if (maze->root_point == maze->last_point)
      break;
  }
}

int maze_move_to_dir(maze_t *maze, int dir) {
  maze_point_t *point = maze->last_point;
  if (dir == DIR_N && point->y > 2 && maze_value_for(maze, point->x, point->y - 2) == 1) {
    maze_set_value_for(maze, point->x, point->y - 1, 0);
    maze_set_value_for(maze, point->x, point->y - 2, 0);
    maze->last_point = maze_create_point(point->x, point->y - 2, point);
    return 1;
  } else if (dir == DIR_S && point->y < maze->height - 2 && maze_value_for(maze, point->x, point->y + 2) == 1) {
    maze_set_value_for(maze, point->x, point->y + 1, 0);
    maze_set_value_for(maze, point->x, point->y + 2, 0);
    maze->last_point = maze_create_point(point->x, point->y + 2, point);
    return 1;
  } else if (dir == DIR_E && point->x < maze->width - 2 && maze_value_for(maze, point->x + 2, point->y) == 1) {
    maze_set_value_for(maze, point->x + 1, point->y, 0);
    maze_set_value_for(maze, point->x + 2, point->y, 0);
    maze->last_point = maze_create_point(point->x + 2, point->y, point);
    return 1;
  } else if (dir == DIR_W && point->x > 2 && maze_value_for(maze, point->x - 2, point->y) == 1) {
    maze_set_value_for(maze, point->x - 1, point->y, 0);
    maze_set_value_for(maze, point->x - 2, point->y, 0);
    maze->last_point = maze_create_point(point->x - 2, point->y, point);
    return 1;
  }
  return 0;
}

void maze_set(maze_t *maze, int v) {
  int x, y;
  for (y = 0; y < maze->height; y++) {
    for (x = 0; x < maze->width; x++) {
      maze->array[maze_index_for(maze, x, y)] = v;
    }
  }
}

void maze_draw(maze_t *maze) {
  int x, y;
  for (y = 0; y < maze->height; y++) {
    for (x = 0; x < maze->width; x++) {
      printf("%c", maze_value_for(maze, x, y) == 1 ? maze->wall : maze->path);
    }
    printf("\n");
  }
}

void maze_print(maze_t *maze) {
  int x, y;
  for (y = 0; y < maze->height; y++) {
    for (x = 0; x < maze->width; x++) {
      printf("%d", maze_value_for(maze, x, y));
    }
  }
  printf("\n");
}

void maze_move_back(maze_t *maze) {
  if (maze->last_point != NULL && maze->root_point != maze->last_point) {
    maze_point_t *temp = maze->last_point;
    maze->last_point = temp->prev;
    maze_destroy_point(temp);
  }
}

int main(int argc, char **argv) {
  int step = 0;
  int print = 0;
  int draw = 0;
  int width = 0;
  int height = 0;
  int help = 0;
  char wall = '#';
  char path = ' ';
  int i;
  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0) {
      step = 60000;
    } else if (strcmp(argv[i], "-ss") == 0) {
      step = 60000 * 2;
    } else if (strcmp(argv[i], "-sss") == 0) {
      step = 60000 * 3;
    } else if (strcmp(argv[i], "-ssss") == 0) {
      step = 60000 * 4;
    } else if (strcmp(argv[i], "-sssss") == 0) {
      step = 60000 * 5;
    } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--draw") == 0) {
      draw = 1;
    } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--print") == 0) {
      print = 1;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      help = 1;
    } else if (strcmp(argv[i], "-W") == 0 || strcmp(argv[i], "--wall") == 0) {
      if (i+1 < argc) {
        wall = *argv[++i];
      } else {
        fprintf(stderr, "Excepted wall char");
      }
    } else if (strcmp(argv[i], "-P") == 0 || strcmp(argv[i], "--path") == 0) {
      if (i+1 < argc) {
        path = *argv[++i];
      } else {
        fprintf(stderr, "Excepted path char");
      }
    } else if (i > 0) {
      width = atoi(argv[i]);
      if (i+1 < argc)
        height = atoi(argv[i+1]);
      break;
    }
  }

  if (help == 1 || width <= 0 || height <= 0 || (draw == 0 && print == 0)) {
    printf("Usage: %s options <width> <height>\n", argv[0]);
    printf("  -d, --draw         %s\n", "Draw the maze");
    printf("  -p, --print        %s\n", "Print maze array");
    printf("  -W, --wall <char>  %s\n", "The wall symbol");
    printf("  -P, --path <char>  %s\n", "The path symbol");
    return !help;
  }

  srand(time(0));
  dir = calloc(5, sizeof(int));
  dir[0] = DIR_N;
  dir[1] = DIR_S;
  dir[2] = DIR_E;
  dir[3] = DIR_W;
  maze_t *maze = maze_create(width, height, wall, path, 1, 1);
  maze->step = step;
  maze_set(maze, 1);
  maze_set_value_for(maze, maze->root_point->x, maze->root_point->y, 0);
  maze_generate(maze);
  if (draw)
    maze_draw(maze);
  else if (print)
    maze_print(maze);
  maze_destroy(maze);
  free(dir);
  return 0;
}
