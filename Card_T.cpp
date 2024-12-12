/*
 * File: Card_T.h
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the implementation of the abstract template Card class and its methods
 */

#include "Card_T.h"

template <typename Rank, typename Suit>
Card<Rank, Suit>::Card(Rank _rank, Suit _suit) : rank(_rank), suit(_suit) {}

template <typename Rank, typename Suit>
std::ostream& operator<<(std::ostream& os, const Card<Rank, Suit>& card) {
    return os << card.rank << card.suit;
}

template <typename Rank, typename Suit>
bool compareByRankThenSuit(const Card<Rank, Suit>& card1, const Card<Rank, Suit>& card2) {
    if (card1.rank < card2.rank) {
        return true;
    } else if (card1.rank == card2.rank) {
        return card1.suit < card2.suit;
    }
    return false;
}

template <typename Rank, typename Suit>
bool compareBySuitThenRank(const Card<Rank, Suit>& card1, const Card<Rank, Suit>& card2) {
    if (card1.suit < card2.suit) {
        return true;
    } else if (card1.suit == card2.suit) {
        return card1.rank < card2.rank;
    }
    return false;
}