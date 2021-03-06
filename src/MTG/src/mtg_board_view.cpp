#include "mtg_board_view.hpp"

MTG_BoardView::MTG_BoardView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mPlayers(0,0)
{
	ui.setupUi(this);

	ui.CardsViewFirst->setAlignment(Qt::AlignHCenter);
	ui.CardsViewSecond->setAlignment(Qt::AlignHCenter);
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
	case MTG_Game::E_StopState: break;
	case MTG_Game::E_StartState: {
		cleanup();
		break;
	}
	}
}

void MTG_BoardView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	UNUSED(aRound);

	switch (aPhase)
	{
	case E_FinishPhase: {
		MTG_CardMap map = aCards;
		ui.CardsViewFirst->setCards(map[mPlayers.first]);
		ui.CardsViewSecond->setCards(map[mPlayers.second]);
		break;
	}
	default: cleanup(); break;
	}
}

void MTG_BoardView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	UNUSED(aPhase);
	
	if (aPlayer == mPlayers.first) ui.CardsViewFirst->setCards(aCards);
	else if (aPlayer == mPlayers.second) ui.CardsViewSecond->setCards(aCards);
}

void MTG_BoardView::cleanup() {
	ui.CardsViewFirst->clear();
	ui.CardsViewSecond->clear();
}