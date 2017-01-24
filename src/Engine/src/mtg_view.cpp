#include "mtg_view.hpp"


MTG_View::MTG_View()
	:mGame(0),
	mIDObserver(INVALID_ID_OBSERVER)
{

}

MTG_View::~MTG_View()
{

}

void MTG_View::setGame(MTG_Game *aGame) {
	using namespace std::placeholders;

	if (mGame) mGame->delObserver(mIDObserver);
	mGame = aGame;
	if (mGame) mIDObserver = mGame->addObserver(std::bind(&MTG_View::event, this, _1));
	changeGame(mGame);
}

MTG_Game *MTG_View::game() const {
	return mGame;
}

void MTG_View::event(const MTG_Event *aEvent) {

}

void MTG_View::changeGame(MTG_Game *aGame) {

}