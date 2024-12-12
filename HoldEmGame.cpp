/*
 * File: HoldEmGame.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the definitions of the HoldEmGame class.
 */

#include "HoldEmGame.h"
#include "Game.h"
#include "Constants.h"
#include <algorithm>

// Initialize the state to preflop and pass parameters to the game base class
HoldEmGame::HoldEmGame(int argc, const char* argv[]): Game(argc, argv), game_state(HoldEmState::preflop), deck(), board_cards() {
    // Push hands into the player hands vector.
    for (size_t i = 0; i < player_names.size(); ++i) {
        player_hands.emplace_back();
    }
}

HoldEmGame::Player::Player(const CardSet<HoldEmRank, Suit>& hand, const std::string& name, HoldEmHandRank handRank): hand(hand), name(name), handRank(handRank) {}

// Deal cards to each player and manage the game state
void HoldEmGame::deal() {
    switch (game_state) {
        case HoldEmState::preflop:
            // Deal two cards to each player
            for (size_t player_index = 0; player_index < player_hands.size(); ++player_index) {
                for (int i = 0; i < HOLD_EM_HAND_SIZE; ++i) {
                    deck >> player_hands[player_index];
                }
            }
            game_state = HoldEmState::flop;  
            break;

        case HoldEmState::flop:
            // Deal three cards to the board
            for (size_t i = 0; i < NUM_FLOP_CARDS; ++i) {
                deck >> board_cards;
            }
            game_state = HoldEmState::turn;  
            break;

        case HoldEmState::turn:
            // Deal one card to the board
            deck >> board_cards;
            game_state = HoldEmState::river;  
            break;

        case HoldEmState::river:
            // Deal one card to the board
            deck >> board_cards;
            game_state = HoldEmState::undefined;  
            break;

        case HoldEmState::undefined:
            break;

        default:
            std::cerr << "Error: Invalid game state." << std::endl;
            break;
    }
}

void HoldEmGame::printBoard() {
    board_cards.print(std::cout, HOLD_EM_BOARD_HAND_SIZE);  
}

void HoldEmGame::printPlayerHands() {
    for (size_t i = 0; i < player_names.size(); ++i) {
        std::cout << player_names[i] << "'s hand:\n";
        player_hands[i].print(std::cout, HOLD_EM_HAND_SIZE); 
    }
}

// Collect cards: Move all cards back from the players' hands and board into the deck
void HoldEmGame::collectCards() {
    for (size_t i = 0; i < player_hands.size(); ++i) {
        deck.collect(player_hands[i]); 
    }
    deck.collect(board_cards); 
}

HoldEmHandRank HoldEmGame::holdEmHandEvaluation(const CardSet<HoldEmRank, Suit>& hand) {

    CardSet<HoldEmRank, Suit> localHand = hand;

    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    // Check if the hand has exactly five cards
    if (cards.size() != HOLD_EM_FULL_HAND_SIZE) {
        return HoldEmHandRank::undefined;
    }

    std::sort(cards.begin(), cards.end(), [](const Card<HoldEmRank, Suit>& a, const Card<HoldEmRank, Suit>& b) {
        return compareByRankThenSuit(a, b);
    });

    // Check for Flush
    bool isFlush = std::all_of(cards.begin(), cards.end(), [&](const Card<HoldEmRank, Suit>& card) {
        return card.suit == cards[0].suit; 
    });

    // Check for Straight
    bool isStraight = true;
    for (size_t i = 0; i < ADDITIONAL_CARDS_FOR_VALID_STRAIGHT; ++i) {
        if (static_cast<int>(cards[i + 1].rank) != static_cast<int>(cards[i].rank) + 1) {
            isStraight = false;
            break;
        }
    }

    // Special case for A, 2, 3, 4, 5
    if (
        !isStraight && cards[FIRST].rank == HoldEmRank::two && cards[SECOND].rank == HoldEmRank::three &&
        cards[THIRD].rank == HoldEmRank::four && cards[FOURTH].rank == HoldEmRank::five &&
        cards[FIFTH].rank == HoldEmRank::ace) {
        isStraight = true;
    }

    // Check for four of a kind, full house, three of a kind, two pair, and pair
    std::unordered_map<HoldEmRank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    bool hasPair = false;
    int pairCount = 0;

    for (const auto& [rank, count] : rankCount) {
        if (count == FOUR_OF_A_KIND) {
            hasFourOfAKind = true;
        } else if (count == THREE_OF_A_KIND) {
            hasThreeOfAKind = true;
        } else if (count == PAIR) {
            hasPair = true;
            pairCount++;
        }
    }

    // Determine hand rank
    if (isFlush && isStraight) {
        return HoldEmHandRank::straightflush;
    } else if (hasFourOfAKind) {
        return HoldEmHandRank::fourofakind;
    } else if (hasThreeOfAKind && pairCount == 1) {
        return HoldEmHandRank::fullhouse;
    } else if (isFlush) {
        return HoldEmHandRank::flush;
    } else if (isStraight) {
        return HoldEmHandRank::straight;
    } else if (hasThreeOfAKind) {
        return HoldEmHandRank::threeofakind;
    } else if (pairCount == 2) {
        return HoldEmHandRank::twopair;
    } else if (hasPair) {
        return HoldEmHandRank::pair;
    } else {
        return HoldEmHandRank::xhigh;
    }
}

