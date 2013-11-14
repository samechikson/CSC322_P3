//
//  main.cpp
//  P3
//
//  Created by Samuel Echikson on 11/9/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#include <iostream>
#include "card.h"
#include "crazy8s.h"
#include "echikson.h"
using namespace std;

int main(int argc, const char * argv[])
{
    cout << "start" << endl;

//    //card class tests.
//    Card crazy(Card::ace, Card::spades);
//    Card crazy2(Card::ace, Card::diamonds);
//    Card eight(Card::eight, Card::diamonds);
//    Card queenSpades(Card::queen, Card::spades);
//    Card fourhearts(Card::four, Card::hearts);
//    
////    cout << "card: " << crazy << endl;
////    cout << "card2: " << crazy2 << endl;
////    cout << (crazy == crazy2) << endl;
//
//    
//    Crazy8s game("game1");
//    
//    cout << game.handSize() << endl;
//    cout << game.isWild(eight) << endl;
//    cout << game.isDrawFive(queenSpades) << endl;
//    cout << game.isBurner(fourhearts) << endl;
    
    
    Crazy8s* game = new Crazy8s("game2");
    Player* p1 = new Echikson(*game, "p1");
    Player* p2 = new Echikson(*game, "p2");
    
    game->addPlayer(p1);
    game->addPlayer(p2);
    
    game->reset();
    game->play();
    
    return 0;
}

