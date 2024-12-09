#include "CardSet_T.h"
#include <algorithm> 
#include <iterator>  
#include <functional> 

template <typename Rank, typename Suits>
void CardSet<Rank, Suits>::print(std::ostream& os, size_t cardsPerLine) const {
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

template <typename Rank, typename Suits>
bool CardSet<Rank, Suits>::is_empty() const {
    return cards.empty();
}

template <typename Rank, typename Suits>
CardSet<Rank, Suits>& CardSet<Rank, Suits>::operator>>(CardSet<Rank, Suits>& other) {
    if (is_empty()) {
        throw std::runtime_error("The current CardSet is empty.");
    }

    Card<Rank, Suits> cardToMove = cards.back();
    other.cards.push_back(cardToMove);
    cards.pop_back();

    return *this;
}

// Accessor method for begin iterator
template <typename Rank, typename Suits>
typename std::vector<Card<Rank, Suits>>::iterator CardSet<Rank, Suits>::begin() {
    return cards.begin();
}

// Accessor method for end iterator
template <typename Rank, typename Suits>
typename std::vector<Card<Rank, Suits>>::iterator CardSet<Rank, Suits>::end() {
    return cards.end();
}

// Sort method implementation
template <typename Rank, typename Suits>
void CardSet<Rank, Suits>::sort() {
    std::sort(cards.begin(), cards.end());
}

template <typename Rank, typename Suits>
void CardSet<Rank, Suits>::collect(CardSet<Rank, Suits>& other) {
    std::move(other.cards.begin(), other.cards.end(), std::back_inserter(this->cards));

    other.cards.clear();
}

template <typename Rank, typename Suits>
void CardSet<Rank, Suits>::collect_if(CardSet<Rank, Suits>& deck, std::function<bool(Card<Rank, Suits>&)> predicate) {
    std::vector<Card<Rank, Suits>> matchingCards;
    std::copy_if(deck.begin(), deck.end(), std::back_inserter(matchingCards), predicate);

    cards.insert(cards.end(), std::make_move_iterator(matchingCards.begin()), std::make_move_iterator(matchingCards.end()));

    auto newEnd = std::remove_if(deck.begin(), deck.end(), predicate);
    deck.cards.erase(newEnd, deck.cards.end());
}

template <typename Rank, typename Suits>
bool CardSet<Rank, Suits>::request(CardSet<Rank, Suits>& deck, Rank rank) {
    auto it = std::find_if(deck.begin(), deck.end(), [&rank](const Card<Rank, Suits>& card) {
        return card.getRank() == rank;
    });

    if (it == deck.end()) {
        return false;
    }

    cards.push_back(std::move(*it));

    deck.cards.erase(it);

    return true;
}