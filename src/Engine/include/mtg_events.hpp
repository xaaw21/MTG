#ifndef MTG_EVENTS_HPP
#define MTG_EVENTS_HPP

#include "mtg_types.hpp"


class MTG_Player;

class MTG_ENGINE_EXPORT MTG_Event
{
public:

	enum Type
	{
		E_NoneEvent = 0,
		E_GameEvent,
		E_StartEvent,
		E_InvocationEvent,
		E_AttackEvent,
		E_FinishEvent,
		E_PlayerEvent,
		E_WinEvent
	};

	MTG_Event(Type aType);
	virtual ~MTG_Event();

	Type type() const;

private:
	Type mType;
};

class MTG_ENGINE_EXPORT MTG_GameEvent : public MTG_Event
{
public:
	MTG_GameEvent(int aState);
	~MTG_GameEvent();

	int state() const;

private:
	int mState;
};

class MTG_ENGINE_EXPORT MTG_PhaseEvent : public MTG_Event
{
public:
	MTG_PhaseEvent(Type aType);
	~MTG_PhaseEvent();

	Phase phase() const;
};

class MTG_ENGINE_EXPORT MTG_StartEvent : public MTG_PhaseEvent
{
public:
	MTG_StartEvent(Runde aRunde,Mana aMana,IDCard aOpenFirst,IDCard aOpenSecond,MTG_Player *aAttackPlayer);
	~MTG_StartEvent();

	Runde runde() const;
	Mana mana() const;
	IDCard openFirst() const;
	IDCard openSecond() const;
	MTG_Player* playerAttack() const;

private:
	Runde mRunde;
	Mana mMana;
	IDCard mOpenFirst, mOpenSecond;
	MTG_Player *mAttackPlayer;
};

class MTG_ENGINE_EXPORT MTG_InvocationEvent : public MTG_PhaseEvent
{
public:
	MTG_InvocationEvent();
	~MTG_InvocationEvent();
};

class MTG_ENGINE_EXPORT MTG_AttackEvent : public MTG_PhaseEvent
{
public:
	MTG_AttackEvent(MTG_Player *aAttackPlayer);
	~MTG_AttackEvent();

	MTG_Player* playerAttack() const;

private:
	MTG_Player *mAttackPlayer
};

class MTG_ENGINE_EXPORT MTG_FinishEvent : public MTG_PhaseEvent
{
public:
	MTG_FinishEvent();
	~MTG_FinishEvent();

};

class MTG_ENGINE_EXPORT MTG_PlayerEvent : public MTG_Event
{
public:
	MTG_PlayerEvent(Phase aPhase,const MTG_CardSet &aCards);
	~MTG_PlayerEvent();

	Phase phase() const;
	MTG_CardSet cards() const;

private:
	MTG_Player *mPlayer;
	Phase mPhase;
	MTG_CardSet mCards;
};

class MTG_ENGINE_EXPORT MTG_WinEvent : public MTG_Event
{
public:
	MTG_WinEvent(MTG_Player *aWinPlayer);
	~MTG_WinEvent();

	MTG_Player* playerWin() const;

private:
	MTG_Player *mWinPlayer
};


#endif //MTG_EVENTS_HPP