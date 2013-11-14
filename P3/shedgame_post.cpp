// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#include <algorithm>
#include <cstdlib>

#include "card.h"
#include "player.h"
#include "shedgame.h"

using namespace std;

ShedGame::~ShedGame() {
    for (int i=0; i<player.size(); i++) delete player[i];
}

/*
 * Simple accessors.
 */
Card::Suit ShedGame::getCurSuit() const {
    return curSuit;
}

Card::Rank ShedGame::getCurRank() const {
    return discard.top().getRank();
}

int ShedGame::getContract() const {
    return contract;
}

/*
 * Add a player to the game.
 */
void ShedGame::addPlayer(Player* p) {
    player.push_back(p);
    net.push_back(0);
    played.push_back(0);
    stage.push_back(0);
}

/*
 * Prepare a game: prepare the stock, inform each player to prepare
 * to receive the initial cards, and deal the initial hands.
 */
void ShedGame::reset() {
    // Automatically sets number of decks based on number of players.
    int numdecks = 1 + player.size()/3;
    stock.clear();
    while (!discard.empty()) discard.pop();
    for (int i=0; i<numdecks; i++) {
        for (int r=0; r<Card::numRanks; r++) {
            for (int s=0; s<Card::numSuits; s++) {
                stock.push_back(Card(Card::ranks[r], Card::suits[s]));
            }
        }
    }
    random_shuffle(stock.begin(), stock.end());  // This is in <algorithm>

    #ifdef debug
        for (int i=0; i<stock.size(); i++) cout << " " << stock[i];
        cout << endl;
    #endif

    for (int i=0; i<player.size(); i++) {
        player[i]->reset();
        stage[i] = net[i] = handSize();
        played[i] = 0;
    }
    for (int i=0; i<handSize(); i++) {
        for (int j=0; j<player.size(); j++) {
            player[j]->take(stock.back());  // Off back for efficiency.
            stock.pop_back();
        }
    }
}

/*
 * Play a game.
 * Precondition: the players have their initial cards and the stock is set.
 * Returns the index of the winning player.
 */
