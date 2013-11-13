// CSC 322 Programming Languages
// M. J. Mossinghoff
// Davidson College

#include <iostream>
#include "card.h"

using namespace std;

Card::Card() : rank(two), suit(clubs) {}
Card::Card(Card::Rank r, Card::Suit s) : rank(r), suit(s) {}

Card::Rank Card::getRank() const { return rank; }
Card::Suit Card::getSuit() const { return suit; }

bool Card::matchesSuit(const Card& c) const {
    return suit == c.suit;
}

bool Card::matchesRank(const Card& c) const {
    return rank == c.rank;
}

bool Card::operator == (const Card& rhs) const {
    return matchesRank(rhs) && matchesSuit(rhs);
}

bool Card::operator != (const Card& rhs) const {
    return !operator==(rhs);
}

ostream& operator << (ostream& out, const Card& c) {
    switch (c.rank) {
        case Card::ace: out << "A"; break;
        case Card::two: out << 2; break;
        case Card::three: out << 3; break;
        case Card::four: out << 4; break;
        case Card::five: out << 5; break;
        case Card::six: out << 6; break;
        case Card::seven: out << 7; break;
        case Card::eight: out << 8; break;
        case Card::nine: out << 9; break;
        case Card::ten: out << "T"; break;
        case Card::jack: out << "J"; break;
        case Card::queen: out << "Q"; break;
        case Card::king: out << "K"; break;
    }
    Card::printSuit(c.suit, out);
    return out;
}

void Card::printSuit(Suit s, ostream& out) {
    switch (s) {
        case Card::spades: out << "\u2660"; break;
        case Card::hearts: out << "\u2661"; break;
        case Card::diamonds: out << "\u2662"; break;
        case Card::clubs: out << "\u2663"; break;
    }
}

// Initialize the static arrays.
Card::Suit Card::suits[] = {spades, hearts, diamonds, clubs};
Card::Rank Card::ranks[] = {ace, two, three, four, five, six, seven,
                            eight, nine, ten, jack, queen, king};
