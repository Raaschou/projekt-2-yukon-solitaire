//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FILEIO_H
#define FILEIO_H

#include "Board.h"

int load_board_from_file(Board *board, const char *filename);
int save_board_to_file(const Board *board, const char *filename);

#endif