int ShedGame::play() {
    // Initialize the discard pile.
    discard.push(stock.back());
    stock.pop_back();
    // Shuffle the order of the players.
    random_shuffle(player.begin(), player.end());
    cur = 0;     // First player
    curId = player[cur]->getId();
    incr = 1;    // Initial step
    #ifndef quiet
        cout << "Starting player: " << player[cur]->getId() << " ("
             << player[cur]->getName() << "), "
             << ((incr == 1) ? "ascending" : "descending") << " order.\n"
             << "Opening card: " << discard.top() << endl;
    #endif
    // Set initial flags based on top discard.
    nextContract = 0;
    burning = false;
    if (isReverser(discard.top())) incr = -incr;
    if (isSkipper(discard.top())) nextPlayer();
    if (isDrawTwo(discard.top())) nextContract = 2;
    if (isDrawFive(discard.top())) nextContract = 5;
    curSuit = discard.top().getSuit();
    // Tell each player to get ready to play.
    for (int i=0; i<player.size(); i++) player[i]->prepare();
    Option opt;
    Card c;
    int winner;

    // Principal loop: Keep playing until somebody wins.
    while (opt != Win) {
        contract = nextContract;
        nextContract = 0;
        personalContract = 0;
        bool playedCard = false;
        bool fillingContract = false;
        bool pickedWild = false;
        bool finishedDrawing = false;
        opt = GetCard;
        // Inner loop: handle a player's turn.
        while (opt != Done && opt != Win) {
            #ifdef debug
                cout << "Player " << player[cur]->getId() << ": ";
                player[cur]->printHand();
                cout << endl;
                cout << " top card is " << discard.top() << endl;
            #endif
            opt = player[cur]->ask();
            #ifdef debug
                cout << " selected " << opt << endl;
            #endif
            switch (opt) {
                case GetCard:
                    if (finishedDrawing) {
                        disqualify(cur, "Cannot draw any more cards.");
                        opt = Done;
                    }
                    // The player wants to pick up a card from the stock.
                    if (isWild(stock.back())) pickedWild = true;
                    player[cur]->take(stock.back());
                    net[curId]++;
                    stock.pop_back();
                    if (stock.empty()) restock();
                    if (contract > 0) {
                        // Opting to draw cards to fulfill a contract.
                        // Do not allow playing a card.
                        fillingContract = true;
                        if (--contract == 0) {
                            finishedDrawing = true;
                            #ifndef quiet
                                cout << "Player " << player[cur]->getId() << " ("
                                     << player[cur]->getName() << "): -- ";
                                player[cur]->printHand();
                                cout << endl;
                            #endif
                        }
                    } else if (personalContract > 0) {
                        // Drawing cards for next stage.  No card playing allowed.
                        if (--personalContract == 0) {
                            finishedDrawing = true;
                            #ifndef quiet
                                cout << "Player " << player[cur]->getId() << " ("
                                     << player[cur]->getName() << "): -- ";
                                player[cur]->printHand();
                                cout << endl;
                            #endif
                        }
                    } else if (pickedWild) {
                        // You're done if you pick a wild card when not under contract.
                        finishedDrawing = true;
                    }
                    break;
                case PlayCard:
                    // The player wants to play a card from his or her hand.
                    if (fillingContract) {
                        disqualify(cur, "Cannot play a card after forced draws.");
                        opt = Done;
                    } else if (!burning && playedCard) {
                        disqualify(cur, "Too many cards played.");
                        opt = Done;
                    }
                    c = player[cur]->playCard();
                    #ifndef quiet
                        cout << "Player " << player[cur]->getId() << " ("
                             << player[cur]->getName() << "): "
                             << c << " ";
                        player[cur]->printHand();
                    #endif
                    playedCard = true;
                    net[curId]--;
                    if (net[curId] == 0) personalContract = --stage[curId];
                    played[curId]++;
                    if (isWild(c)) {
                        contract = 0;
                        discard.push(c);
                        curSuit = player[cur]->setSuit();
                        burning = false;
                        #ifndef quiet
                            cout << " Suit is ";
                            Card::printSuit(curSuit, cout);
                            cout << endl;
                        #endif
                    } else if (c.getSuit() == curSuit
                      || (!burning && c.matchesRank(discard.top()))
                      || (isDrawFive(discard.top()) && isCancel(c))) {
                        if (contract > 0) {
                            if (isDrawTwo(c)) {
                                nextContract = contract + 2;
                                contract = 0;
                            } else if (isDrawFive(c)) {
                                nextContract = contract + 5;
                                contract = 0;
                            } else if (isCancel(c)) {
                                contract = 0;
                            } else {
                                disqualify(cur, "Illegal card played when under contract.");
                            }
                        } else {
                            if (isDrawTwo(c)) nextContract += 2;
                            else if (isDrawFive(c)) nextContract += 5;
                            if (isReverser(c)) incr = -incr;
                            if (isBurner(c)) burning = true;
                            if (isSkipper(c)) incr += (incr > 0) ? 1 : -1;
                        }
                        discard.push(c);
                        curSuit = c.getSuit();
                        #ifndef quiet
                            cout << endl;
                        #endif
                    } else {
                        disqualify(cur, "Non-matching card played.");
                    }
                    if (net[cur] < 0) {
                        disqualify(cur, "Too many cards played.");
                    }
                    break;
                case Win:
                    // The player claims that he or she has won!
                    if (stage[curId] != 0) {
                        disqualify(cur, "Not all stages completed.");
                        opt = Done;
                    } else if (net[curId] != 0) {
                        disqualify(cur, "Player still has cards.");
                        opt = Done;
                    } else if (contract > 0) {
                        disqualify(cur, "Still under contract.");
                        opt = Done;
                    } else if (personalContract > 0) {
                        disqualify(cur, "Must draw replacement cards this turn.");
                        opt = Done;
                    } else if (!playedCard) {
                        disqualify(cur, "Never played a card.");
                        opt = Done;
                    }
                    winner = cur;
                    burning = false;
                    break;
                case Done:
                    // The player is done with his or her turn.
                    if (contract > 0) {
                        disqualify(cur, "Still under contract.");
                    } else if (personalContract > 0) {
                        disqualify(cur, "Must draw replacement cards this turn.");
                    } else if (!playedCard && !fillingContract) {
                        disqualify(cur, "Passing is not allowed.");
                    }
                    burning = false;
                    for (int i=0; i<player.size(); i++) {
                        if (i == cur) continue;
                        player[i]->inform(player[cur]->getId(), stage[i], net[i]);
                    }
                    break;
            }
        }
        nextPlayer();
    }

    // Display interesting statistics from the game, and return the winner.
    cout << "Player " << player[winner]->getId() << " (" << player[winner]->getName()
         << ") wins!" << endl;
    cout << "Cards played:\n";
    int totCards = 0;
    for (int i=0; i<player.size(); i++) {
        totCards += played[player[i]->getId()];
        cout << " Card player " << player[i]->getId() << " (" << player[i]->getName()
             << "): " << played[player[i]->getId()] << endl;
    }
    cout << "Total number of cards played: " << totCards << ".\n"
         << "Cards played by winner: " << played[player[winner]->getId()] << "\n"
         << "Average number of cards played per player: "
         << static_cast<double>(totCards)/player.size() << "." << endl;
    return player[winner]->getId();
}

/*
 * Determines which player is next by using the value of incr, and sets
 * the value of cur to the next player number, and curId to that player's id.
 * The sign of incr tells which direction play is proceeding; the absolute
 * value of incr tells the distance to jump.
 */
void ShedGame::nextPlayer() {
    cur += incr;
    
    //loop around for both sides. 
    if (cur > player.size()-1) cur = 0;
    else if (cur < 0) cur = (int)player.size() - 1;
    
    curId = player[cur]->getId();
}

/*
 * A player has done something illegal and is disqualified!
 * Remove him or her from the match.
 */
void ShedGame::disqualify(int idx, const string& msg) {
    Player* p = player[idx];
    cerr << "Player " << p->getId() << " (" << p->getName()
         << ") disqualified!  " << msg << ".\n";
    for (int i=0; i<player.size(); i++) player[i]->disqualified(p->getId());
    p->reset();
    delete p;
    player.erase(player.begin() + idx);
    net.erase(net.begin() + p->getId());
    played.erase(played.begin() + p->getId());
    stage.erase(stage.begin() + p->getId());
}

/*
 * This function should take all the cards from the discard pile
 * except for the top card, place them in the stock, and then
 * shuffle the stock.
 */
void ShedGame::restock() {
    //retrieve the top card and remove it from the array.
    Card topCard = stock.back();
    stock.pop_back();
    
    //loop through discard stack, and put every card into stock.
    //empty discard stack.
    while (!discard.empty()) {
        stock.push_back(discard.top());
        discard.pop();
    }
    
    random_shuffle(stock.begin(), stock.end());
    
    //replace top card that was taken at start of the function.
    stock.push_back(topCard);
}