int HoldEmGame::play() {
    std::string user_input;

    do {
        deck.shuffle();
        game_state = HoldEmState::preflop;

        deal();

        printPlayerHands();

        deal();

        std::cout << "BOARD (flop): \n";
        printBoard();

        std::vector<Player> playerHands;

        for (size_t i = 0; i < player_names.size(); ++i) {
            playerHands.emplace_back(player_hands[i], player_names[i], HoldEmHandRank::undefined);
        }

        for (auto& playerHand : playerHands) {

            CardSet<HoldEmRank, Suit> board_cards_copy = board_cards;

            for (int i = 0; i < NUM_FLOP_CARDS; i++) {
                board_cards_copy >> playerHand.hand;
            }

            playerHand.handRank = holdEmHandEvaluation(playerHand.hand); // Evaluate hand rank
        }

        std::sort(playerHands.begin(), playerHands.end(), [](const Player& a, const Player& b) {
            return a < b; 
        });

        // Backwards because the way std::sort works is the revers of what we want
        for (int i = playerHands.size() - 1; i >= 0; i--) {
            std::cout << playerHands[i].name << " ";
            playerHands[i].hand.print(std::cout, 5);
            std::cout << " " << playerHands[i].handRank << std::endl;
        }


        deal();

        std::cout << "BOARD (turn): \n";
        printBoard();

        deal();

        std::cout << "BOARD (river): \n";
        printBoard();

        collectCards();
    } while (!askUserToQuit());

    return SUCCESS;
}

// Overloaded operator<< for printing HoldEmHandRanks
std::ostream& operator<<(std::ostream& os, const HoldEmHandRank& rank) {
    static const std::array<std::string, NUM_OF_HOLD_EM_RANKS> rankNames = {
        "xhigh", 
        "pair", 
        "twopair", 
        "threeofakind", 
        "straight", 
        "flush", 
        "fullhouse", 
        "fourofakind", 
        "straightflush", 
        "undefined"
    };

    int index = static_cast<int>(rank);
    
    os << rankNames[index];
    return os;
}

