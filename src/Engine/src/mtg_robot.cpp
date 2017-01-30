#include "mtg_robot.hpp"

MTG_Robot::MTG_Robot()
	:MTG_Player()
{

}

MTG_Robot::~MTG_Robot()
{

}

bool MTG_Robot::invocation() {
	MTG_CardSet cards = this->cards(MTG_Card::E_OpenState);
	MTG_CardSet invocation_cards;
	Mana_t mana = this->mana();
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {
		if (it->cost() <= mana) {
			invocation_cards.push_back(*it);
			mana -= it->cost();
		}

		if (mana <= 0) break;
	}


	return play(invocation_cards);
}

bool MTG_Robot::attack() {
	MTG_CardSet cards = this->cards(MTG_Card::E_ProtectionState);
	MTG_CardSet attack_cards;
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {;
		if(it->attack() > it->protection()) attack_cards.push_back(*it);
	}

	if (attack_cards.empty()) attack_cards = cards;

	return play(attack_cards);
}

bool MTG_Robot::protection(const MTG_CardSet &aAttackCards) {
	UNUSED(aAttackCards);
	
	MTG_CardSet cards = this->cards(MTG_Card::E_ProtectionState);
	MTG_CardSet protection_cards;
	for (auto it = cards.begin(), end = cards.end(); it != end; ++it) {
		if (it->protection() > it->attack()) protection_cards.push_back(*it);
	}

	//if (protection_cards.empty()) protection_cards = cards;

	return play(protection_cards);
}
