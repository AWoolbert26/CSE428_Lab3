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
#include <vector>
#include <string>
#include "Deck_T.h"
#include "Card_T.h"

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

std::ostream& operator<<(std::ostream&, const Color&);

Color& operator++(Color&);

class UnoDeck : public Deck<UnoRank, Color> {
public:
    UnoDeck();  

    // Used to check valid requests for GoFish
    static UnoRank fromString(const std::string& str);    
};