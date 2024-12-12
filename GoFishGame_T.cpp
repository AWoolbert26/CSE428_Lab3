/*
 * File: GoFishGame.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the definitions of the GoFishGame class.
 */

#include "Deck_T.h"
#include "GoFishGame_T.h"
#include "Constants.h"
#include "HoldEmDeck.h"
#include "PinochleDeck.h"
#include "UnoDeck.h"
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

/* 
    GoFishGame constructor using a HoldEmDeck
*/
template<>
GoFishGame<Suit, HoldEmRank, HoldEmDeck>::GoFishGame(int argc, const char* argv[]): Game(argc, argv) {
    HoldEmRank first_rank = HoldEmRank::two;
    HoldEmRank final_rank = HoldEmRank::undefined;

    if (checkIfValidDeck(deck, first_rank, final_rank) != SUCCESS) {
        throw std::runtime_error("Deck does not have at least 4 cards of each rank.");
    }

    for (size_t i = 0; i < player_names.size(); ++i) {
       player_hands.emplace_back();
    }

    books.resize(player_names.size());
}


/* 
    GoFishGame constructor using a PinochleDeck
*/
template<>
GoFishGame<Suit, PinochleRank, PinochleDeck>::GoFishGame(int argc, const char* argv[]): Game(argc, argv) {
    PinochleRank first_rank = PinochleRank::nine;
    PinochleRank final_rank = PinochleRank::undefined;

    if (checkIfValidDeck(deck, first_rank, final_rank) != SUCCESS) {
        throw std::runtime_error("Deck does not have at least 4 cards of each rank.");
    }

    for (size_t i = 0; i < player_names.size(); ++i) {
        player_hands.emplace_back();
    }

    books.resize(player_names.size());
}


/* 
    GoFishGame constructor using an UnoDeck
*/
template<>
GoFishGame<Color, UnoRank, UnoDeck>::GoFishGame(int argc, const char* argv[]): Game(argc, argv) {
    UnoRank first_rank = UnoRank::zero;
    UnoRank final_rank = UnoRank::undefined;

    if (checkIfValidDeck(deck, first_rank, final_rank) != SUCCESS) {
        throw std::runtime_error("Deck does not have at least 4 cards of each rank.");
    }

    for (size_t i = 0; i < player_names.size(); ++i) {
        player_hands.emplace_back();
    }

    books.resize(player_names.size());
}

/* 
    Helper function to determine if a deck is valid for GoFish
*/
template<typename Suit, typename Rank, typename Deck>
int GoFishGame<Suit, Rank, Deck>::checkIfValidDeck(Deck deck, Rank current_rank, Rank final_rank) {
    do {
        int count = std::count_if(deck.begin(), deck.end(), [current_rank](const auto& card) {
            return card.rank == current_rank;
        });

        if (count < 4) {
            return WRONG_NUM_OF_CARDS_PER_RANK;
        }

        ++current_rank;
    } while (current_rank != final_rank);

    return SUCCESS;
} 

/* 
    Helper function to print current game state
*/
template<typename Suit, typename Rank, typename Deck>
void GoFishGame<Suit, Rank, Deck>::printGameState(int player_number)  {
    // Display the current state of all players' hands and books
    std::cout << "Current Game State:\n";
    std::cout << "It is Player " << player_number << "'s turn:" << std::endl;
    for (size_t i = 0; i < player_hands.size(); ++i) {
        std::cout << "Player " << i << " (" << player_names[i] << "):\n";
        std::cout << "  Hand: ";
        player_hands[i].print(std::cout, 13);
        std::cout << "  Books: ";
        books[i].print(std::cout, 13);
        std::cout << "\n";
    }
}


/* 
    Helper function to get a valid request
    Returns: true if the request returns at least one card
             false if the request returns no cards
*/
template<typename Suit, typename Rank, typename Deck>
bool GoFishGame<Suit, Rank, Deck>::getValidRequestOrGoFish(int player_number) {
    CardSet<Rank, Suit>& hand = player_hands[player_number];
    bool rank_is_not_valid = true;
    std::string entered_rank;
    Rank requested_rank;

    bool selected_player_number_is_not_valid = true;
    int selected_player_number;

    bool cards_in_hand = true;

    if (hand.get_size() == 0) {
        cards_in_hand = false;
        entered_rank = "undefined";
        requested_rank = Deck::fromString(entered_rank);
    }

    while (rank_is_not_valid && cards_in_hand) {

        std::cout << "Enter a rank (e.g., zero, one, two, skip, etc.): ";
        std::cin >> entered_rank;

        requested_rank = Deck::fromString(entered_rank);

        int count = std::count_if(hand.begin(), hand.end(), [requested_rank](const auto& card) {
            return card.rank == requested_rank;
        });

        if (count != 0) {
            rank_is_not_valid = false;  
        } else {
            std::cout << "You don't have any cards of that rank. Try again." << std::endl;
        }
    }

    while (selected_player_number_is_not_valid && cards_in_hand) {
        std::cout << "Enter a player number (e.g., 0, 1, 2, 3, etc.): ";
        std::cin >> selected_player_number;

        if (selected_player_number == player_number || (size_t) selected_player_number >= player_hands.size()) {
            std::cout << "Invalid player number. Select a another." << std::endl;
            std::cin.clear();
        } else {
            selected_player_number_is_not_valid = false;
        }
    }

    if (cards_in_hand && hand.request(player_hands[selected_player_number], requested_rank)) {
        while (collect_books(player_number)) {
            continue;
        }
        return true;
    } else {
        if (deck.get_size() == 0) {
            deck.collect(hand);
            std::cout << player_names[player_number] << " removed from the game." << std::endl;
            removed_player_names.push_back(player_names[player_number]);
            return false;
        } else {
            Card<Rank, Suit> drawn_card = deck.get_top_card();
            deck >> hand;
            if (drawn_card.rank == requested_rank) {
                return true;
            } else {
                return false;
            }
        }
    }    
} 


