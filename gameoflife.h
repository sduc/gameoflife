#ifndef GAMEOFLIFE_H_
#define GAMEOFLIFE_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_GRID_SIZE 200

typedef enum {
    UNINIT=-1,
    FAILURE=0,
    SUCCESS=1
} err_t;

#define ERR_print(x) fprintf(stderr,(x))
#define LOG(x) fprintf(stdout,"LOG: %s\n" , (x))

typedef enum {
    DEAD = 0,
    ALIVE = 1,
} cell_state;

typedef enum {
    CORNER_NO,
    CORNER_NE,
    CORNER_SO,
    CORNER_SE,
    BORDER_N,
    BORDER_S,
    BORDER_O,
    BORDER_E,
    CENTER,
    OUT_OF_BOUND
} cell_location_t;

typedef struct {
    cell_state *cells;
    unsigned nrow;
    unsigned ncol;
} grid;

cell_location_t grid_get_cell_location_type(const grid *g, int rowpos, int colpos);

grid* grid_new(const unsigned nrow, const unsigned ncol);
err_t grid_copy(grid *from, grid *to);
void grid_free(grid *g);

err_t grid_next_state(grid *g);
err_t cell_next_state(grid *g_next, grid *g, unsigned rowpos, unsigned colpos);

cell_state grid_get_cell(const grid *g, unsigned rowpos, unsigned colpos);

err_t grid_init_cells(grid *g);

err_t grid_set_cell(
        const grid *g, 
        unsigned rowpos, 
        unsigned colpos,
        cell_state st);

int grid_fprint(FILE *stream, grid *g);
int cell_fprint(FILE *stream, cell_state st);

int grid_equals(grid *g1, grid *g2);

#ifdef __cplusplus
}
#endif

#endif
