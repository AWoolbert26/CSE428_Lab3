#include "CardSet_T.h"
#include <algorithm> 
#include <iterator>  
#include <functional> 

template <typename Rank, typename Suit>
void CardSet<Rank, Suit>::print(std::ostream& os, size_t cardsPerLine) const {
    size_t count = 0;
    for (const auto& c : cards) {
        os << c << ' ';
        ++count;
        if (count % cardsPerLine == 0) {
            os << '\n';
        }
    }

    // Final newline if the last line wasn't complete
    if (count % cardsPerLine != 0 || is_empty()) {
        os << '\n';
    }
}

template <typename Rank, typename Suit>
bool CardSet<Rank, Suit>::is_empty() const {
    return cards.empty();
}

template <typename Rank, typename Suit>
CardSet<Rank, Suit>& CardSet<Rank, Suit>::operator>>(CardSet<Rank, Suit>& other) {
    if (is_empty()) {
        throw std::runtime_error("The current CardSet is empty.");
    }

    Card<Rank, Suit> cardToMove = cards.back();
    other.cards.push_back(cardToMove);
    cards.pop_back();

    return *this;
}

// Accessor method for begin iterator
template <typename Rank, typename Suit>
typename std::vector<Card<Rank, Suit>>::iterator CardSet<Rank, Suit>::begin() {
    return cards.begin();
}

// Accessor method for end iterator
template <typename Rank, typename Suit>
typename std::vector<Card<Rank, Suit>>::iterator CardSet<Rank, Suit>::end() {
    return cards.end();
}

// Sort method implementation
template <typename Rank, typename Suit>
void CardSet<Rank, Suit>::sort() {
    std::sort(cards.begin(), cards.end());
}

template <typename Rank, typename Suit>
void CardSet<Rank, Suit>::collect(CardSet<Rank, Suit>& other) {
    std::move(other.cards.begin(), other.cards.end(), std::back_inserter(this->cards));

    other.cards.clear();
}

template <typename Rank, typename Suit>
void CardSet<Rank, Suit>::collect_if(CardSet<Rank, Suit>& deck, std::function<bool(Card<Rank, Suit>&)> predicate) {
    std::vector<Card<Rank, Suit>> matchingCards;
    std::copy_if(deck.begin(), deck.end(), std::back_inserter(matchingCards), predicate);

    cards.insert(cards.end(), std::make_move_iterator(matchingCards.begin()), std::make_move_iterator(matchingCards.end()));

    auto newEnd = std::remove_if(deck.begin(), deck.end(), predicate);
    deck.cards.erase(newEnd, deck.cards.end());
}

template <typename Rank, typename Suit>
bool CardSet<Rank, Suit>::request(CardSet<Rank, Suit>& deck, Rank rank) {
    auto it = std::find_if(deck.begin(), deck.end(), [&rank](const Card<Rank, Suit>& card) {
        return card.rank == rank;
    });

    if (it == deck.end()) {
        return false;
    }

    cards.push_back(std::move(*it));

    deck.cards.erase(it);

    return true;
}

template <typename Rank, typename Suit>
int CardSet<Rank, Suit>::get_size() {
    return (this->cards.size());
}

template <typename Rank, typename Suit>
Card<Rank, Suit> CardSet<Rank, Suit>::get_top_card() {
    return cards.back();
}