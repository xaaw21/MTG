#include "mtg_player.hpp"

#include "mtg_game.hpp"
#include "mtg_events.hpp"

MTG_Player::MTG_Player()
	:mGame(0), mMana(0), mHealth(0)
{

}

MTG_Player::~MTG_Player() {

}

MTG_Game *MTG_Player::game() const {
	return mGame;
}

Mana MTG_Player::mana() const {
	return mMana;
}

Health MTG_Player::health() const {
	return mHealth;
}

MTG_Player::State MTG_Player::state() const {
	return mState;
}

MTG_CardSet MTG_Player::cards(CardState aCardState) const {
	return  MTG_CardSet();
}

bool MTG_Player::select(IDCard aIDCard) {
	if (!mGame) return false;

	switch (mGame->info().phase) {
	case E_CallPhase: {
		MTG_Card card = CardFromID(aIDCard);
		if (!card.isValid()) return false;
		if (card.Cost > mMana) return false;
		if (!mCardsOpen.take(aIDCard)) return false;
		mMana -= card.Cost;
	}
	case E_AttackPhase: return mCardsProtection.take(aIDCard);
	}

	return false;
}

bool MTG_Player::play(const MTG_CardSet &aCards) {
	if (!mGame) return false;
	if (mState != E_Walk) return false;
	MTG_CardSet card_set;
	for (auto it = aCards.begin(), end = aCards.end(); it != end; ++it) {
		if (select(*it)) card_set.push_back(*it);
	}

	switch (mGame->info().phase)
	{
	case E_CallPhase: mCardsInvocation.push(card_set); break;
	case E_AttackPhase: mCardsAttack.push(card_set); break;
	}

	return mGame->play(this, card_set);
}

void MTG_Player::event(const MTG_Event *aEvent) {
	if (!mGame) return;

	switch (aEvent->type())
	{
	case MTG_Event::E_GameEvent:  break;
	case MTG_Event::E_RundeEvent: break;
	case MTG_Event::E_PhaseEvent: {
		if (mState == E_Walk) {
			const MTG_PhaseEvent *phase_event = (const MTG_PhaseEvent*)aEvent;
			switch (phase_event->phase())
			{
			case E_CallPhase: invocation(); break;
			case E_AttackPhase: attack(); break;
			}
		}
		break;
	}
	case MTG_Event::E_PlayerEvent: {
		if (mState == E_Walk) {
			const MTG_PlayerEvent *player_event = (const MTG_PlayerEvent*)aEvent;
			switch (player_event->phase())
			{
			case E_AttackPhase: protection(player_event->cards()); break;
			}
		}
		break;
	}
	}
}
