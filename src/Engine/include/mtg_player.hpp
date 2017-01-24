#ifndef MTG_PLAYER_HPP
#define MTG_PLAYER_HPP

#include "mtg_types.hpp"

class MTG_Game;
class MTG_Event;

class MTG_ENGINE_EXPORT MTG_Player
{
	friend MTG_Game;
public:
	MTG_Player();
	virtual ~MTG_Player();

	enum State
	{
		E_NoneState = 0,
		E_WaitWalk,
		E_NoWalk,
		E_PlayState,
		E_AlredyWalk
	};

	MTG_Game *game() const;
	State state() const;
	Mana mana() const;
	Health health() const;
	bool attack() const;
	MTG_CardSet cards(CardState aCardState) const;
	
	bool play(const MTG_CardSet &aCards = MTG_CardSet());

protected:
	virtual void event(const MTG_Event *aEvent);
	virtual bool invocation() = 0;
	virtual bool attack() = 0;				    
	virtual bool protection(const MTG_CardSet &aAttackCards) = 0;

private:
	bool select(IDCard aIDCard);

private:
	MTG_Game *mGame;
	Mana mMana;
	Health mHealth;
	MTG_Deck mDeck;
	State mState;

	MTG_CardSet mCardsOpen;
	MTG_CardSet mCardsInvocation;
	MTG_CardSet mCardsProtection;
	MTG_CardSet mCardsAttack;
};

#endif //MTG_PLAYER_HPP