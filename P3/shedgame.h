// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#ifndef shedgame_h
#define shedgame_h

#include <vector>
#include <stack>
#include <string>
#include "card.h"
using namespace std;

class Player;

class ShedGame {
  public:
    ShedGame(const string& nm) : name(nm) {}
    virtual ~ShedGame();

    // Accessors.
    Card::Suit getCurSuit() const;
    Card::Rank getCurRank() const;
    int getContract() const;
    string getName() const { return name; }
    int numPlayers() const { return player.size(); }
    Player* getPlayer(int i) const { return player[i]; }
    int getIncr() const { return incr; }

    // Pure virtual functions.
    virtual bool isDrawTwo(const Card&) const = 0;
    virtual bool isDrawFive(const Card&) const = 0;
    virtual bool isBurner(const Card&) const = 0;
    virtual bool isReverser(const Card&) const = 0;
    virtual bool isSkipper(const Card&) const = 0;
    virtual bool isWild(const Card&) const = 0;
    virtual bool isCancel(const Card&) const = 0;
    virtual int handSize() const = 0;

    // For gathering stats: how many cards has player with identity id played so far?
    int getCardsPlayed(int id) const { return played[id]; }

    // Principal functions for playing a game.
    void addPlayer(Player*);
    void reset();
    int play();

    // Enumeration type for player's option at game time.
    enum Option {GetCard, PlayCard, Done, Win};

  protected:
    const string name;

  private:
    Card::Suit curSuit;
    vector<Player*> player;
    vector<int> net;     // Current hand size of each player.
    vector<int> played;  // Total number of cards played by each player.
    vector<int> stage;   // Size of last hand drawn by each player.
    vector<Card> stock;
    stack<Card> discard;

    // Variables and state flags for running the game.
    int cur;
    int curId;
    int incr;
    int contract;
    int nextContract;
    int personalContract;
    bool burning;

    // Handy private methods.
    void nextPlayer();
    void restock();
    void disqualify(int, const string&);
};

#endif
