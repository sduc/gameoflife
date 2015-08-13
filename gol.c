/*=================================================
                  INCLUDES
=================================================*/

#include <math.h>

/*=================================================
           MACROS AND DEFINITIONS
=================================================*/

//#define GOL_LOG_ENABLE //uncomment to enable logs
#ifdef GOL_LOG_ENABLE
#define GOL_LOG(x) fprintf(stderr, x)
#else
#define GOL_LOG(x)
#endif

/*=================================================
              INTERNAL FUNCTIONS
=================================================*/

static inline int is_perfect_sqr(int n)
{
    int sqrtn = (int) round(sqrt(n));
    return (sqrtn*sqrtn == n)
}

static inline int size_is_mult_overflow(size_t l, size_t r)
{
    return (SIZE_MAX / l < r);
}

/*=================================================
              EXTERNAL FUNCTIONS
=================================================*/

gol_err_t gol_new(gol_t *g, size_t size)
{
    if (g == NULL) {
        return GOL_NULL_PTR;
    }

    if (!is_perfect_sqr(size))
    {
        return GOL_INVAL_INPUT_ERR;
    }

    if (size_is_mult_overflow(size, sizeof(cell_t)))
    {
        return GOL_SIZE_OVERFLOW;
    }

    g->size = size;
    g->cells = (cell_t *) malloc (size * sizeof(cell_t));

    if (g->cells == NULL)
    {
        return GOL_MALLOC_FAIL;
    }

    return GOL_SUCCESS;
}

void gol_free(gol_t *t)
{
    if (g == NULL || g->cells == NULL) return;

    free(g->cells);
}

gol_err_t gol_init(gol_t *g)
{
}
