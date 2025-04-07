#ifndef TOKEN_H
#define TOKEN_H

class Token {
private:
    int position;
    bool isFinished;

public:
    Token();

    int getPosition();
    void setPosition(int pos);
    void markFinished();
    bool finished();
};

#endif
