#include "../Header Files/Player.h"


Player::Player(std::string playerName) {
    name = playerName;
}

std::string Player::getName() {
    return name;
}

Token& Player::getToken(int index) {
    return tokens[index];
}

int Player::countFinishedTokens() {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (tokens[i].finished()) {
            count++;
        }
    }
    return count;
}
