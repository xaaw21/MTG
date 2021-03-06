#ifndef MTG_GAME_VIEW_HPP
#define MTG_GAME_VIEW_HPP

#include <QWidget>
#include <QTimer>
#include <QGraphicsBlurEffect>
#include "ui_game_view.h"
#include "mtg_engine.hpp"

using namespace Engine;

class SplashView;

/*************************************
 *
 * Главное графическое представление игры - отображает все процессы, которые происходят в игре,
 * а так же позволяет управлять ей
 * 
 * Состоит из отображений:
 * 	- MTG_BoardView
 * 	- MTG_PlayerView(их два - для каждого игрока)
 * 	- MTG_LogView
 *
 ************************************/

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
	void resizeEvent(QResizeEvent *aEvent);

private Q_SLOTS:
	void next();
	void start();
	void exit();

private:
	void update(Phase_t Phase);

private:
	Ui::GameView ui;
	QTimer mTimer;
	SplashView *mSplash;
	QGraphicsBlurEffect mEffect;
};

#endif //MTG_GAME_VIEW_HPP