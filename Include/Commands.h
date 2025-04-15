//
// Created by August Frandsen on 15/04/2025.
//

#ifndef COMMAND_H
#define COMMAND_H

#include "Board.h"

typedef struct {
    char raw[128];
    char type[32];
    int args[3];
} Command;

Command parse_command(const char *input);
int execute_command(Command cmd, Board *board);

#endif