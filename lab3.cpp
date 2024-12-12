/*
 * File: lab3.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the main function.
 */

#include <iostream>
#include <memory>
#include <cstring>
#include "HoldEmDeck.h"
#include "PinochleDeck.h"
#include "Suit.h"
#include "PinochleGame.h"
#include "HoldEmGame.h"
#include "GoFishGame_T.h"
#include "Game.h"
#include "UnoDeck.h"
#include <stdexcept>

std::shared_ptr<Game> create(int argc, const char *argv[]) {
    // Error checking is done before hand so we don't have to do it in the create method.
    std::shared_ptr<Game> game_ptr;

    std::string game_name = argv[GAME_NAME];

    if (game_name == "Pinochle") {
        game_ptr = std::make_shared<PinochleGame>(argc, argv);
    } else if (game_name == "HoldEm") {
        game_ptr = std::make_shared<HoldEmGame>(argc, argv);
    } else if (game_name == "GoFish") {
        std::string deckType = argv[GOFISH_DECK_TYPE_POS];
        if (deckType == "HoldEmDeck") {
            game_ptr = std::make_shared<GoFishGame<Suit, HoldEmRank, HoldEmDeck> >(argc, argv);
        } else if (deckType == "PinochleDeck") {
            game_ptr = std::make_shared<GoFishGame<Suit, PinochleRank, PinochleDeck> >(argc, argv);
        } else if (deckType == "UnoDeck") {
            game_ptr = std::make_shared<GoFishGame<Color, UnoRank, UnoDeck> >(argc, argv);
        }
    }

    return game_ptr;
}

bool checkIfAllPlayerNamesAreUnique(int argc, const char *argv[]) {
    for (int i = PLAYER_NAMES_START; i < argc; ++i) {
        for (int j = i + 1; j < argc; ++j) {
            if (strcmp(argv[i], argv[j]) == 0) {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < REQUIRED_ARGS) {
        std::cerr << "Usage: " << argv[EXECUTABLE] << " <GameType> <PlayerNames...>\n";
        std::cerr << "GameType: Pinochle <Player1> <Player2> <Player3> <Player4>\n";
        std::cerr << "         or HoldEm <Player1> <Player2> [<Player3> ... <Player9>]\n";
        std::cerr << "         or GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player9>]\n";
        return USAGE_WRONG_NUM_ARGS_ERROR;
    }

    std::string game_name = argv[GAME_NAME];
    std::shared_ptr<Game> game_ptr;

    if (game_name == "Pinochle") {
        if (argc != PINOCHLE_INPUT_VARS) {
            std::cerr << "Wrong number of players, Pinochle requires exactly four players.\n";
            std::cerr << "Usage: " << argv[EXECUTABLE] << " Pinochle <Player1> <Player2> <Player3> <Player4>\n";
            return USAGE_PINOCHLE_ERROR;
        }
        if (!checkIfAllPlayerNamesAreUnique(argc, argv)) {
            std::cerr << "In Pinochle, players can't have the same names.\n";
            std::cerr << "All player names should be unique.\n";
            return PLAYERS_HAVE_SAME_NAMES;
        }
        game_ptr = create(argc, argv);
    } else if (game_name == "HoldEm") {
        if (argc < HOLDEM_MIN_INPUT_VARS || argc > HOLDEM_MAX_INPUT_VARS) {
            std::cerr << "Wrong number of players, HoldEm requires from 2 to 9 players.\n";
            std::cerr << "Usage: " << argv[EXECUTABLE] << " HoldEm <Player1> <Player2> [<Player3> ... <Player9>]\n";
            return USAGE_HOLDEM_ERROR;
        }
        if (!checkIfAllPlayerNamesAreUnique(argc, argv)) {
            std::cerr << "In HoldEm, players can't have the same names.\n";
            std::cerr << "All player names should be unique.\n";
            return PLAYERS_HAVE_SAME_NAMES;
        }
        game_ptr = create(argc, argv);
    } else if (game_name == "GoFish") {
        if (argc < MIN_NUM_GOFISH_VARS || argc > MAX_NUM_GOFISH_VARS) {
            std::cerr << "Wrong number of players, GoFish requires from 2 to 5 players.\n";
            std::cerr << "Usage: " << argv[EXECUTABLE] << " GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player5>]\n";
            return USAGE_HOLDEM_ERROR;
        }
        if (!checkIfAllPlayerNamesAreUnique(argc, argv)) {
            std::cerr << "In GoFish, players can't have the same names.\n";
            std::cerr << "All player names should be unique.\n";
            return PLAYERS_HAVE_SAME_NAMES;
        }
        game_ptr = create(argc, argv);
    } else {
        std::cerr << "Unknown game type: " << game_name << "\n";
        std::cerr << "Usage: " << argv[EXECUTABLE] << " <game_name> <PlayerNames...>\n";
        return USAGE_UKNOWN_GAME_ERROR;
    }

    if (game_ptr) {
        try {
            return game_ptr->play(); 
        } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
            return GAME_PLAY_FAILURE;
        }
    } else {
        std::cerr << "Error: Failed to create the game.\n";
        return GAME_CREATION_ERROR; 
    }
    return SUCCESS;
}