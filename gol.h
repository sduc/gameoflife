#ifndef GOL_H
#define GOL_H

enum gol_err_t {
    GOL_SUCCESS         =  0,
    GOL_ERR             = -1,
    GOL_NULL_PTR        = -2,
    GOL_INVAL_INPUT_ERR = -3,
    GOL_MALLOC_FAIL     = -4,
    GOL_SIZE_OVERFLOW   = -5,
}

enum cell_state {
    CELL_DEAD,
    CELL_ALIVE
}

typedef struct cell_s {
    enum cell_state  state;
    cell_t          *adj_cells;
    size_t           n_adj_cells;
} cell_t;

typedef struct gol_s {
    cell_t *cells;
    size_t  size;
} gol_t;


/**
 * @brief
 *   Allocate memory for a game of life.
 *
 * @param[out] g    pointer to the game of life to allocate
 * @param[in]  size number of cells to have in the gol
 *
 * @return GOL_SUCCESS on success; error codes otherwise see #gol_err_t
 */
gol_err_t gol_new(gol_t *g, size_t size);

/**
 * @brief
 *   Free a gol_t allocated with gol_new
 */
void gol_free(gol_t *g);

gol_err_t gol_init(gol_t *g);

/**
 * @brief
 *   Update the gol to the next state
 *
 * @param[in/out] g gol to update
 */
gol_err_t gol_update(gol_t *g);

/**
 * @brief
 *   Compare two gol and test for equality
 *
 * @param[in] g_l
 * @param[in] g_r
 *
 * @return 1 of they are equal; 0 otherwise
 */
int gol_equ(const gol_t *g_l, const gol_t *g_r);

#endif/*GOL_H*/

