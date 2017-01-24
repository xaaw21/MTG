#include "mtg_robot.hpp"

MTG_Robot::MTG_Robot()
	:MTG_Player()
{

}

MTG_Robot::~MTG_Robot()
{

}

bool MTG_Robot::invocation() {
	MTG_CardSet cards = this->cards(E_CallCard);
	MTG_CardSet invocation_cards;
	Mana mana = this->mana();
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {
		MTG_Card card = CardFromID(*it);
		if (card.Cost <= mana) {
			invocation_cards.push_back(*it);
			mana -= card.Cost;
		}

		if (mana <= 0) break;
	}

	return play(invocation_cards);
}

bool MTG_Robot::attack() {
	MTG_CardSet cards = this->cards(E_ProtectionCard);
	MTG_CardSet attack_cards;
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {
		MTG_Card card = CardFromID(*it);
		if(card.Attack > card.Protection) attack_cards.push_back(*it);
	}

	return play(attack_cards);
}

bool MTG_Robot::protection(const MTG_CardSet &aAttackCards) {
	MTG_CardSet cards = this->cards(E_ProtectionCard);
	MTG_CardSet protection_cards;
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {
		MTG_Card card = CardFromID(*it);
		if (card.Protection > card.Attack) protection_cards.push_back(*it);
	}

	return play(protection_cards);
}
