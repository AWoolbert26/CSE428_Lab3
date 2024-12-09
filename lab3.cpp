/*
 * File: lab1.cpp
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
#include "UnoDeck.h"
#include "Suit.h"
#include "PinochleGame.h"
#include "HoldEmGame.h"
#include "GoFishGame.h"
#include "Game.h"
#include <stdexcept>

std::shared_ptr<Game> create(int argc, const char *argv[]) {
    // Error checking is done beforehand, so we don't have to do it here.
    std::shared_ptr<Game> game_ptr;

    std::string game_name = argv[GAME_NAME];
    if (game_name == "Pinochle") {
        game_ptr = std::make_shared<PinochleGame>(argc, argv);
    } else if (game_name == "HoldEm") {
        game_ptr = std::make_shared<HoldEmGame>(argc, argv);
    } else if (game_name == "GoFish") {
        // Check if the next argument specifies the deck type (e.g., "HoldEm", "Pinochle", or "Uno")
        if (argc > GAME_NAME + 1) {
            std::string deck_type = argv[GAME_NAME + 1];
            if (deck_type == "HoldEm") {
                // Ensure we have enough arguments for player names
                if (argc >= PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS) {
                    std::array<const char*, MAX_NUM_GOFISH_PLAYERS> player_names;
                    int num_players = argc - (PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS);
                    for (int i = 0; i < num_players; ++i) {
                        player_names[i] = argv[PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS + i];
                    }

                    try {
                        // Create the GoFishGame with the appropriate types
                        game_ptr = std::make_shared<GoFishGame<Suit, HoldEmRank, HoldEmDeck> >(num_players, player_names);
                    } catch (const std::exception& e) {
                        std::cerr << "Error creating GoFishGame: " << e.what() << std::endl;
                        return nullptr; // Return nullptr or handle as needed
                    }
                }
            } else if (deck_type == "Pinochle") {
                if (argc >= PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS) {
                    std::array<const char*, MAX_NUM_GOFISH_PLAYERS> player_names;
                    int num_players = argc - (PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS);
                    for (int i = 0; i < num_players; ++i) {
                        player_names[i] = argv[PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS + i];
                    }

                    try {
                        game_ptr = std::make_shared<GoFishGame<Suit, PinochleRank, PinochleDeck> >(num_players, player_names);
                    } catch (const std::exception& e) {
                        std::cerr << "Error creating GoFishGame: " << e.what() << std::endl;
                        return nullptr;
                    }
                }
            } else if (deck_type == "Uno") {
                if (argc >= PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS) {
                    std::array<const char*, MAX_NUM_GOFISH_PLAYERS> player_names;
                    int num_players = argc - (PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS);
                    for (int i = 0; i < num_players; ++i) {
                        player_names[i] = argv[PLAYER_NAMES_START + NON_PLAYER_NAME_GOFISH_INPUTS + i];
                    }

                    try {
                        game_ptr = std::make_shared<GoFishGame<Color, UnoRank, UnoDeck> >(num_players, player_names);
                    } catch (const std::exception& e) {
                        std::cerr << "Error creating GoFishGame: " << e.what() << std::endl;
                        return nullptr;
                    }
                }
            } else {
                std::cerr << "Invalid deck type specified. Supported types are: HoldEm, Pinochle, Uno." << std::endl;
                return nullptr;
            }
        } else {
            std::cerr << "Deck type not specified for GoFish." << std::endl;
            return nullptr;
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