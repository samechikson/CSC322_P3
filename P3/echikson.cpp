//
//  Echikson.cpp
//  P3
//
//  Created by Samuel Echikson on 11/9/13.
//  Copyright (c) 2013 Samuel Echikson. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
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
    firstRefilling = false;
    refilling = false;
    isDone = false;
    burnPhase = false;
}

void Echikson::prepare(){
    //intial draw is like refilling hand with cards up to stage.
//    refilling = true;
    firstRefilling = true;
}

ShedGame::Option Echikson::ask(){
    Card topCard(game.getCurRank(), game.getCurSuit());
    
    if (hand.size() == 0 && !refilling){
        if (stage == 0)
            return ShedGame::Win;
        else{
            stage--;
            cout << "[" << name << ": Stage " << stage << "]" << endl;
            refilling = true;
            isDone = false;
            return ShedGame::GetCard; //begin drawing cards to get stage number of new cards.
        }
    }
    //First hand of game. Fill hand and Play card
    else if (firstRefilling){
        if (hand.size() == game.handSize()){
            firstRefilling = false;
            //if cur card is either a draw 2 or draw 5 card. Invoke PlayCard if I have those cards to counter.
            if (game.isDrawTwo(topCard)){
//                if (draw2CardCount > 0)
//                    return ShedGame::PlayCard;
//                else
                    return ShedGame::GetCard;
            }
            if (game.isDrawFive(topCard)){
//                if (draw5CardCount > 0)
//                    return ShedGame::PlayCard;
//                else
                    return ShedGame::GetCard;
            }
            return ShedGame::PlayCard;
        }
        else
            return ShedGame::GetCard;
    }
    else if (refilling){
//        else if (game.getContract() > 0)
//            return ShedGame::GetCard;
        if (game.getContract() == 0){
            refilling = false;
            return ShedGame::Done;//After refilling, can't play card.
        }
        else
            return ShedGame::GetCard;//refilling hand. Still under contract.
    }
//    else if (game.getContract() == 5 || (game.getContract() == 2 && !refilling)){
//        refilling = true;
//        if (cancelCardCount > 0)
//           return ShedGame::PlayCard;
//        return ShedGame::GetCard;
//    }
    else if (game.getContract() > 0){
        refilling = true;
        return ShedGame::GetCard;
    }
    else if (isDone){
        isDone = !isDone;
        return ShedGame::Done;
    }
//    else if (burnPhase){
//        for (int i=0; i < hand.size(); i++){
//            if (hand[i].getSuit() == topCard.getSuit())
//                return ShedGame::PlayCard;
//        }
//        burnPhase = false;
//        return ShedGame::Done;
//    }
    else{
        //go through hand, and see if one fits the current suit or rank. If yes, indicate PlayCard
        for (int i=0; i<hand.size(); i++){
            if (hand[i].getRank() == game.getCurRank() || hand[i].getSuit() == game.getCurSuit())
                return ShedGame::PlayCard;
        }
        //If no rank or suit, try to find a wild card.
        for (int i=0; i<hand.size(); i++){
            if (game.isWild(hand[i]))
                return ShedGame::PlayCard;
        }
        
        //if none work, get a card.
        return ShedGame::GetCard;
    }
    
    return ShedGame::PlayCard;
}


void Echikson::take(const Card& c){
    hand.push_back(c);
    
    if (c.getSuit() < 4) suitCount[c.getSuit()]++;//there was a bug where c's suit was a large integer
    
    if(game.isSkipper(c)) skipCardCount++;
    else if(game.isReverser(c)) reverseCardCount++;
    else if(game.isDrawTwo(c)) draw2CardCount++;
    else if(game.isDrawFive(c)) draw5CardCount++;
    else if(game.isCancel(c)) cancelCardCount++;
    else if(game.isWild(c)) wildCardCount++;
    else if(game.isBurner(c)) burnCardCount++;
    
//    cout << name << "        hand: " ;
//    printHand();
//    cout << "  size: " << hand.size() << endl;
}

Card::Suit Echikson::setSuit(){
    
    calculateHighest();
    return Card::suits[highestIndex]; //return suit of which I have the highest count.
}

Card Echikson::playCard(){
    Card topCard(game.getCurRank(), game.getCurSuit());
    int returnIndex = -1;
    bool foundCard = false;
    int i;
    
    if (game.isDrawFive(topCard)){
        int haveDraw5 = getCard("draw5");
        if (haveDraw5 >= 0)
            returnIndex = haveDraw5;
        
    }
    else if (game.isDrawTwo(topCard)){
        int haveDraw2 = getCard("draw2");
        if (haveDraw2 >= 0)
            returnIndex = haveDraw2;
    }
    //go through hand, and see if one fits the current suit or rank.
    i = 0;
    while (!foundCard && i<hand.size()) {
        if (hand[i].getRank() == game.getCurRank()){
            returnIndex = i;
            foundCard = true;
        }
        else if (hand[i].getSuit() == game.getCurSuit()){
            returnIndex = i;
            foundCard = true;
        }
        i++;
    }
    
    i = 0;
    while (!foundCard && i<hand.size()) {
        if (game.isWild(hand[i])){
            foundCard = true;
            returnIndex = i;
        }
        i++;
    }
    
//    cout << "before delete: ";
//    printHand();
//    cout << endl;
    if(game.isSkipper(hand[returnIndex])) skipCardCount--;
    else if(game.isReverser(hand[returnIndex])) reverseCardCount--;
    else if(game.isDrawTwo(hand[returnIndex])) draw2CardCount--;
    else if(game.isDrawFive(hand[returnIndex])) draw5CardCount--;
    else if(game.isCancel(hand[returnIndex])) cancelCardCount--;
    else if(game.isWild(hand[returnIndex])) wildCardCount--;
    else if(game.isBurner(hand[returnIndex])){
        burnCardCount--;
        burnPhase = true;
    }
    
    isDone = true;
    Card cardToReturn(hand[returnIndex].getRank(), hand[returnIndex].getSuit());//copy card so that it is not deleted before it is returned.0
    hand.erase(hand.begin() + returnIndex);//delete element at returnIndex
    
//    cout << "after delete: ";
//    printHand();
//    cout << endl;
    return cardToReturn;
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
