// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#ifndef player_h
#define player_h

#include <iostream>
#include <string>
#include "card.h"
#include "shedgame.h"
using namespace std;

class Player {
  public:
    Player(ShedGame& g, const string& nm) : game(g), name(nm) {
        id = nextPlayerId++;
    }
    virtual ~Player() {}

    // Pure virtual functions.
    virtual void reset() = 0;              // Issued just before dealing.
    virtual void prepare() = 0;            // Issued just before game starts, after dealing.
    virtual ShedGame::Option ask() = 0;    // What do you want to do?
    virtual void take(const Card&) = 0;    // Take this card.
    virtual Card::Suit setSuit() = 0;      // Issued after playing a wild card.
    virtual Card playCard() = 0;           // What card do you want to play?
    virtual void inform(int p, int s, int t) = 0; // Player with id p is in stage s and has t cards.
    virtual void disqualified(int p) = 0;  // Player with id p has been disqualified.

    // Virtual function with a default implementation.
    virtual void restocked() {}

    // Non-overridable functions
    string getName() const;
    void printHand() const;
    int getId() const { return id; }

  protected:
    int id;  // Player number
    ShedGame& game;
    const string name;
    vector<Card> hand;

  private:
    static int nextPlayerId;
};

#endif
