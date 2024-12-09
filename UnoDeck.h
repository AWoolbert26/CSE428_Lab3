/*
 * File: UnoDeck.h
 * 
 * Andrew Woolbert - a.s.woolbert@wustl.edu
 * Daniel Tymoshenko d.tymoshenko@wustl.edu
 * Rohan Tatikonda r.s.tatikonda@wustl.edu
 * 
 * This file contains the declarations related to the uno deck and its class
 */
#pragma once 

#include <iostream>
#include "Deck_T.h"

enum class Color {
    red, 
    blue, 
    green, 
    yellow, 
    black,
    undefined
};

enum class UnoRank {
    zero, 
    one, 
    two, 
    three, 
    four, 
    five, 
    six, 
    seven, 
    eight, 
    nine, 
    skip, 
    reverse, 
    drawtwo, 
    drawfour, 
    wild, 
    blank, 
    undefined
};

std::ostream& operator<<(std::ostream&, const UnoRank&);

UnoRank& operator++(UnoRank&);

class UnoDeck : public Deck<Color, UnoRank> {
public:
    UnoDeck();  
};