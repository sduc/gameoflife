#ifndef PLAYER_H_
#define PLAYER_H_

#ifndef PLAYER
#define PLAYER gol_player

typedef struct {
    const char *name;
} gol_player;

typedef struct {
    cell_state cell;
    gol_player owner;
} player_cell; 

typedef struct {

} player_grid;
