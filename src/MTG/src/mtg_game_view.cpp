#include "mtg_game_view.hpp"

QString PhaseString(Phase_t aPhase) {
	QString str;
	switch (aPhase)
	{
	case E_StartPhase: str = "Начало боя"; break;
	case E_InvocationPhase: str = "Призыв"; break;
	case E_AttackPhase: str = "Аттака"; break;
	case E_FinishPhase: str = "Завершение боя"; break;
	}

	return "Фаза: " + str;
}

QString RoundString(Round_t aRound) {
	return QString("Раунд №%1").arg(aRound);
}

MTG_GameView::MTG_GameView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer()
{
	ui.setupUi(this);

	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(next()));
}

MTG_GameView::~MTG_GameView()
{

}

void MTG_GameView::changeGame(MTG_Game *aGame) {
	auto players = aGame->players();
	ui.PlayerViewFirst->setPlayer(players.first);
	ui.PlayerViewSecond->setPlayer(players.second);
	ui.LogView->setGame(aGame);
}

void MTG_GameView::gameEvent(MTG_Game::State_t aState) {
	  
}

void MTG_GameView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	switch (aPhase)
	{
	case E_StartPhase: {
		mTimer.start(1000);
		break;
	}
	case E_InvocationPhase:  break;
	case E_AttackPhase:  break;
	case E_FinishPhase: {
		mTimer.start(4000);
		break;
	}
	}

	update();
}

void MTG_GameView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	switch (aPhase)
	{
	case E_InvocationPhase:
	case E_AttackPhase: {
		auto players = mGame->players();
		if (players.first->state() == MTG_Player::E_PlayedState && players.second->state() == MTG_Player::E_PlayedState) {
			mTimer.start(1000);
		}
		break;
	}
	}
}

void MTG_GameView::winEvent(MTG_Player *aPlayerWin) {

}

void MTG_GameView::next() {
	mGame->next();
}

void MTG_GameView::update() {
	if (!mGame) return;
	ui.PhaseLabel->setText(PhaseString(mGame->phase()));
	ui.RoundLabel->setText(RoundString(mGame->round()));
	//if (mGame->state() == MTG_Game::E_StopState) ui.StartButton->setText("Старт игра");
	//else ui.StartButton->setText("Стоп игра");
}