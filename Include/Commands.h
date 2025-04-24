#ifndef COMMANDS_H
#define COMMANDS_H

// Kommandoer i startfasen af spillet – 'P' starter spilfasen
typedef enum {
    LD,  // Load deck
    SW,  // Show deck
    SI,  // Split
    SR,  // Shuffle random
    SD,  // Save deck
    QQ,  // Quit program
    P    // Start play phase
} StartupCommand;

// Kommandoer i spilfasen – 'Q' går tilbage til startfasen
typedef enum {
    Q,      // Quit play phase
    MOVES,  // Show legal moves
    U,      // Undo
    R,      // Redo
    S,      // Save game
    L       // Load game
} PlayCommand;

// Enum til at repræsentere spillets faser
typedef enum {
    STARTUP,
    PLAY
} GamePhase;

// Funktion til at udføre kommandoer baseret på fase
void executeCommand(GamePhase phase, void *command, char *message);

#endif // COMMANDS_H