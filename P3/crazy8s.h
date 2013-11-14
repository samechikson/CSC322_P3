//
//  crazy8s.h
//  P3
//
//  Created by Samuel Echikson on 11/9/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#ifndef __P3__crazy8s__
#define __P3__crazy8s__

#include <vector>
#include <stack>
#include <string>
#include "card.h"
#include "shedgame.h"
using namespace std;

class Crazy8s: public ShedGame {
public:
    Crazy8s(const string& nm) : ShedGame(nm), name(nm){};
    virtual ~Crazy8s(){};
    
    // Pure virtual functions.
    bool isDrawTwo(const Card& c) const{
        return c.getRank() == Card::two;
    };
    bool isDrawFive(const Card& c) const{
        Card compare(Card::queen, Card::spades);
        return c == compare;
    };
    bool isBurner(const Card& c) const{
        return (c.getRank() == Card::four && (c.getSuit() == Card::hearts || c.getSuit() == Card::diamonds)) ||
        (c.getRank() == Card::five && (c.getSuit() == Card::spades || c.getSuit() == Card::clubs));
    };
    bool isReverser(const Card& c) const{
        return c.getRank() == Card::queen;
    };
    bool isSkipper(const Card& c) const{
        return c.getRank() == Card::ace;   
    };
    bool isWild(const Card& c) const{
        return c.getRank() == Card::eight;
    };
    bool isCancel(const Card& c) const{
        return c.getRank() == Card::king && c.getSuit() == Card::diamonds;
    };
    int handSize() const{
        return 8;
    };
    
    // Enumeration type for player's option at game time.
    enum Option {GetCard, PlayCard, Done, Win};
    
protected:
    const string name;
    
};


#endif /* defined(__P3__crazy8s__) */
