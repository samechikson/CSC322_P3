//
//  blackjack.h
//  P3
//
//  Created by Samuel Echikson on 11/11/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#ifndef __P3__blackjack__
#define __P3__blackjack__

#include <vector>
#include <stack>
#include <string>
#include "card.h"
#include "shedgame.h"
using namespace std;

class BlackJack: public ShedGame {
public:
    BlackJack(const string& nm) : ShedGame(nm), name(nm){};
    virtual ~BlackJack(){};
    
    // Pure virtual functions.
    bool isDrawTwo(const Card& c) const{
        return c.getRank() == Card::two;
    };
    bool isDrawFive(const Card& c) const{
        return c.getRank() == Card::jack && (c.getSuit() == Card::spades || c.getSuit() == Card::clubs);
    };
    bool isBurner(const Card& c) const{
        return c.getRank() == Card::seven;
    };
    bool isReverser(const Card& c) const{
        return c.getRank() == Card::ten;
    };
    bool isSkipper(const Card& c) const{
        return c.getRank() == Card::eight;
    };
    bool isWild(const Card& c) const{
        return c.getRank() == Card::ace;
    };
    bool isCancel(const Card& c) const{
        return c.getRank() == Card::jack && (c.getSuit() == Card::diamonds || c.getSuit() == Card::hearts);
    };
    int handSize() const{
        return 7;
    };
    
    // Enumeration type for player's option at game time.
    enum Option {GetCard, PlayCard, Done, Win};
    
protected:
    const string name;
    
private:
    Card::Suit curSuit;
    //vector<Player*> player;
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
};


#endif /* defined(__P3__blackjack__) */
