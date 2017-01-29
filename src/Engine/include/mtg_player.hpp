#ifndef MTG_PLAYER_HPP
#define MTG_PLAYER_HPP

#include "mtg_types.hpp"
#include <string>

class MTG_Game;
class MTG_Event;

class MTG_ENGINE_EXPORT MTG_Player
{
	friend MTG_Game;
public:
	MTG_Player();
	virtual ~MTG_Player();

	enum State_t
	{
		E_NoneState = 0,
		E_PlayState,
		E_PlayedState
	};

	const MTG_Game *game() const;
	State_t state() const;
	Role_t role() const;
	Mana_t mana() const;
	Health_t health() const;
	MTG_Deck deck() const;
	MTG_CardSet cards() const;
	MTG_CardSet cards(MTG_Card::State_t aState) const;

	void setName(const std::string &aName);
	std::string name() const;
	
	bool play(const MTG_CardSet &aCards = MTG_CardSet());

protected:
	virtual void event(const MTG_Event *aEvent);
	virtual bool invocation() = 0;
	virtual bool attack() = 0;				    
	virtual bool protection(const MTG_CardSet &aAttackCards) = 0;

private:
	MTG_Game *mGame;
	std::string mName;
	Mana_t mMana;
	Health_t mHealth;
	MTG_Deck mDeck;
	State_t mState;
	Role_t mRole;
	MTG_CardSet mCards;
};

#endif //MTG_PLAYER_HPP