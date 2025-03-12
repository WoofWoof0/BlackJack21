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
        if(cards.empty()) {
            shuffle();
        }

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

// Helper: Convert Card::Rank to lowercase string (e.g., ACE → "ace")
std::string getRankString(Card::Rank rank) {
    switch (rank) {
        case Card::ACE:   return "ace";
        case Card::TWO:   return "two";
        case Card::THREE: return "three";
        case Card::FOUR:  return "four";
        case Card::FIVE:  return "five";
        case Card::SIX:   return "six";
        case Card::SEVEN: return "seven";
        case Card::EIGHT: return "eight";
        case Card::NINE:  return "nine";
        case Card::TEN:   return "ten";
        case Card::JACK:  return "jack";
        case Card::QUEEN: return "queen";
        case Card::KING:  return "king";
        default:          return "";
    }
}

// Helper: Convert Card::Suit to lowercase string (e.g., SPADES → "spades")
std::string getSuitString(Card::Suit suit) {
    switch (suit) {
        case Card::HEARTS:   return "hearts";
        case Card::DIAMONDS: return "diamonds";
        case Card::CLUBS:    return "clubs";
        case Card::SPADES:   return "spades";
        default:             return "";
    }
}

std::string getImageName(const Card &card) {
    std::string rank = getRankString(card.rank);
    std::string suit = getSuitString(card.suit);
    std::string imageName = rank + "_of_" + suit;

    // Append "2" for Kings, Queens, Jacks, or Ace of Spades
    if (card.rank == Card::KING || card.rank == Card::QUEEN || card.rank == Card::JACK) {
        imageName += "2"; // For IDs like "king_of_spades2"
    } else if (card.rank == Card::ACE && card.suit == Card::SPADES) {
        imageName += "2"; // For "ace_of_spades2"
    }

    return imageName;
}

Deck deck;
Player player;
Dealer dealer;
bool gameOver = false;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_nativeInit(JNIEnv *env, jobject thiz) {
    deck = Deck();
    player.clearHand();
    dealer.clearHand();
    gameOver = false;

    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_nativeHit(JNIEnv *env, jobject thiz) {
    if(!gameOver) {
        player.addCard(deck.drawCard());
        if(player.calculateScore() > 21) {
            gameOver = true;
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_nativeStand(JNIEnv *env, jobject thiz) {
    if(!gameOver) {
        while(dealer.shouldHit()) {
            dealer.addCard(deck.drawCard());
        }
        gameOver = true;
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_getDealerHand(JNIEnv *env, jobject thiz) {
    /*std::string handStr;
    for (const Card &card : dealer.getHand()) {
        switch (card.rank) {
            case Card::ACE: handStr += "A "; break;
            case Card::JACK: handStr += "J "; break;
            case Card::QUEEN: handStr += "Q "; break;
            case Card::KING: handStr += "K ";break;
            default:         handStr += std::to_string(card.rank) + " ";
        }
    }
    return env->NewStringUTF(handStr.c_str());*/

    std::string handStr;
    for (const Card &card : dealer.getHand()) {
        handStr += getImageName(card) + " "; // e.g., "king_of_spades2 ace_of_hearts "
    }
    if (!handStr.empty()) handStr.pop_back(); // Remove trailing space
    return env->NewStringUTF(handStr.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_getPlayerHand(JNIEnv *env, jobject thiz) {
    std::string handStr;
    for (const Card &card : player.getHand()) {
        handStr += getImageName(card) + " ";
    }
    if (!handStr.empty()) handStr.pop_back();
    return env->NewStringUTF(handStr.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_getGameStatus(JNIEnv *env, jobject thiz) {
    if(!gameOver) return env->NewStringUTF("");

    int playerScore = player.calculateScore();
    int dealerScore = dealer.calculateScore();

    if(playerScore > 21) return env->NewStringUTF("Bust! Dealer Wins.");
    if(dealerScore > 21) return env->NewStringUTF("Dealer Busts! You Win.");
    if(playerScore > dealerScore) return env->NewStringUTF("You Win.");
    if(dealerScore > playerScore) return env->NewStringUTF("Dealer Wins.");
    return env->NewStringUTF("Push (Tie).");
}
