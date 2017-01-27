#include "mtg_player_view.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QFrame>

/***************************   MTG_PlayerView   **************************************/

MTG_PlayerView::MTG_PlayerView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mPlayer(0)
{
	ui.setupUi(this);
}

MTG_PlayerView::~MTG_PlayerView()
{

}

void MTG_PlayerView::setPlayer(MTG_Player *aPlayer) {
	mPlayer = aPlayer;
	//TODO: так делать плохо
	MTG_Observer::setGame(const_cast<MTG_Game*>(mPlayer->game()));
}

MTG_Player *MTG_PlayerView::player() const {
	return mPlayer;
}	

void MTG_PlayerView::changeGame(MTG_Game *aGame) {

}

void MTG_PlayerView::gameEvent(MTG_Game::State_t aState) {

}

void MTG_PlayerView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {

}

void MTG_PlayerView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {

}

void MTG_PlayerView::winEvent(MTG_Player *aPlayerWin) {

}

void MTG_PlayerView::resizeEvent(QResizeEvent *aEvent) {

}

void MTG_PlayerView::play() {
	if (!mPlayer) return;
	mPlayer->play();
}