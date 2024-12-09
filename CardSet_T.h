#pragma once

#include <iostream>
#include <vector>
#include "Card_T.h"
#include <functional> 

template <typename Rank, typename Suits>
class CardSet {
protected:
    // Protected vector of cards (parameterized by Rank and Suits)
    std::vector<Card<Rank, Suits>> cards;

public:
    // Typedefs for convenience
    typedef Card<Rank, Suits> card_type; // Represents the type of cards stored
    typedef typename std::vector<Card<Rank, Suits>>::iterator iterator; // Iterator type for cards
    typedef typename std::vector<Card<Rank, Suits>>::const_iterator const_iterator; // Const iterator type for cards

    virtual ~CardSet() = default;

    // Print method
    void print(std::ostream&, size_t) const;

    // Check if the card set is empty
    bool is_empty() const;

    // Move a card from this set to another
    CardSet<Rank, Suits>& operator>>(CardSet<Rank, Suits>& other);

    // Accessor methods for iterators
    iterator begin();
    iterator end();

    // Sort method
    void sort();

    // Collect method to move cards from another set
    void collect(CardSet<Rank, Suits>& other);

    // Method to collect cards matching a given predicate
    void collect_if(CardSet<Rank, Suits>& deck, std::function<bool(Card<Rank, Suits>&)> predicate);

    // Method to request a card by rank from another CardSet
    bool request(CardSet<Rank, Suits>& deck, Rank rank);
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "CardSet_T.cpp"
#endif
