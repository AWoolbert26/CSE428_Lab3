/*
 * File: GoFishGame.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the definitions of the GoFishGame class.
 */
#include "GoFishGame.h"
#include "Constants.h"
#include <algorithm>

template <typename Suit, typename Rank, typename DeckType>
GoFishGame<Suit, Rank, DeckType>::GoFishGame(int numPlayers, const std::array<const char*, MAX_NUM_GOFISH_PLAYERS>& playerNames) {
    validateDeck();

    playerHands.resize(numPlayers);
    books.resize(numPlayers);

    deal();
}

template <typename Suit, typename Rank, typename DeckType>
void GoFishGame<Suit, Rank, DeckType>::validateDeck() {
    auto cardCounts = std::vector<int>(static_cast<int>(Rank::count), 0);
    
    for (const auto& card : deck) {
        ++cardCounts[static_cast<int>(card.rank)];
    }

    for (int count : cardCounts) {
        if (count < 4) {
            throw std::runtime_error("Deck does not have at least 4 cards of each rank.");
        }
    }
}

template <typename Suit, typename Rank, typename DeckType>
bool GoFishGame<Suit, Rank, DeckType>::turn(int playerNumber) {
    if (playerNumber < 0 || playerNumber >= playerHands.size()) {
        throw std::out_of_range("Invalid player number");
    }

    // Display the current state of all players' hands and books
    std::cout << "Current Game State:\n";
    for (size_t i = 0; i < playerHands.size(); ++i) {
        std::cout << "Player " << i << " (" << playerNames[i] << "):\n";
        std::cout << "  Hand: ";
        playerHands[i].print(std::cout, 5);
        std::cout << "\n  Books: ";
        playerBooks[i].print(std::cout, 5);
        std::cout << "\n";
    }

    // Prompt for a rank and player to ask
    Rank requestedRank;
    int targetPlayer;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Player " << playerNumber << " (" << playerNames[playerNumber] << ") is asking for a card.\n";
        std::cout << "Enter the rank you are asking for: ";
        std::cin >> requestedRank;

        // Check if the rank is in the player's hand
        validInput = std::any_of(playerHands[playerNumber].begin(), playerHands[playerNumber].end(),
                                  [requestedRank](const Card<Suit, Rank>& card) {
                                      return card.getRank() == requestedRank;
                                  });

        if (!validInput) {
            std::cout << "You do not have any cards of that rank in your hand. Please try again.\n";
            continue;
        }

        std::cout << "Enter the number of the player you are asking: ";
        std::cin >> targetPlayer;

        if (targetPlayer < 0 || targetPlayer >= playerHands.size() || targetPlayer == playerNumber) {
            std::cout << "Invalid player number. Please try again.\n";
            validInput = false; 
        } else {
            validInput = true;
        }
    }

    // Request cards from the target player
    if (playerHands[targetPlayer].request(playerHands[playerNumber], requestedRank)) {
        // If successful, collect books for the current player
        while (collect_books(playerNumber)) {
            // Keep collecting books until no more 4-of-a-kinds are found
        }
        return true;
    }

    // If the request was unsuccessful, go fish
    if (!deck.is_empty()) {
        Card<Suit, Rank> drawnCard = deck.draw();
        std::cout << "Player " << playerNumber << " drew a card: " << drawnCard << "\n";

        if (drawnCard.getRank() == requestedRank) {
            std::cout << "The drawn card matches the requested rank!\n";
            playerHands[playerNumber].addCard(drawnCard);
            return true;  // The turn was successful
        } else {
            std::cout << "The drawn card does not match the requested rank.\n";
            playerHands[playerNumber].addCard(drawnCard);
            return false;  // The turn ends
        }
    } else {
        // Deck is empty, remove the current player from the game
        std::cout << "The deck is empty! Player " << playerNumber << " has been removed from the game.\n";
        playerHands.erase(playerHands.begin() + playerNumber);
        playerBooks.erase(playerBooks.begin() + playerNumber);
        playerNames.erase(playerNames.begin() + playerNumber);
        return false;
    }
}

