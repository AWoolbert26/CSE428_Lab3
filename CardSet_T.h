#pragma once

#include <iostream>
#include <vector>
#include "Card_T.h"
#include <functional> 

template <typename Rank, typename Suit>
class CardSet {
protected:
    std::vector<Card<Rank, Suit>> cards;

public:
    typedef Card<Rank, Suit> card_type; 
    typedef typename std::vector<Card<Rank, Suit>>::iterator iterator; 
    typedef typename std::vector<Card<Rank, Suit>>::const_iterator const_iterator;

    virtual ~CardSet() = default;

    void print(std::ostream&, size_t) const;

    bool is_empty() const;

    CardSet<Rank, Suit>& operator>>(CardSet<Rank, Suit>& other);

    iterator begin();
    iterator end();

    void sort();
    void collect(CardSet<Rank, Suit>& other);
    void collect_if(CardSet<Rank, Suit>& deck, std::function<bool(Card<Rank, Suit>&)> predicate);
    bool request(CardSet<Rank, Suit>& deck, Rank rank);
    int get_size();
    Card<Rank, Suit> get_top_card();
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "CardSet_T.cpp"
#endif
