/**
 * Created by shiny on 2025-02-20.
 *
 */

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <jni.h>

class Card {
public:
    enum Suit {HEARTS, DIAMONDS, CLUBS, SPADES};
    enum Rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

    Suit suit;
    Rank rank;

    Card(Suit s, Rank r) : suit(s), rank(r) {}

    int getValue() const {
        return (rank >= JACK) ? 10 : rank;
    }
};

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck() {
        for (int s = Card::HEARTS; s <= Card::SPADES; s++) {
            for (int r = Card::ACE; r <= Card::KING; r++) {
                cards.emplace_back(static_cast<Card::Suit>(s), static_cast<Card::Rank>(r));
            }
        }
        shuffle();
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card drawCard() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    bool isEmpty() const {
        return cards.empty();
    }
};

class Player {
protected:
    std::vector<Card> hand;

public:
    void addCard(Card card) {
        hand.push_back(card);
    }

    int calculateScore() const {
        int score = 0;
        int aces = 0;

        for (const Card &card : hand) {
            int value = card.getValue();
            if (value == 1) aces++;
            score += value;
        }

        while (aces > 0 && score + 10 <= 21) {
            score += 10;
            aces--;
        }
        return score;
    }

    void clearHand() {
        hand.clear();
    }

    const std::vector<Card>& getHand() const {
        return hand;
    }
};

class Dealer : public Player {
public:
    bool shouldHit() const {
        return calculateScore() < 17;
    }
};