bool operator<(const HoldEmGame::Player& player1, const HoldEmGame::Player& player2) {
    if (player1.handRank != player2.handRank) {
        return player1.handRank < player2.handRank; 
    }

    switch (player1.handRank) {
        case HoldEmHandRank::pair: {
            auto player1PairRank = HoldEmGame::getPairRank(player1.hand);
            auto player2PairRank = HoldEmGame::getPairRank(player2.hand);

            if (player1PairRank != player2PairRank) {
                return player1PairRank < player2PairRank;
            }
         
            auto player1Kickers = HoldEmGame::getKickers(player1.hand);
            auto player2Kickers = HoldEmGame::getKickers(player2.hand);

            for (size_t i = 0; i < std::min(player1Kickers.size(), player2Kickers.size()); ++i) {
                if (player1Kickers[i] != player2Kickers[i]) {
                    return player1Kickers[i] < player2Kickers[i];
                }
            }
            return false; 
        }

        case HoldEmHandRank::twopair: {
            auto player1HigherPairRank = HoldEmGame::getHigherPairRank(player1.hand);
            auto player2HigherPairRank = HoldEmGame::getHigherPairRank(player2.hand);
            if (player1HigherPairRank != player2HigherPairRank) {
                return player1HigherPairRank < player2HigherPairRank;
            }
            auto player1LowerPairRank = HoldEmGame::getLowerPairRank(player1.hand);
            auto player2LowerPairRank = HoldEmGame::getLowerPairRank(player2.hand);
            if (player1LowerPairRank != player2LowerPairRank) {
                return player1LowerPairRank < player2LowerPairRank;
            }
            auto player1Kicker = HoldEmGame::getKickers(player1.hand)[0];
            auto player2Kicker = HoldEmGame::getKickers(player2.hand)[0];
            return player1Kicker < player2Kicker;
        }

        case HoldEmHandRank::threeofakind: {
            auto player1ThreeRank = HoldEmGame::getThreeOfAKindRank(player1.hand);
            auto player2ThreeRank = HoldEmGame::getThreeOfAKindRank(player2.hand);

            if (player1ThreeRank != player2ThreeRank) {
                return player1ThreeRank < player2ThreeRank;
            }

            auto player1Kickers = HoldEmGame::getKickers(player1.hand);
            auto player2Kickers = HoldEmGame::getKickers(player2.hand);

            for (size_t i = 0; i < std::min(player1Kickers.size(), player2Kickers.size()); ++i) {
                if (player1Kickers[i] != player2Kickers[i]) {
                    return player1Kickers[i] < player2Kickers[i];
                }
            }
            return false; 
        }

        case HoldEmHandRank::straight: {
            // Get sorted ranks for both players
            auto player1Ranks = HoldEmGame::getSortedRanks(player1.hand);
            auto player2Ranks = HoldEmGame::getSortedRanks(player2.hand);

            // Check for "wheel" straight in player 1's hand
            bool player1HasWheel = (player1Ranks[0] == HoldEmRank::ace &&
                                    player1Ranks[1] == HoldEmRank::five &&
                                    player1Ranks[2] == HoldEmRank::four &&
                                    player1Ranks[3] == HoldEmRank::three &&
                                    player1Ranks[4] == HoldEmRank::two);

            // Check for "wheel" straight in player 2's hand
            bool player2HasWheel = (player2Ranks[0] == HoldEmRank::ace &&
                                    player2Ranks[1] == HoldEmRank::five &&
                                    player2Ranks[2] == HoldEmRank::four &&
                                    player2Ranks[3] == HoldEmRank::three &&
                                    player2Ranks[4] == HoldEmRank::two);

            // Adjust the highest card for "wheel" hands to be the lowest rank (two)
            auto player1HighestCard = player1HasWheel ? HoldEmRank::five : player1Ranks[0];
            auto player2HighestCard = player2HasWheel ? HoldEmRank::five : player2Ranks[0];

            // Compare the highest cards
            return player1HighestCard < player2HighestCard;
        }

        case HoldEmHandRank::flush:
        case HoldEmHandRank::xhigh: {
            auto player1Ranks = HoldEmGame::getSortedRanks(player1.hand);
            auto player2Ranks = HoldEmGame::getSortedRanks(player2.hand);
            for (size_t i = 0; i < std::min(player1Ranks.size(), player2Ranks.size()); ++i) {
                if (player1Ranks[i] != player2Ranks[i]) {
                    return player1Ranks[i] < player2Ranks[i];
                }
            }
            return false; 
        }

        case HoldEmHandRank::fullhouse: {
            auto player1ThreeRank = HoldEmGame::getThreeOfAKindRank(player1.hand);
            auto player2ThreeRank = HoldEmGame::getThreeOfAKindRank(player2.hand);
            return player1ThreeRank < player2ThreeRank;
        }

        case HoldEmHandRank::fourofakind: {
            auto player1FourRank = HoldEmGame::getFourOfAKindRank(player1.hand);
            auto player2FourRank = HoldEmGame::getFourOfAKindRank(player2.hand);
            return player1FourRank < player2FourRank;
        }

        case HoldEmHandRank::straightflush: {
            // Get sorted ranks for both players
            auto player1Ranks = HoldEmGame::getSortedRanks(player1.hand);
            auto player2Ranks = HoldEmGame::getSortedRanks(player2.hand);

            // Check for "wheel" straight in player 1's hand
            bool player1HasWheel = (player1Ranks[0] == HoldEmRank::ace &&
                                    player1Ranks[1] == HoldEmRank::five &&
                                    player1Ranks[2] == HoldEmRank::four &&
                                    player1Ranks[3] == HoldEmRank::three &&
                                    player1Ranks[4] == HoldEmRank::two);

            // Check for "wheel" straight in player 2's hand
            bool player2HasWheel = (player2Ranks[0] == HoldEmRank::ace &&
                                    player2Ranks[1] == HoldEmRank::five &&
                                    player2Ranks[2] == HoldEmRank::four &&
                                    player2Ranks[3] == HoldEmRank::three &&
                                    player2Ranks[4] == HoldEmRank::two);

            // Adjust the highest card for "wheel" hands to be the lowest rank (two)
            auto player1HighestCard = player1HasWheel ? HoldEmRank::five : player1Ranks[0];
            auto player2HighestCard = player2HasWheel ? HoldEmRank::five : player2Ranks[0];

            // Compare the highest cards
            return player1HighestCard < player2HighestCard;
        }

        default:
            return false; 
    }
}

