#include "mtg_types.hpp"

#include <random>
#include <algorithm>

Card_t CardFromID(IDCard_t aIDCard, bool *ok) {
	if (ok) *ok = false;
	if (aIDCard < 0 || aIDCard > COUNT_CARDS - 1) return {0,0,0};
	if (ok) *ok = true;
	return CARDS[aIDCard];
}

Phase_t NextPhase(Phase_t aPhase) {
	if (aPhase == E_FinishPhase) return E_StartPhase;
	return (Phase_t)(aPhase + 1);
}

/*****************************   MTG_Card   ******************************************/

MTG_Card::MTG_Card(MTG_Card::State_t aState)
	:ID(INVALID_ID_CARD),
	State(aState),
	Health(0)
{

}

MTG_Card::MTG_Card(IDCard_t aIDCard, State_t aState)
	:ID(aIDCard),
	State(aState),
	Health(0)
{
	Health = protection();
}

MTG_Card::~MTG_Card()
{

}

uint8_t MTG_Card::attack() const {
	return CardFromID(ID).Attack;
}

uint8_t MTG_Card::protection() const {
	return CardFromID(ID).Protection;
}

uint8_t MTG_Card::cost() const {
	return CardFromID(ID).Cost;
}

MTG_Card::operator IDCard_t() const {
	return ID;
}

MTG_Card::operator Card_t() const {
	return CardFromID(ID);
}

/*****************************   MTG_CardSet   ******************************************/

MTG_CardSet::MTG_CardSet()
	:std::list<MTG_Card>()
{

}

MTG_CardSet::~MTG_CardSet()
{

}

MTG_CardSet MTG_CardSet::cards(MTG_Card::State_t aState) const {
	MTG_CardSet cards;
	for (auto it = this->begin(), end = this->end(); it != end; it++) {
		if (aState == it->State) cards.push_back(*it);
	}

	return cards;
}

/*****************************   MTG_Deck   ******************************************/

MTG_Deck::MTG_Deck()
	:std::vector<IDCard_t>()
{
	reset();
}

MTG_Deck::~MTG_Deck()
{

}

IDCard_t MTG_Deck::card() {
	if (empty()) return INVALID_ID_CARD;
	IDCard_t card = back();
	pop_back();
	return card;
}

MTG_CardSet MTG_Deck::cards(uint8_t aCount) {
	MTG_CardSet cards;
	for (int i = 0; i < aCount;i++) {
		IDCard_t card = this->card();
		if (card == INVALID_ID_CARD) break;
		cards.push_back(card);
	}
	return cards;
}

void MTG_Deck::reset() {
	clear();
	reserve(COUNT_CARDS);
	for (IDCard_t i = COUNT_CARDS - 1; i >= 0; i--) {
		push_back(i);
	}
	//std::list<int> list = { 0,1,2,3,4,5,6,7,6,4 };
}

void MTG_Deck::disturb() {
	std::random_device device;
	std::mt19937 generator(device());
	std::shuffle(this->begin(), this->end(), generator);
}

