#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "../Header Files/Player.h"

const int BOARD_SIZE = 52;

using namespace std;

int rollDice() {
    return (rand() % 6) + 1;
}

const int safeZones[] = {0, 8, 13, 21, 26, 34, 39, 47};
const int numSafeZones = sizeof(safeZones) / sizeof(safeZones[0]);

bool isSafeZone(int position) {
    for (int i = 0; i < numSafeZones; i++) {
        if (safeZones[i] == position)
            return true;
    }
    return false;
}

void showPlayerTokens(Player& player) {
    cout << player.getName() << "'s Tokens:\n";
    for (int i = 0; i < 4; i++) {
        Token& token = player.getToken(i);
        cout << "  Token " << i + 1 << " -> ";
        if (token.finished()) cout << "Finished\n";
        else if (token.getPosition() == -1) cout << "In Base\n";
        else cout << "At Position: " << token.getPosition() << endl;
    }
}

void checkForKills(Player* attacker, Player* players[], int totalPlayers, int attackerIndex, int pos) {
    if (isSafeZone(pos)) {
        cout << "Safe Zone at position " << pos << " — No kill allowed!\n";
        return;
    }

    for (int i = 0; i < totalPlayers; i++) {
        if (i == attackerIndex) continue;

        Player* victim = players[i];
        for (int j = 0; j < 4; j++) {
            Token& enemy = victim->getToken(j);
            if (!enemy.finished() && enemy.getPosition() == pos) {
                enemy.setPosition(-1);
                cout << attacker->getName() << " killed "
                     << victim->getName() << "'s token " << j + 1 << "!\n";
            }
        }
    }
}

void drawBoardWithTokens(Player* players[], int totalPlayers) {
    string board[15][15];
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            board[i][j] = "  ";

    // Home areas
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            board[i][j] = "RR";
    for (int i = 9; i < 15; i++)
        for (int j = 0; j < 6; j++)
            board[i][j] = "BB";

    // Center
    for (int i = 6; i < 9; i++)
        for (int j = 6; j < 9; j++)
            board[i][j] = "**";

    // Paths (rough layout)
    for (int i = 0; i < 6; i++) board[i][6] = "[]";
    for (int i = 0; i < 6; i++) board[6][i] = "[]";
    for (int i = 9; i < 15; i++) board[i][8] = "[]";
    for (int i = 9; i < 15; i++) board[8][i] = "[]";
    for (int j = 0; j < 6; j++) board[8][j] = "[]";
    for (int j = 9; j < 15; j++) board[6][j] = "[]";
    for (int j = 0; j < 6; j++) board[j][8] = "[]";
    for (int j = 9; j < 15; j++) board[8][j] = "[]";

    // Show tokens on board
    for (int p = 0; p < totalPlayers; p++) {
        Player* player = players[p];
        string symbol = (player->getName() == "Red") ? "R" : "B";

        for (int t = 0; t < 4; t++) {
            Token& token = player->getToken(t);
            if (!token.finished() && token.getPosition() != -1) {
                int displayPos = token.getPosition() % 52;
                int row = 1 + (displayPos / 13);   // Just for display layout
                int col = 1 + (displayPos % 13);
                if (row < 15 && col < 15)
                    board[row][col] = symbol + to_string(t + 1);
            }
        }
    }

    cout << "\n========== ASCII LUDO BOARD ==========\n\n";
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++)
            cout << board[i][j];
        cout << endl;
    }
    cout << "\n======================================\n";
}

void animateMovement(int from, int to) {
    for (int pos = from + 1; pos <= to; ++pos) {
        cout << "Moving... Position: " << (pos % BOARD_SIZE) << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    srand(time(0));

    Player red("Red");
    Player blue("Blue");

    Player* players[2] = { &red, &blue };
    const int totalPlayers = 2;
    int currentPlayerIndex = 0;

    while (true) {
        Player* currentPlayer = players[currentPlayerIndex];

        cout << "\n" << currentPlayer->getName() << "'s turn\n";

        drawBoardWithTokens(players, totalPlayers);

        int dice = rollDice();
        cout << "Rolled a " << dice << "!\n";

        showPlayerTokens(*currentPlayer);

        int tokenChoice;
        cout << "Select token to move (1-4): ";
        cin >> tokenChoice;

        if (tokenChoice < 1 || tokenChoice > 4) {
            cout << "Invalid token choice.\n";
        } else {
            Token& chosen = currentPlayer->getToken(tokenChoice - 1);

            if (chosen.getPosition() == -1) {
                if (dice == 6) {
                    chosen.setPosition(0);
                    cout << "Token entered the board!\n";
                } else {
                    cout << "Need a 6 to enter the board.\n";
                }
            } else {
                int oldPos = chosen.getPosition();
                int newPos = oldPos + dice;

                if (newPos >= 57) {
                    chosen.markFinished();
                    cout << "Token finished!\n";
                } else {
                    newPos = newPos % BOARD_SIZE;
                    animateMovement(oldPos, newPos);
                    chosen.setPosition(newPos);
                    cout << "Token moved to " << newPos << endl;
                    checkForKills(currentPlayer, players, totalPlayers, currentPlayerIndex, newPos);
                }

                if (currentPlayer->countFinishedTokens() == 4) {
                    cout << currentPlayer->getName() << " wins the game!\n";
                    break;
                }
            }
        }

        if (dice != 6)
            currentPlayerIndex = (currentPlayerIndex + 1) % totalPlayers;
    }

    return 0;
}
