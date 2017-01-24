#include "mtg_events.hpp"

/**********************    MTG_Event   ***************************************/

MTG_Event::MTG_Event(Type aType)
	:mType(E_NoneEvent)
{

}

MTG_Event::~MTG_Event()
{

}

MTG_Event::Type MTG_Event::type() const {
	return mType;
}

/**********************    MTG_GameEvent   ***************************************/

MTG_GameEvent::MTG_GameEvent(int aState)
	:MTG_Event(E_GameEvent), mState(aState)
{

}

MTG_GameEvent::~MTG_GameEvent()
{

}

int MTG_GameEvent::state() const {
	return mState;
}

/**********************    MTG_PhaseEvent   ***************************************/

MTG_PhaseEvent::MTG_PhaseEvent(Type aType)
	:MTG_Event(aType)
{

}

MTG_PhaseEvent::~MTG_PhaseEvent()
{

}

Phase MTG_PhaseEvent::phase() const {
	switch(type())
	{
	case E_StartEvent: return E_StartPhase;
	case E_InvocationEvent: return E_InvocationPhase;
	case E_AttackEvent:	return E_AttackPhase;
	case E_FinishEvent:	return E_FinishPhase;
	}

	return E_NonePhase;
}

/**********************    MTG_StartEvent   ***************************************/

MTG_StartEvent::MTG_StartEvent(Runde aRunde, Mana aMana, IDCard aOpenFirst, IDCard aOpenSecond, MTG_Player *aAttackPlayer)
	:MTG_PhaseEvent(E_StartEvent),
	mRunde(aRunde),
	mMana(aMana),
	mOpenFirst(aOpenFirst),
	mOpenSecond(aOpenSecond),
	mAttackPlayer(aAttackPlayer)
{

}

MTG_StartEvent::~MTG_StartEvent()
{

}

Runde MTG_StartEvent::runde() const {
	return mRunde;
}

Mana MTG_StartEvent::mana() const {
	return mMana;
}

IDCard MTG_StartEvent::openFirst() const {
	return mOpenFirst;
}

IDCard MTG_StartEvent::openSecond() const {
	return mOpenSecond;
}

MTG_Player* MTG_StartEvent::playerAttack() const {
	return mAttackPlayer;
}

/**********************    MTG_InvocationEvent   ***************************************/

MTG_InvocationEvent::MTG_InvocationEvent()
	:MTG_PhaseEvent(E_InvocationEvent)
{

}

MTG_InvocationEvent::~MTG_InvocationEvent()
{

}

/**********************    MTG_AttackEvent   ***************************************/

MTG_AttackEvent::MTG_AttackEvent(MTG_Player *aAttackPlayer)
	:MTG_PhaseEvent(E_AttackEvent),
	mAttackPlayer(aAttackPlayer)
{

}

MTG_AttackEvent::~MTG_AttackEvent()
{

}

MTG_Player* MTG_AttackEvent::playerAttack() const {
	return mAttackPlayer;
}

/**********************    MTG_FinishEvent   ***************************************/

MTG_FinishEvent::MTG_FinishEvent()
	:MTG_PhaseEvent(E_FinishEvent)
{

}

MTG_FinishEvent::~MTG_FinishEvent()
{

}

/**********************    MTG_PlayerEvent   ***************************************/

MTG_PlayerEvent::MTG_PlayerEvent(Phase aPhase, const MTG_CardSet &aCards)
	:MTG_Event(E_PlayerEvent), mPhase(aPhase), mCards(aCards)
{

}

MTG_PlayerEvent::~MTG_PlayerEvent()
{

}

Phase MTG_PlayerEvent::phase() const {
	return mPhase;
}

MTG_CardSet MTG_PlayerEvent::cards() const {
	return mCards;
}

/**********************    MTG_WinEvent   ***************************************/

MTG_WinEvent::MTG_WinEvent(MTG_Player *aWinPlayer)
	:MTG_Event(E_WinEvent),
	mWinPlayer(aWinPlayer)
{

}

MTG_WinEvent::~MTG_WinEvent()
{

}

MTG_Player* MTG_WinEvent::playerWin() const {
	return mWinPlayer;
}