template <typename Suit, typename Rank, typename DeckType>
void GoFishGame<Suit, Rank, DeckType>::play() {
    // Start by dealing the cards
    deal();

    // Collect books for each player until no more 4-of-a-kinds are found
    for (size_t i = 0; i < playerHands.size(); ++i) {
        while (collect_books(i)) {
            // Keep collecting books until no more 4-of-a-kinds are found in the player's hand
        }
    }

    // Main game loop: repeat until the game ends
    size_t round = 1;
    while (!deck.is_empty()) {
        std::cout << "Round " << round << " starts!\n";

        // Count active players within the play function
        size_t activePlayerCount = 0;
        for (const auto& hand : playerHands) {
            if (!hand.is_empty()) {
                activePlayerCount++;
            }
        }

        // If there is one or zero active players, the game should end
        if (activePlayerCount <= 1) {
            break;
        }

        // Iterate over each player in the game
        for (size_t i = 0; i < playerHands.size(); ++i) {
            // Check if the player is still active (i.e., has cards left)
            if (playerHands[i].is_empty()) {
                continue; // Skip inactive players
            }

            // Call the turn method and keep it running as long as it returns true
            while (turn(i)) {
                // Player gets another chance if they successfully collect cards
            }
        }

        // Print the number of books each player has made at the end of the round
        std::cout << "End of Round " << round << ".\n";
        for (size_t i = 0; i < playerHands.size(); ++i) {
            if (!playerHands[i].is_empty()) { // Print only for active players
                std::cout << "Player " << i << " has " << playerBooks[i].size() << " books.\n";
            }
        }
        round++;
    }

    // Determine the winner based on who has the most books
    size_t maxBooks = 0;
    std::vector<size_t> winners;

    for (size_t i = 0; i < playerBooks.size(); ++i) {
        if (playerBooks[i].size() > maxBooks) {
            maxBooks = playerBooks[i].size();
            winners.clear();
            winners.push_back(i);
        } else if (playerBooks[i].size() == maxBooks) {
            winners.push_back(i);
        }
    }

    // Print the winner(s)
    if (winners.size() == 1) {
        std::cout << "Player " << winners[0] << " wins with " << maxBooks << " books!\n";
    } else {
        std::cout << "It's a tie! Players ";
        for (size_t i = 0; i < winners.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << winners[i];
        }
        std::cout << " all have " << maxBooks << " books!\n";
    }
}

template <typename Suit, typename Rank, typename DeckType>
void GoFishGame<Suit, Rank, DeckType>::deal() {
    // Shuffle the deck
    deck.shuffle();
    
    // Determine how many cards each player should receive
    size_t cardsPerPlayer = (playerHands.size() == 2) ? 7 : 5;

    // Deal cards to each player
    for (size_t i = 0; i < playerHands.size(); ++i) {
        for (size_t j = 0; j < cardsPerPlayer; ++j) {
            // Draw a card from the deck and add it to the player's hand
            if (!deck.is_empty()) {
                Card<Suit, Rank> card = deck.draw();
                playerHands[i].addCard(card);
            } else {
                std::cout << "The deck is empty, unable to deal more cards.\n";
                return; // If the deck is empty, stop dealing
            }
        }
    }

    std::cout << "Cards have been dealt!\n";
}

template <typename Suit, typename Rank, typename DeckType>
bool GoFishGame<Suit, Rank, DeckType>::collect_books(int playerNumber) {
    if (playerNumber < 0 || playerNumber >= playerHands.size()) {
        throw std::out_of_range("Invalid player number");
    }

    CardSet<Suit, Rank>& playerHand = playerHands[playerNumber];

    // Iterate through the player's hand to find a 4-of-a-kind
    for (const auto& card : playerHand) {
        Rank rank = card.getRank();
        int count = std::count_if(playerHand.begin(), playerHand.end(), [rank](const Card<Suit, Rank>& c) {
            return c.getRank() == rank;
        });

        if (count == 4) {
            // Found a 4-of-a-kind, use collect_if to move these cards to the book
            auto predicate = [rank](Card<Suit, Rank>& c) {
                return c.getRank() == rank;
            };

            playerHand.collect_if(books[playerNumber], predicate);
            return true;  // Successfully collected 4-of-a-kind
        }
    }

    return false;  // No 4-of-a-kind found
}
