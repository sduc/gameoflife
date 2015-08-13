#include <stdlib.h>
#include <string.h>
#include "gameoflife.h"

static inline __attribute__((nonnull)) int 
invalid_grid(const grid *g)
{
    return !g || !g->cells;
}

static inline __attribute__((nonnull)) int 
invalid_grid_pos(const grid *g, int rowpos, int colpos)
{
    return !g || !g->cells || (rowpos >= g->nrow) || (colpos >= g->ncol);
}

static inline __attribute__((nonnull)) int 
grid_dimension_equals(const grid *g1, const grid *g2)
{
    return g1->nrow == g2->nrow && g1->ncol == g2->ncol;
}

cell_location_t grid_get_cell_location_type(const grid *g, int rowpos, int colpos)
{
    if (invalid_grid_pos(g, rowpos, colpos)){
        //error
        ERR_print("grid_get_cell_location_type: invalid grid and position\n");
        return (int) OUT_OF_BOUND;
    }
    if (rowpos == 0){
        if (colpos == 0){
            return CORNER_NO;
        }
        else if (colpos == g->ncol - 1){
            return CORNER_NE;
        }
        else {
            return BORDER_N;
        }
    }
    else if (rowpos == g->nrow -1){
        if (colpos == 0){
            return CORNER_SO;
        }
        else if (colpos == g->ncol -1){
            return CORNER_SE;
        }
        else {
            return BORDER_S;
        }
    }
    else {
        if (colpos == 0){
            return BORDER_O;
        }
        else if (colpos == g->ncol -1){
            return BORDER_E;
        }
        else {
            return CENTER;
        }
    }
}

grid *grid_new(unsigned nrow, unsigned ncol)
{
    grid *g = NULL;

    g = (grid *) malloc(sizeof(grid));
    if (!g){
        //error
        ERR_print("grid_new: could not allocate memory for grid\n");
        return NULL;
    }

    g->nrow = nrow;
    g->ncol = ncol;
    g->cells = NULL;

    g->cells = (cell_state *) malloc(nrow*ncol*sizeof(cell_state));
    if (!g->cells){
        //error
        ERR_print("grid_new: could not allocate memory for cells\n");
        return NULL;
    }

    return g;
}

err_t grid_copy(grid *from, grid *to)
{
    if (invalid_grid(from) || invalid_grid(to)){
        ERR_print("grid_copy: invalid grids\n");
        return FAILURE;
    }
    if (!grid_dimension_equals(from,to)){
        ERR_print("grid_copy: unmatching grid dimensions\n");
        return FAILURE;
    }
    memcpy(to->cells, from->cells, sizeof(cell_state)*to->nrow*to->ncol);
    return SUCCESS;
}

void grid_free(grid *g)
{
    if (!g){
        return;
    }

    if (g->cells){
        free(g->cells);
    }
    free(g);
}

err_t grid_init_cells(grid *g)
{
    int i;
    if (invalid_grid(g)){
        //error
        ERR_print("grid_init_cells: invalid grid\n");
        return FAILURE;
    }

    memset(g->cells, 0, sizeof(g->cells));
    return SUCCESS;
}

static __attribute__((nonnull)) unsigned 
cell_num_living_neigbors(
        const grid *g, 
        unsigned rowpos, 
        unsigned colpos)
   {
    unsigned n_liv_neig = 0;

    switch(grid_get_cell_location_type(g,rowpos,colpos))
    {
        case CORNER_NO:
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            break;

        case CORNER_NE:
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            break;

        case CORNER_SO:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            break;

        case CORNER_SE:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            break;

        case BORDER_N:
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            break;

        case BORDER_S:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            break;

        case BORDER_O:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            break;

        case BORDER_E:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            break;

        case CENTER:
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos-1,colpos);
            n_liv_neig += grid_get_cell(g,rowpos,colpos-1);
            n_liv_neig += grid_get_cell(g,rowpos,colpos+1);
            n_liv_neig += grid_get_cell(g,rowpos+1,colpos);
            break;

    }

    return n_liv_neig;
}

