#ifndef MTG_LOG_VIEW_HPP
#define MTG_LOG_VIEW_HPP

#include <QWidget>
#include "ui_log_view.h"
#include "mtg_engine.hpp"

/*************************************
 *
 * Графическое представление, регистрирующее события игры и отображающее их в виде лога
 *
 ************************************/

class MTG_LogView : public QWidget, public MTG_Observer
{
public:
	MTG_LogView(QWidget *aParent = 0);
	~MTG_LogView();

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);

private:
	void refresh();
	void print(const QString &aMessage);

private:
	Ui::LogView ui;
};


#endif //MTG_LOG_VIEW_HPP