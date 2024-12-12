/*
 * File: PinochleDeck.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the definitions related to the pinochle deck and its class
 */

#include "PinochleDeck.h"

// Inserts pinochle rank representation on ostream
std::ostream& operator<<(std::ostream& os, const PinochleRank& rank) {
    switch (rank) {
        case PinochleRank::nine:
            os << '9';
            break;
        case PinochleRank::jack:
            os << 'J';
            break;
        case PinochleRank::queen:
            os << 'Q';
            break;
        case PinochleRank::king:
            os << 'K';
            break;
        case PinochleRank::ten:
            os << "10";
            break;
        case PinochleRank::ace:
            os << 'A';
            break;
        default:
            os << '?';
    }
    return os;
}

// Increments pinochle rank by one
PinochleRank& operator++(PinochleRank& rank) {
    switch (rank) {
        case PinochleRank::nine:
            rank = PinochleRank::jack;
            break;
        case PinochleRank::jack:
           rank = PinochleRank::queen;
           break;
        case PinochleRank::queen:
            rank = PinochleRank::king;
            break;
        case PinochleRank::king:
            rank = PinochleRank::ten;
            break;
        case PinochleRank::ten:
            rank = PinochleRank::ace;
            break;
        default:
            rank = PinochleRank::undefined;
    }
    return rank;
}

PinochleRank PinochleDeck::fromString(const std::string& str) {
    if (str == "nine" || str == "9") return PinochleRank::nine;
    if (str == "jack" || str == "J") return PinochleRank::jack;
    if (str == "queen" || str == "Q") return PinochleRank::queen;
    if (str == "king" || str == "K") return PinochleRank::king;
    if (str == "ten" || str == "10") return PinochleRank::ten;
    if (str == "ace" || str == "A") return PinochleRank::ace;

    // If the string doesn't match any valid rank, return undefined
    return PinochleRank::undefined;
}

// Constructor for the pinochle deck
PinochleDeck::PinochleDeck() {
    for (Suit s = Suit::clubs; s != Suit::undefined; ++s) {
        for (PinochleRank r = PinochleRank::nine; r != PinochleRank::undefined; ++r) {
            this->cards.push_back({r, s});
            this->cards.push_back({r, s});
        }
    }
}