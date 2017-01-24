#ifndef MTG_VIEW_HPP
#define MTG_VIEW_HPP

#include "mtg_game.hpp"

class MTG_ENGINE_EXPORT MTG_View
{
public:
	MTG_View();
	virtual ~MTG_View();

	void setGame(MTG_Game *aGame);
	MTG_Game *game() const;

protected:
	virtual void event(const MTG_Event *aEvent);
	virtual void changeGame(MTG_Game *aGame);
	virtual void changeRunde(Runde aRunde);
	virtual void changePhase(Phase aPhase);


protected:
	MTG_Game *mGame;

private:
	IDObserver mIDObserver;
};

#endif //MTG_VIEW_HPP