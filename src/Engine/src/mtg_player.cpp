#include "mtg_player.hpp"

#include "mtg_game.hpp"
#include "mtg_events.hpp"

MTG_Player::MTG_Player()
	:mGame(0),
	mName(),
	mMana(0),
	mHealth(0),
	mDeck(),
    mState(E_NoneState),
    mRole(E_NoneRole),
    mCards()
{

}

MTG_Player::~MTG_Player() {

}

const MTG_Game *MTG_Player::game() const {
	return mGame;
}

Mana_t MTG_Player::mana() const {
	return mMana;
}

Health_t MTG_Player::health() const {
	return mHealth;
}

MTG_Player::State_t MTG_Player::state() const {
	return mState;
}

Role_t MTG_Player::role() const {
	return mRole;
}

MTG_Deck MTG_Player::deck() const {
	return mDeck;
}

MTG_CardSet MTG_Player::cards() const {
	return mCards;
}

MTG_CardSet MTG_Player::cards(MTG_Card::State_t aState) const {
	return  mCards.cards(aState);
}

void MTG_Player::setName(const std::string &aName) {
	mName = aName;
}

std::string MTG_Player::name() const {
	return mName;
}

#include <iostream>

bool MTG_Player::play(const MTG_CardSet &aCards) {
	if (!mGame) return false;
	if (mState != E_PlayState) return false;

	MTG_CardSet play_cards;
	Phase_t phase = mGame->phase();
	for (auto it = aCards.begin(), end = aCards.end(); it != end; ++it) {		
		switch (phase)
		{
		case E_InvocationPhase: {
			if (it->cost() > mMana) continue;
			for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
				if (it_cards->ID == it->ID){
					if (it_cards->State == MTG_Card::E_OpenState) {
						mMana -= it_cards->cost();
						it_cards->State = MTG_Card::E_InvocationState;
						play_cards.push_back(*it_cards);
					}
				}
			}

			break;
		}
		case E_AttackPhase: {
			for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
				if (it_cards->ID == it->ID) {
					if (it_cards->State == MTG_Card::E_ProtectionState) {
						it_cards->State = MTG_Card::E_AttackState;
						play_cards.push_back(*it_cards);
					}
					break;
				}
			}

			break;
		}
		default: return false;
		}
	}

	return mGame->play(this, play_cards);
}

void MTG_Player::event(const MTG_Event *aEvent) {
	if (!mGame) return;

	switch (aEvent->type())
	{
	case MTG_Event::E_PhaseEvent: {
		if (mState == E_PlayState) {
			const MTG_PhaseEvent *phase_event = (const MTG_PhaseEvent*)aEvent;
			switch (phase_event->phase())
			{
			case E_InvocationPhase: invocation(); break;
			case E_AttackPhase: if(mRole == ::E_AttackRole) attack(); break;
			default: break;
			}
		}
		break;
	}
	case MTG_Event::E_PlayerEvent: {
		if (mState == E_PlayState) {
			const MTG_PlayerEvent *player_event = (const MTG_PlayerEvent*)aEvent;
			if (player_event->player() == this) break;
			switch (player_event->phase())
			{
			case E_AttackPhase: if(mRole == ::E_ProtectionRole) protection(player_event->cards()); break;
			default: break;
			}
		}
		break;
	}
	default: break;
	}
}
