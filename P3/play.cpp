//// CSC 322 Programming Languages
//// M. J. Mossinghoff
//// Davidson College
//
//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//#include <stdexcept>
//#include <string>
//#include "mjmplayer.h"
//#include "crazy8.h"
//#include "switch.h"
//#include "blackjack.h"
//
//using namespace std;
//
//enum Game {CrazyEights, IrishSwitch, Black_Jack};
//Game selectGame();
//int getNumPlayers();
//int getNumIterations();
//void play(ShedGame&, int, int);
//int getPlayer(int);
//string getName(int);
//
///*
// * Main routine for playing shed games.
// */
//int main() {
//    // Seed the random number generator.
//    srand(static_cast<unsigned>(time(NULL)));
//    Game g = selectGame();
//    int num = getNumPlayers();
//    int N = getNumIterations();
//    if (g == CrazyEights) {
//        Crazy8 crazy8;
//        play(crazy8, num, N);
//    } else if (g == IrishSwitch) {
//        Switch swatch;
//        play(swatch, num, N);
//    } else if (g == Black_Jack) {
//        BlackJack blackjack;
//        play(blackjack, num, N);
//    }
//    return 0;
//}
//
///*
// * Play the selected game g with M players, N times.
// */
//void play(ShedGame& g, int M, int N) {
//    for (int i=0; i<M; i++) {
//        int ch = getPlayer(i);
//        string name = getName(i);
//        // Add a case here for each player object you want to support.
//        switch (ch) {
//            case 1:
//                // A player that I wrote (code not available!).
//                g.addPlayer(new MJMPlayer(g, name));
//                break;
//            default:
//                throw logic_error("Error: invalid choice received");
//                break;
//        }
//    }
//
//    cout << "\nPlaying " << g.getName() << "\n"
//         << " Players: " << g.getPlayer(0)->getName();
//    for (int i=1; i<g.numPlayers(); i++) {
//        cout << ", " << g.getPlayer(i)->getName();
//    }
//    cout << "." << endl;
//
//    int* wins = new int[g.numPlayers()];
//    long long* discards = new long long[g.numPlayers()];
//    for (int i=0; i<g.numPlayers(); i++) discards[i] = wins[i] = 0;
//    long long winnerDiscards = 0LL;
//
//    for (int i=0; i<N; i++) {
//        cout << "\nGame " << i+1 << ".\n" << endl;
//        g.reset();
//        int winner = g.play();  // Returns id of winner.
//        wins[winner]++;
//        winnerDiscards += g.getCardsPlayed(winner);
//        for (int j=0; j<g.numPlayers(); j++)
//            discards[g.getPlayer(j)->getId()] += g.getCardsPlayed(g.getPlayer(j)->getId());
//    }
//
//    cout.precision(4);
//    cout << "\nTotal number of wins:\n";
//    for (int i=0; i<g.numPlayers(); i++) {
//        cout << " Player " << g.getPlayer(i)->getId()
//		     << " (" << g.getPlayer(i)->getName() << ") : "
//		     << wins[g.getPlayer(i)->getId()] << " ("
//		     << static_cast<double>(wins[g.getPlayer(i)->getId()])/N*100 << "%)\n";
//    }
//
//    cout << "\nAverage number of cards played per game:\n";
//    for (int i=0; i<g.numPlayers(); i++) {
//        cout << " Player " << g.getPlayer(i)->getId()
//		     << " (" << g.getPlayer(i)->getName() << "): "
//		     << static_cast<double>(discards[g.getPlayer(i)->getId()])/N << ".\n";
//    }
//    cout << "Average number of cards played per game by winner: "
//         << static_cast<double>(winnerDiscards)/N << "." << endl;
//
//    delete [] wins;
//    delete [] discards;
//}
//
///*
// * Allow the user to select which shedding game he or she would like to play.
// */
//Game selectGame() {
//    int resp;
//    do {
//        cout << "\nSelect the game we're playing.\n"
//             << " 1. Crazy Eights.\n"
//             << " 2. Irish Switch.\n"
//             << " 3. Black Jack.\n"
//             << "Your choice: ";
//        cin >> resp;
//        if (resp < 0 || resp > 3)
//            cout << "Error: invalid choice.  Try again.\n";
//    } while (resp < 0 || resp > 3);
//    Game ans;
//    switch (resp) {
//        case 1: ans = CrazyEights; break;
//        case 2: ans = IrishSwitch; break;
//        case 3: ans = Black_Jack; break;
//    }
//    return ans;
//}
//
///*
// * Get the number of players from the user.
// */
//int getNumPlayers() {
//    int ans;
//    do {
//        cout << "\nNumber of players? ";
//        cin >> ans;
//        if (ans <= 1) cout << "You must have at least two players.  ";
//    } while (ans <= 1);
//    return ans;
//}
//
///*
// * Get the number of times the user would like to play the game.
// */
//int getNumIterations() {
//    int ans;
//    do {
//        cout << "\nNumber of iterations? ";
//        cin >> ans;
//        if (ans <= 0) cout << "You must play at least one game.  ";
//    } while (ans <= 0);
//    return ans;
//}
//
///*
// * Select a player.  Add an option here for each player class that you want
// * to support.
// */
//int getPlayer(int k) {
//    int ans;
//    const int Choices = 1;
//    cout << "\nChoose player " << k << ":\n";
//    do {
//        cout << " 1. Dr. M!\n"
//             << "Enter choice: ";
//        cin >> ans;
//        if (ans <= 0 || ans > Choices) cout << "Invalid choice, try again.\n";
//    } while (ans <= 0 || ans > Choices);
//    return ans;
//}
//
///*
// * Get the name of a player.
// */
//string getName(int k) {
//    const int BufferSize = 256;
//    char line[BufferSize];
//    string ans;
//    cout << "Enter name of player " << k << ": ";
//    cin.getline(line, BufferSize);  // Clear buffer first.
//    cin.getline(line, BufferSize);  // Now read the name.
//    return string(line);
//}
