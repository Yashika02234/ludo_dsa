#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "../Header Files/Player.h"

using namespace std;

const int BOARD_SIZE = 52;
const int safeZones[] = {0, 8, 13, 21, 26, 34, 39, 47};
const int numSafeZones = sizeof(safeZones) / sizeof(safeZones[0]);

int rollDice() {
    return (rand() % 6) + 1;
}

bool isSafeZone(int position) {
    for (int i = 0; i < numSafeZones; i++) {
        if (safeZones[i] == position) return true;
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

void drawEmptyLudoBoard() {
    string board[15][15];

    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            board[i][j] = "  ";

    // Red Home (Top-left)
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            board[i][j] = "RR";

    // Blue Home (Bottom-left)
    for (int i = 9; i < 15; i++)
        for (int j = 0; j < 6; j++)
            board[i][j] = "BB";

    // Center star
    for (int i = 6; i < 9; i++)
        for (int j = 6; j < 9; j++)
            board[i][j] = "**";

    // Only red and blue paths
    for (int i = 0; i < 6; i++) board[i][6] = "[]";
    for (int i = 0; i < 6; i++) board[6][i] = "[]";

    for (int i = 9; i < 15; i++) board[i][8] = "[]";
    for (int j = 0; j < 6; j++) board[8][j] = "[]";

    cout << "\n========== ASCII LUDO BOARD ==========\n\n";
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++)
            cout << board[i][j];
        cout << endl;
    }
    cout << "\n======================================\n";
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
        drawEmptyLudoBoard();

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
                    chosen.setPosition(newPos);
                    cout << "Token moved from " << oldPos << " to " << newPos << endl;
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
