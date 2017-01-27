#ifndef MTG_PLAYER_VIEW_HPP
#define MTG_PLAYER_VIEW_HPP

#include <QWidget>
#include "ui_player_view.h"
#include "mtg_engine.hpp"

class MTG_PlayerView : public QWidget, protected MTG_Observer
{
	Q_OBJECT
public:
	MTG_PlayerView(QWidget *aParent = 0);
	virtual ~MTG_PlayerView();

	void setPlayer(MTG_Player *aPlayer);
	MTG_Player *player() const;

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);
	void resizeEvent(QResizeEvent *aEvent);

private Q_SLOTS:
    void play();

private:
	Ui::PlayerView ui;
	MTG_Player *mPlayer;
};


#endif //MTG_PLAYER_VIEW_HPP