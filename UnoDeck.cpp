/*
 * File: UnoDeck.cpp
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the definitions related to the Uno deck and its class
 */

#include "UnoDeck.h"

// Inserts color representation on ostream
std::ostream& operator<<(std::ostream& os, const Color& color) {
    switch (color) {
        case Color::red:
            os << "Red";
            break;
        case Color::blue:
            os << "Blue";
            break;
        case Color::green:
            os << "Green";
            break;
        case Color::yellow:
            os << "Yellow";
            break;
        case Color::black:
            os << "Black";
            break;
        default:
            os << "Undefined";
    }
    return os;
}

// Increments color (cycles through colors)
Color& operator++(Color& color) {
    switch (color) {
        case Color::red:
            color = Color::blue;
            break;
        case Color::blue:
            color = Color::green;
            break;
        case Color::green:
            color = Color::yellow;
            break;
        case Color::yellow:
            color = Color::black;
            break;
        case Color::black:
            color = Color::red;
            break;
        default:
            color = Color::undefined;
    }
    return color;
}

// Inserts UnoRank representation on ostream
std::ostream& operator<<(std::ostream& os, const UnoRank& rank) {
    switch (rank) {
        case UnoRank::zero:
            os << "0";
            break;
        case UnoRank::one:
            os << "1";
            break;
        case UnoRank::two:
            os << "2";
            break;
        case UnoRank::three:
            os << "3";
            break;
        case UnoRank::four:
            os << "4";
            break;
        case UnoRank::five:
            os << "5";
            break;
        case UnoRank::six:
            os << "6";
            break;
        case UnoRank::seven:
            os << "7";
            break;
        case UnoRank::eight:
            os << "8";
            break;
        case UnoRank::nine:
            os << "9";
            break;
        case UnoRank::skip:
            os << "Skip";
            break;
        case UnoRank::reverse:
            os << "Reverse";
            break;
        case UnoRank::drawtwo:
            os << "Draw Two";
            break;
        case UnoRank::drawfour:
            os << "Draw Four";
            break;
        case UnoRank::wild:
            os << "Wild";
            break;
        case UnoRank::blank:
            os << "Blank";
            break;
        default:
            os << "Undefined";
    }
    return os;
}

// Increments UnoRank (cycles through ranks)
UnoRank& operator++(UnoRank& rank) {
    switch (rank) {
        case UnoRank::zero:
            rank = UnoRank::one;
            break;
        case UnoRank::one:
            rank = UnoRank::two;
            break;
        case UnoRank::two:
            rank = UnoRank::three;
            break;
        case UnoRank::three:
            rank = UnoRank::four;
            break;
        case UnoRank::four:
            rank = UnoRank::five;
            break;
        case UnoRank::five:
            rank = UnoRank::six;
            break;
        case UnoRank::six:
            rank = UnoRank::seven;
            break;
        case UnoRank::seven:
            rank = UnoRank::eight;
            break;
        case UnoRank::eight:
            rank = UnoRank::nine;
            break;
        case UnoRank::nine:
            rank = UnoRank::skip;
            break;
        case UnoRank::skip:
            rank = UnoRank::reverse;
            break;
        case UnoRank::reverse:
            rank = UnoRank::drawtwo;
            break;
        case UnoRank::drawtwo:
            rank = UnoRank::drawfour;
            break;
        case UnoRank::drawfour:
            rank = UnoRank::wild;
            break;
        case UnoRank::wild:
            rank = UnoRank::blank;
            break;
        case UnoRank::blank:
            rank = UnoRank::zero;  // Cycles back to the beginning
            break;
        default:
            rank = UnoRank::undefined;
    }
    return rank;
}

// Constructor for UnoDeck
UnoDeck::UnoDeck() {
    // One card of rank zero for each color (one red, blue, green, yellow)
    for (Color color : {Color::red, Color::blue, Color::green, Color::yellow}) {
        this->cards.push_back(Card<Color, UnoRank>(color, UnoRank::zero));
    }

    // Two cards of each rank (1 to drawtwo) for each color
    for (UnoRank rank : {UnoRank::one, UnoRank::two, UnoRank::three, UnoRank::four,
                          UnoRank::five, UnoRank::six, UnoRank::seven, UnoRank::eight,
                          UnoRank::nine, UnoRank::skip, UnoRank::reverse, UnoRank::drawtwo}) {
        for (Color color : {Color::red, Color::blue, Color::green, Color::yellow}) {
            this->cards.push_back(Card<Color, UnoRank>(color, rank));
            this->cards.push_back(Card<Color, UnoRank>(color, rank));  // Add the second card
        }
    }

    // Four black cards for each rank (drawfour, wild, blank)
    for (UnoRank rank : {UnoRank::drawfour, UnoRank::wild, UnoRank::blank}) {
        for (int i = 0; i < 4; ++i) {
            this->cards.push_back(Card<Color, UnoRank>(Color::black, rank));
        }
    }
}
