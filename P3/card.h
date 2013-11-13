// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#ifndef card_h
#define card_h

#include <iostream>
#include <vector>

using namespace std;

class Card {
  public:
    // Enumerations for representing suits and ranks.  Way better in C++11!!
    enum Suit {spades, hearts, diamonds, clubs};
    enum Rank {ace, two, three, four, five, six, seven, eight, nine, ten,
               jack, queen, king};
    
    Card();  // Default is two of clubs.
    Card(Rank, Suit);

    bool matchesSuit(const Card& c) const;
    bool matchesRank(const Card& c) const;
    Rank getRank() const;
    Suit getSuit() const;

    // Operators
    bool operator == (const Card&) const;
    bool operator != (const Card&) const;
    friend ostream& operator << (ostream&, const Card&);

    // Helpful for iterating over suits and ranks.
    static const int numSuits = 4;
    static const int numRanks = 13;
    static Suit suits[];
    static Rank ranks[];

    static void printSuit(Suit, ostream&);

  private:
    Rank rank;
    Suit suit;
};

#endif