err_t cell_next_state(grid *g_next, grid *g, unsigned rowpos, unsigned colpos)
{
    cell_state this_cell; 
    unsigned n_liv_neig = 0;

    if (invalid_grid_pos(g, rowpos, colpos)){
        //error
        ERR_print("cell_next_state: invalid grid and position\n");
        return FAILURE;
    }
    
    this_cell = grid_get_cell(g, rowpos, colpos);

    n_liv_neig = cell_num_living_neigbors(g,rowpos,colpos); 

    if (this_cell == ALIVE){
        if (n_liv_neig < 2 || n_liv_neig > 3){
            grid_set_cell(g_next, rowpos, colpos, DEAD);
        }
    }
    else{
        if (n_liv_neig == 3){
            grid_set_cell(g_next, rowpos, colpos, ALIVE);
        }
    }
    return SUCCESS;

}

err_t grid_next_state(grid *g)
{
    unsigned i,j;
    grid *tmp = NULL;

    if (invalid_grid(g)){
        //error
        ERR_print("grid_next_state: invalid grid \n");
        return FAILURE;
    }

    tmp = grid_new(g->nrow, g->ncol);
    if (!tmp){
        ERR_print("grid_next_state: failed to alloc memory for tmp gird\n");
        return FAILURE;
    }

    grid_copy(g, tmp);

    for(i = 0; i < g->nrow; ++i){
        for(j = 0; j < g->ncol; ++j){
            cell_next_state(g, tmp, i, j);
        }
    }

    grid_free(tmp);

    return SUCCESS;
}

cell_state grid_get_cell(const grid *g, unsigned rowpos, unsigned colpos)
{
    if (invalid_grid_pos(g, rowpos, colpos)){
        //error
        ERR_print("grid_get_cell: invalid grid and position\n");
        exit(EXIT_FAILURE);
    }
    return g->cells[rowpos * g->ncol + colpos];
}

err_t grid_set_cell(
        const grid *g, 
        unsigned rowpos, 
        unsigned colpos,
        cell_state st)
{
    if (invalid_grid_pos(g, rowpos, colpos)){
        //error
        ERR_print("grid_set_cell: invalid grid and position\n");
        return UNINIT;
    }

    g->cells[rowpos * g->ncol + colpos] = st;

    return SUCCESS;
}

int grid_fprint(FILE *stream, grid *g)
{
    int i,j,tmp,ret;

    ret = 0;

    if (!stream){
        ERR_print("grid_fprint: stream NULL\n");
        return -1;
    }
    if (invalid_grid(g)){
        //error
        ERR_print("grid_fprint: invalid grid\n");
        return -1;
    }

    for(i = 0; i < g->nrow; ++i){
        for(j = 0; j < g->ncol; ++j){
            if ((tmp = cell_fprint(stream,grid_get_cell(g,i,j))) < 0){
                ERR_print("grid_fprint: failed to print\n");
                return tmp;
            }
            ret += tmp;
        }
        ret += fprintf(stream, "\n");
    }
    return ret;
}

int cell_fprint(FILE *stream, cell_state st)
{
    if (!stream){
        ERR_print("cell_fprint: stream NULL\n");
        return -1;
    }
    return fprintf(stream,"%c", (st==ALIVE)?'x':'.');
}

int grid_equals(grid *g1, grid *g2)
{
    if (invalid_grid(g1) || invalid_grid(g2)){
        ERR_print("grid_equals: invalid grids\n");
        return -1;
    }
    if (!grid_dimension_equals(g1,g2)){
        LOG("grid_equals: dimension mismatch");
        return 0;
    }
    return (memcmp(
                g1->cells, 
                g2->cells, 
                sizeof(cell_state) * g1->nrow * g1->ncol
                )== 0) ? 1:0;
}
