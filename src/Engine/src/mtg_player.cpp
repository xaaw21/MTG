#include "mtg_player.hpp"

#include "mtg_game.hpp"
#include "mtg_events.hpp"

namespace Engine
{

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

	const MTG_Game* MTG_Player::game() const {
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

	bool MTG_Player::play(const MTG_CardSet &aCards) {
		if (!mGame) return false;
		return mGame->play(this, aCards);
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
				case E_AttackPhase: if (mRole == E_AttackRole) attack(); break;
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
				case E_AttackPhase: if (mRole == E_ProtectionRole) protection(player_event->cards()); break;
				default: break;
				}
			}
			break;
		}
		default: break;
		}
	}

};//namespace Engine