// Implementation of helper functions

HoldEmRank HoldEmGame::getPairRank(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;

    CardSet<HoldEmRank, Suit> localHand = hand;

    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }
    for (const auto& [rank, count] : rankCount) {
        if (count == PAIR) {
            return rank; 
        }
    }

    return HoldEmRank::undefined; 
}

std::vector<HoldEmRank> HoldEmGame::getKickers(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;

    CardSet<HoldEmRank, Suit> localHand = hand;

    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    std::vector<HoldEmRank> kickers;

    for (const auto& [rank, count] : rankCount) {
        if (count == 1) {
            kickers.push_back(rank);
        }
    }

    std::sort(kickers.rbegin(), kickers.rend());

    return kickers;
}

HoldEmRank HoldEmGame::getHigherPairRank(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    HoldEmRank higherPairRank = HoldEmRank::undefined;

    for (const auto& [rank, count] : rankCount) {
        if (count >= PAIR) {
            if (higherPairRank == HoldEmRank::undefined || rank > higherPairRank) {
                higherPairRank = rank;
            }
        }
    }

    return higherPairRank;
}

HoldEmRank HoldEmGame::getLowerPairRank(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    HoldEmRank lowerPairRank = HoldEmRank::undefined;

    for (const auto& [rank, count] : rankCount) {
        if (count >= PAIR) {
            if (lowerPairRank == HoldEmRank::undefined || rank < lowerPairRank) {
                lowerPairRank = rank;
            }
        }
    }

    return lowerPairRank;
}

HoldEmRank HoldEmGame::getFourOfAKindRank(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    for (const auto& [rank, count] : rankCount) {
        if (count == FOUR_OF_A_KIND) {
            return rank;
        }
    }

    return HoldEmRank::undefined;
}

HoldEmRank HoldEmGame::getThreeOfAKindRank(const CardSet<HoldEmRank, Suit>& hand) {
    std::unordered_map<HoldEmRank, int> rankCount;
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    for (const auto& card : cards) {
        rankCount[card.rank]++;
    }

    for (const auto& [rank, count] : rankCount) {
        if (count == THREE_OF_A_KIND) {
            return rank;
        }
    }

    return HoldEmRank::undefined;
}

HoldEmRank HoldEmGame::getHighestCard(const CardSet<HoldEmRank, Suit>& hand) {
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    HoldEmRank highestRank = HoldEmRank::undefined;

    for (const auto& card : cards) {
        if (card.rank > highestRank) {
            highestRank = card.rank;
        }
    }

    return highestRank;
}

std::vector<HoldEmRank> HoldEmGame::getSortedRanks(const CardSet<HoldEmRank, Suit>& hand) {
    CardSet<HoldEmRank, Suit> localHand = hand;
    std::vector<Card<HoldEmRank, Suit>> cards(localHand.begin(), localHand.end());

    std::vector<HoldEmRank> ranks;

    for (const auto& card : cards) {
        ranks.push_back(card.rank);
    }

    std::sort(ranks.begin(), ranks.end(), std::greater<HoldEmRank>());
    
    return ranks;
}