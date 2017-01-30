#ifndef MTG_OBSERVER_HPP
#define MTG_OBSERVER_HPP

#include "mtg_game.hpp"

/*************************************
 *
 * Базовый класс - наблюдатель за игрой.
 * Наследуясь от него, можно безпроблемно наблюдать за игрой и обрабатывать события игры.
 * Служит посредником 
 ************************************/

class MTG_ENGINE_EXPORT MTG_Observer
{
public:
	MTG_Observer();
	virtual ~MTG_Observer();

	void setGame(MTG_Game *aGame);
	MTG_Game *game() const;

	void clear();

protected:
	virtual void event(const MTG_Event *aEvent);
	virtual void changeGame(MTG_Game *aGame);
	virtual void gameEvent(MTG_Game::State_t aState);
	virtual void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	virtual void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	virtual void winEvent(MTG_Player *aPlayerWin);

protected:
	MTG_Game *mGame;

private:
	IDObserver_t mIDObserver;
};

#endif //MTG_OBSERVER_HPP