/* 
    GoFishGame collect_books method
*/
template <typename Suit, typename Rank, typename Deck>
bool GoFishGame<Suit, Rank, Deck>::collect_books(int player_number) {

    CardSet<Rank, Suit>& player_hand = player_hands[player_number];

    for (const auto& card : player_hand) {
        Rank rank = card.rank;
        int count = std::count_if(player_hand.begin(), player_hand.end(), [rank](const Card<Rank, Suit>& c) {
            return c.rank == rank;
        });

        if (count == 4) {
            auto predicate = [rank](Card<Rank, Suit>& c) {
                return c.rank == rank;
            };

            books[player_number].collect_if(player_hand, predicate);
            return true;  
        }
    }

    return false;  
}


/* 
    GoFishGame turn method
*/
template <typename Suit, typename Rank, typename Deck>
bool GoFishGame<Suit, Rank, Deck>::turn(int player_number) {

    printGameState(player_number);
    if (getValidRequestOrGoFish(player_number)) {
        return true;
    } else {
        return false;
    }
}


/* 
    GoFishGame deal method
*/
template <typename Suit, typename Rank, typename Deck>
void GoFishGame<Suit, Rank, Deck>::deal() {
    deck.shuffle();

    int num_players = player_names.size();

    if (num_players == 2) {
        for (int i = 0; i < CARDS_FOR_2_GOFISH_PLAYERS; ++i) {
            for (auto& hand : player_hands) {
                deck >> hand;
            }
        }
    } else if (num_players > 2){
        for (int i = 0; i < CARDS_FOR_MORE_THAN_2_GOFISH_PLAYERS; ++i) {
            for (auto& hand : player_hands) {
                deck >> hand;
            }
        }
    }
    return;
}


/* 
    GoFishGames play method
*/
template <typename Suit, typename Rank, typename Deck>
int GoFishGame<Suit, Rank, Deck>::play() {
    deal();
    collectBooksForAllPlayers();

    int round_number = 1;

    while (gameNotOver()) {
        for (size_t i = 0; i < player_names.size(); ++i) {
            if (std::find(removed_player_names.begin(), removed_player_names.end(), player_names[i]) != removed_player_names.end()) {
                break;
            } else {
                while(turn(i));
            }
        }

        printRoundInfo(round_number);
    }

    // printWinner();
    return SUCCESS; 
}


/* 
    Helper function that collects books for all players
*/
template <typename Suit, typename Rank, typename Deck>
void GoFishGame<Suit, Rank, Deck>::collectBooksForAllPlayers() {
    for (size_t i = 0; i < player_names.size(); ++i) {
        while (collect_books(i));
    }
}


/* 
    Helper function that checks if the game is over
*/
template <typename Suit, typename Rank, typename Deck>
bool GoFishGame<Suit, Rank, Deck>::gameNotOver() {

    // Check if there are one or no players left
    if (removed_player_names.size() - player_names.size() < 2) {
        return false;
    }

    // Check if all cards have been moved into books
    if (deck.get_size() == 0) {
        for (size_t i = 0; i < player_names.size(); ++i) {
            if (player_hands[i].get_size() != 0) {
                return true;
            }
        }
        return false;
    } 

    return true;
}

/* 
    Helper function that checks if the game is over
*/
template <typename Suit, typename Rank, typename Deck>
void GoFishGame<Suit, Rank, Deck>::printRoundInfo(int round_number) {
    std::cout << "Round Number: " << round_number << std::endl;

    for (size_t i = 0; i < player_names.size(); ++i) {
        std::cout << player_names[i] << " has made: " << books[i].get_size() / 4 << "books" << std::endl;
    } 
}


/* 
    Helper function that prints the winner/s and the number of books they had!
*/
template <typename Suit, typename Rank, typename Deck>
void GoFishGame<Suit, Rank, Deck>::printWinner() {
    int max_books = 0;

    for (size_t i = 0; i < player_names.size(); ++i) {
        int player_books = books[i].get_size() / 4;
        if (player_books > max_books) {
            max_books = player_books;
        }
    }

    std::cout << "The winners are: ";
    for (size_t i = 0; i < player_names.size(); ++i) {
        int player_books = books[i].get_size() / 4;
        if (player_books == max_books) {
            std::cout << player_names[i] << " with " << max_books << " books ";
        }
    }

    std::cout << std::endl;
}