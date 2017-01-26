#include "mtg_observer.hpp"

#include "mtg_events.hpp"

MTG_Observer::MTG_Observer()
	:mGame(0),
	mIDObserver(INVALID_ID_OBSERVER)
{

}

MTG_Observer::~MTG_Observer()
{

}

void MTG_Observer::setGame(MTG_Game *aGame) {
	using namespace std::placeholders;

	if (mGame) mGame->delObserver(mIDObserver);
	mGame = aGame;
	if (mGame) mIDObserver = mGame->addObserver(std::bind(&MTG_Observer::event, this, _1));
	changeGame(mGame);
}

MTG_Game *MTG_Observer::game() const {
	return mGame;
}

void MTG_Observer::clear() {
	if (mGame) {
		mGame->delObserver(mIDObserver);
		mGame = 0;
		changeGame(mGame);
	}
}

void MTG_Observer::event(const MTG_Event *aEvent) {
	switch (aEvent->type())
	{
	case MTG_Event::E_GameEvent: {
		const MTG_GameEvent *game_event = (const MTG_GameEvent*)aEvent;
		this->gameEvent((MTG_Game::State_t)game_event->state());
		break;
	}
	case MTG_Event::E_PhaseEvent: {
		const MTG_PhaseEvent *phase_event = (const MTG_PhaseEvent*)aEvent;
		this->phaseEvent(phase_event->phase(), phase_event->round(), phase_event->cards());
		break;
	}
	case MTG_Event::E_PlayerEvent: {
		const MTG_PlayerEvent *player_event = (const MTG_PlayerEvent*)aEvent;
		this->playerEvent(player_event->phase(), player_event->player(), player_event->cards());
		break;
	}
	case MTG_Event::E_WinEvent: {
		const MTG_WinEvent *win_event = (const MTG_WinEvent*)aEvent;
		this->winEvent(win_event->playerWin());
		break;
	}
	}
}

void MTG_Observer::changeGame(MTG_Game *aGame) {}
void MTG_Observer::gameEvent(MTG_Game::State_t aState) {}
void MTG_Observer::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {}
void MTG_Observer::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {}
void MTG_Observer::winEvent(MTG_Player *aPlayerWin) {}