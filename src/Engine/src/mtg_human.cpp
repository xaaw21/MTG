#include "mtg_human.hpp"

MTG_Human::MTG_Human()
	:MTG_Player()
{

}

MTG_Human::~MTG_Human()
{

}

bool MTG_Human::invocation() {
	return false;
}

bool MTG_Human::attack() {
	return false;
}

bool MTG_Human::protection(const MTG_CardSet &aAttackCards) {
	return false;
}
