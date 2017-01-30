#ifndef MTG_BOARD_VIEW_HPP
#define MTG_BOARD_VIEW_HPP

#include "ui_board_view.h"
#include "mtg_engine.hpp"

using namespace Engine;

/*************************************
 *
 * Графическое представление доски - на которой отображаются карты которыми игроки ходят.
 *
 ************************************/

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

private:
	void cleanup();

private:
	Ui::BoardView ui;
	std::pair<MTG_Player*, MTG_Player*> mPlayers;
};


#endif //MTG_BOARD_VIEW_HPP