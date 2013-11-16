//
//  Echikson.h
//  P3
//
//  Created by Samuel Echikson on 11/9/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#ifndef __P3__Echikson__
#define __P3__Echikson__


#include <iostream>
#include <string>
#include "card.h"
#include "shedgame.h"
#include "player.h"
using namespace std;

class Echikson: public Player {
public:
    Echikson(ShedGame& g, const string& nm): Player(g, nm){}
    virtual ~Echikson() {
        delete table;
        //delete cardToPlay;
    }
    
    // Pure virtual functions.
    virtual void reset();              // Issued just before dealing.
    virtual void prepare();            // Issued just before game starts, after dealing.
    virtual ShedGame::Option ask();    // What do you want to do?
    virtual void take(const Card&);    // Take this card.
    virtual Card::Suit setSuit();      // Issued after playing a wild card.
    virtual Card playCard();           // What card do you want to play?
    virtual void inform(int p, int s, int t); // Player with id p is in stage s and has t cards.
    virtual void disqualified(int p);  // Player with id p has been disqualified.
    
    // Virtual function with a default implementation.
    virtual void restocked() {}
    
private:
    int suitCount[4];
    int highest;
    int highestIndex;
    
    bool initialDraw;
    int stage;
    bool refilling;
    bool firstRefilling;
    bool isDone;
    bool burnPhase;
    int indexToPlay;
    bool cardToPlayIsset;
    
    //cardCounts
    int skipCardCount;
    int reverseCardCount;
    int draw2CardCount;
    int draw5CardCount;
    int cancelCardCount;
    int wildCardCount;
    int burnCardCount;
    
    int otherPlayer[3];
    int* table;
    
    int getCard(string);
    int calculateHighest();
    bool canPlay();
    void printStats() const;
};


#endif /* defined(__P3__Echikson__) */
