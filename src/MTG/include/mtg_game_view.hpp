#ifndef MTG_GAME_VIEW_HPP
#define MTG_GAME_VIEW_HPP

#include <QWidget>
#include <QTimer>
#include "ui_mtg_game_view.h"
#include "mtg_engine.hpp"

class MTG_GameView : public QWidget, public MTG_Observer
{
	Q_OBJECT
public:
	MTG_GameView(QWidget *aParent = 0);
	~MTG_GameView();

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);

private Q_SLOTS:
	void next();

private:
	void update();

private:
	Ui::GameView ui;
	QTimer mTimer;
};

#endif //MTG_GAME_VIEW_HPP