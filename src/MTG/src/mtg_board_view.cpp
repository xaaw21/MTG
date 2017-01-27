#include "mtg_board_view.hpp"

MTG_BoardView::MTG_BoardView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer()
{
	ui.setupUi(this);
}

MTG_BoardView::~MTG_BoardView()
{

}

void MTG_BoardView::changeGame(MTG_Game *aGame) {

}

void MTG_BoardView::gameEvent(MTG_Game::State_t aState) {

}

void MTG_BoardView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {

}

void MTG_BoardView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {

}

void MTG_BoardView::winEvent(MTG_Player *aPlayerWin) {

}