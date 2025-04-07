#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Token.h"

class Player {
private:
    std::string name;
    Token tokens[4];

public:
    Player(std::string playerName);

    std::string getName();

    Token& getToken(int index); // get reference to a token
    int countFinishedTokens();
};

#endif
