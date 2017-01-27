#ifndef MTG_BOARD_VIEW_HPP
#define MTG_BOARD_VIEW_HPP

#include "ui_board_view.h"
#include "mtg_engine.hpp"


class MTG_BoardView : public QWidget, public MTG_Observer
{
public:
	MTG_BoardView(QWidget *aParent = 0);
	~MTG_BoardView();

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);

private:
	Ui::BoardView ui;
};

#endif //MTG_BOARD_VIEW_HPP