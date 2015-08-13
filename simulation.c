#include "gameoflife.h"
#include <stdio.h>

#define N 10

void print_stage(grid *g)
{
    printf("New stage\n");
    printf("---------\n\n");
    grid_fprint(stdout,g);
    printf("\n\n");
}

void simulate(grid *g)
{
    grid *old = NULL;

    old = grid_new(g->nrow,g->ncol);

    do {
        print_stage(g);

        grid_copy(g,old);
        grid_next_state(g);
    } while(!grid_equals(g,old));

    grid_free(old);
    old = NULL;
}

int main(void)
{
    grid *g = NULL;
    g = grid_new(N,N);
    grid_set_cell(g, 3, 3, 1);
    grid_set_cell(g, 3, 4, 1);
    grid_set_cell(g, 3, 5, 1);

    simulate(g);

    grid_free(g);
    g = NULL;
}
