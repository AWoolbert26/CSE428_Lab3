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
#include <type_traits>  // For std::is_base_of
#include "CardSet_T.h"
#include "Deck_T.h" 
#include "Suit.h"
#include "HoldEmDeck.h"
#include "PinochleDeck.h"
#include "UnoDeck.h"
#include "Constants.h"
#include "Game.h"

// Ensure that DeckType is derived from CardSet<Suit, Rank>
template <typename Suit = Suit, typename Rank = HoldEmRank, typename DeckType = HoldEmDeck>
class GoFishGame: public Game {
    static_assert(std::is_base_of<CardSet<Suit, Rank>, DeckType>::value, 
                  "DeckType must be derived from CardSet<Suit, Rank>");

public:
    GoFishGame(int numPlayers, const std::array<const char*, MAX_NUM_GOFISH_PLAYERS>& playerNames);
    virtual ~GoFishGame() = default;
    virtual void play();
    bool collect_books(int playerNumber);

protected:
    DeckType deck;
    std::vector<CardSet<Suit, Rank>> playerHands;
    std::vector<CardSet<Suit, Rank>> books;

    bool turn(int playerNumber);
    virtual void deal();

private:
    void validateDeck();
};

template <>
GoFishGame<Suit, HoldEmRank, HoldEmDeck>::GoFishGame(int numPlayers, const std::array<const char*, MAX_NUM_GOFISH_PLAYERS>& playerNames);

template <>
GoFishGame<Suit, PinochleRank, PinochleDeck>::GoFishGame(int numPlayers, const std::array<const char*, MAX_NUM_GOFISH_PLAYERS>& playerNames);

template <>
GoFishGame<Color, UnoRank, UnoDeck>::GoFishGame(int numPlayers, const std::array<const char*, MAX_NUM_GOFISH_PLAYERS>& playerNames);


