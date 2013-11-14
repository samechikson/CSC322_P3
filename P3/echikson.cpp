//
//  Echikson.cpp
//  P3
//
//  Created by Samuel Echikson on 11/9/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#include <iostream>
#include <string>
#include "card.h"
#include "shedgame.h"
#include "echikson.h"
using namespace std;

void Echikson::reset(){
    //init suit counts to all 0's.
    for (int i=0; i < 4; i++)
        suitCount[i] = 0;
    
    highest = 0;
    highestIndex = 0;
    
    skipCardCount = 0;
    reverseCardCount = 0;
    draw2CardCount = 0;
    draw5CardCount = 0;
    cancelCardCount = 0;
    wildCardCount = 0;
    burnCardCount = 0;
    
    stage = game.handSize();
    refilling = false;
    isDone = false;
}

void Echikson::prepare(){
    //intial draw is like refilling hand with cards up to stage.
    refilling = true;
}

ShedGame::Option Echikson::ask(){
    
    if (hand.size() == 0 && !refilling){
        if (stage == 1)
            return ShedGame::Win;
        else{
            stage--;
            cout << "[" << name << ": Stage " << stage << "]" << endl;
            refilling = true;
            return ShedGame::GetCard; //begin drawing cards to get stage number of new cards.
        }
    }
    else if (hand.size() > 0 && refilling){
        if (hand.size() == stage){
            refilling = false;
            return ShedGame::PlayCard;//end of refill. Play on.
        }
        else
            return ShedGame::GetCard;//refilling hand.
    }
    else if (hand.size() > 0 && game.getContract() == 0)
        return ShedGame::PlayCard;
    else if (game.getContract() > 0){
        if (cancelCardCount > 0)
            return ShedGame::PlayCard;
        return ShedGame::GetCard;
    }
    else if (isDone)
        return ShedGame::Done;
    
    return ShedGame::PlayCard;
}


void Echikson::take(const Card& c){
    hand.push_back(c);
    
    suitCount[c.getSuit()]++;
    
    if(game.isSkipper(c)) skipCardCount++;
    else if(game.isReverser(c)) reverseCardCount++;
    else if(game.isDrawTwo(c)) draw2CardCount++;
    else if(game.isDrawFive(c)) draw5CardCount++;
    else if(game.isCancel(c)) cancelCardCount++;
    else if(game.isWild(c)) wildCardCount++;
    else if(game.isBurner(c)) burnCardCount++;
    
}

Card::Suit Echikson::setSuit(){
    
    calculateHighest();
    return Card::suits[highestIndex]; //return suit of which I have the highest count.
}

Card Echikson::playCard(){
    Card topCard(game.getCurRank(), game.getCurSuit());
    
    if (game.isDrawFive(topCard)){
        int haveDraw5 = getCard("draw5");
        if (haveDraw5 >= 0)
            return hand[haveDraw5];
        
    }
    else if (game.isDrawTwo(topCard)){
        int haveDraw2 = getCard("draw2");
        if (haveDraw2 >= 0)
            return hand[haveDraw2];
    }
    //go through hand, and see if one fits the current suit or rank.
    for (int i=0; i<hand.size(); i++){
        if (hand[i].getRank() == game.getCurRank()) {
            return hand[i];
        }
        else if (hand[i].getSuit() == game.getCurSuit()) {
            return hand[i];
        }
    }
    
    return hand.back();
}

void Echikson::inform(int p, int s, int t){
    
}

//signifies that player[p] has been disqualified.
void Echikson::disqualified(int p){
    
}

/*
 * Private Functions
 */

int Echikson::getCard(string cardName){
    for (int i=0; i<hand.size(); i++){
        if (cardName == "cancel" && game.isCancel(hand[i]))
            return i;
        else if (cardName == "draw5" && game.isDrawFive(hand[i]))
            return i;
        else if (cardName == "draw2" && game.isDrawTwo(hand[i]))
            return i;
        else if (cardName == "wild" && game.isWild(hand[i]))
            return i;
        else if (cardName == "skipper" && game.isSkipper(hand[i]))
            return i;
        else
            return -1;
    }
    return -1;
}

//Function that goes through hand and sets the state of highest suit count.
int Echikson::calculateHighest(){
    for (int i=0; i < 4; i++){
        if (suitCount[i] > highest) {
            highest = suitCount[i];
            highestIndex = i;
        }
    }
    return highestIndex;
}


void Echikson::printStats() const{
    cout << "highest: " << highest << endl;
    cout << "highestIndex: " << highestIndex << endl << endl;
    
    cout << "skipCardCount: " << skipCardCount << endl;
    cout << "reverseCardCount: " << reverseCardCount << endl;
    cout << "draw2CardCount: " << draw2CardCount << endl;
    cout << "draw5CardCount: " << draw5CardCount << endl;
    cout << "cancelCardCount: " << cancelCardCount << endl;
    cout << "wildCardCount: " << wildCardCount << endl;
    cout << "burnCardCount: " << burnCardCount << endl << endl;
    
}
