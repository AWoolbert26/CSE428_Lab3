/*
 * File: GoFishGame.h
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the declarations related to the GoFishGame class
 */
#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <type_traits> 
#include "CardSet_T.h"
#include "Deck_T.h" 
#include "Suit.h"
#include "HoldEmDeck.h"
#include "PinochleDeck.h"
#include "UnoDeck.h"
#include "Constants.h"
#include "Game.h"


template <typename Suit = Suit, typename Rank = HoldEmRank, typename Deck = HoldEmDeck>
class GoFishGame: public Game {
    static_assert(std::is_base_of_v<CardSet<Rank, Suit>, Deck>, "DeckType must be derived from CardSet<Rank, Suit>");

public:
    GoFishGame(int argc, const char* argv[]);
    virtual ~GoFishGame() = default;
    virtual int play();
    bool collect_books(int player_number);

protected:
    Deck deck;
    std::vector<CardSet<Rank, Suit>> player_hands;
    std::vector<CardSet<Rank, Suit>> books;
    std::vector<std::string> removed_player_names;

    bool turn(int playerNumber);
    virtual void deal();

    // Helper functions
    void printGameState(int);
    int checkIfValidDeck(Deck, Rank, Rank);
    bool getValidRequestOrGoFish(int);
    void collectBooksForAllPlayers();
    bool gameNotOver();
    void printRoundInfo(int);
    void printWinner();
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "GoFishGame_T.cpp"
#endif