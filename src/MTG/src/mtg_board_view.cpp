#include "mtg_board_view.hpp"

MTG_BoardView::MTG_BoardView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mPlayers(0,0)
{
	ui.setupUi(this);
}

MTG_BoardView::~MTG_BoardView()
{

}

void MTG_BoardView::changeGame(MTG_Game *aGame) {
	mPlayers = std::pair<MTG_Player*, MTG_Player*>(0,0);
	cleanup();
	if (aGame) mPlayers = aGame->players();
}

void MTG_BoardView::gameEvent(MTG_Game::State_t aState) {
	switch (aState)
	{
	case MTG_Game::E_StartState: {
		cleanup();
		break;
	}
	}
}

void MTG_BoardView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	switch (aPhase)
	{
	case E_FinishPhase: {
		ui.CardsViewFirst->repaint();
		ui.CardsViewSecond->repaint();
		break;
	}
	default: cleanup(); break;
	}
}

void MTG_BoardView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	if (aPlayer == mPlayers.first) ui.CardsViewFirst->setCards(aCards);
	else if (aPlayer == mPlayers.second) ui.CardsViewSecond->setCards(aCards);
}

void MTG_BoardView::winEvent(MTG_Player *aPlayerWin) {

}

void MTG_BoardView::cleanup() {
	ui.CardsViewFirst->clear();
	ui.CardsViewSecond->clear();
}