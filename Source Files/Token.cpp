#include "../Header Files/Token.h"


Token::Token() {
    position = -1;
    isFinished = false;
}

int Token::getPosition() {
    return position;
}

void Token::setPosition(int pos) {
    position = pos;
}

void Token::markFinished() {
    isFinished = true;
}

bool Token::finished() {
    return isFinished;
}
