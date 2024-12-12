/*
 * File: Card_T.h
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the declaration of the abstract template Card class and its methods
 */

#pragma once

#include <iostream>

template <typename Rank, typename Suit>
struct Card {
    Rank rank;
    Suit suit;

    Card(Rank, Suit);
};

template <typename Rank, typename Suit>
std::ostream& operator<<(std::ostream&, const Card<Rank, Suit>&);

template <typename Rank, typename Suit>
bool compareByRankThenSuit(const Card<Rank, Suit>&, const Card<Rank, Suit>&);

template <typename Rank, typename Suit>
bool compareBySuitThenRank(const Card<Rank, Suit>&, const Card<Rank, Suit>&);


#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Card_T.cpp"
#endif