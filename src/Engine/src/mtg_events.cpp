#include "mtg_events.hpp"

/**********************    MTG_Event   ***************************************/

MTG_Event::MTG_Event(Type aType)
	:mType(aType),
	mGame(0)
{

}

MTG_Event::~MTG_Event()
{

}

MTG_Event::Type MTG_Event::type() const {
	return mType;
}

const MTG_Game* MTG_Event::game() const {
	return mGame;
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

MTG_PhaseEvent::MTG_PhaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards)
	:MTG_Event(E_PhaseEvent),
	mPhase(aPhase),
	mRound(aRound),
	mCards(aCards)
{

}

MTG_PhaseEvent::~MTG_PhaseEvent()
{

}

Phase_t MTG_PhaseEvent::phase() const {
	return mPhase;
}

Round_t MTG_PhaseEvent::round() const {
	return mRound;
}

MTG_CardMap MTG_PhaseEvent::cards() const {
	return mCards;
}

/**********************    MTG_PlayerEvent   ***************************************/

MTG_PlayerEvent::MTG_PlayerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards)
	:MTG_Event(E_PlayerEvent),
	mPlayer(aPlayer),
	mPhase(aPhase),
	mCards(aCards)
{

}

MTG_PlayerEvent::~MTG_PlayerEvent()
{

}

Phase_t MTG_PlayerEvent::phase() const {
	return mPhase;
}

MTG_CardSet MTG_PlayerEvent::cards() const {
	return mCards;
}

MTG_Player* MTG_PlayerEvent::player() const {
	return mPlayer;
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