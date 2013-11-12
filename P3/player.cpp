// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#include <iostream>
#include <string>
#include "player.h"
using namespace std;

string Player::getName() const {
    return name;
}

void Player::printHand() const {
    if (hand.empty()) {
        cout << "[]";
    } else {
        cout << "[" << hand[0];
        for (int i=1; i<hand.size(); i++) cout << " " << hand[i];
        cout << "]";
    }
}

// Initialize the static field.
int Player::nextPlayerId = 0;
