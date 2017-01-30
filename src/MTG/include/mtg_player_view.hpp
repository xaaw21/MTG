#ifndef MTG_PLAYER_VIEW_HPP
#define MTG_PLAYER_VIEW_HPP

#include <QWidget>
#include "ui_player_view.h"
#include "mtg_engine.hpp"

/*************************************
 *
 * Графическое представление панели игрока(состояние и набор карт)
 *
 ************************************/

class MTG_PlayerView : public QWidget, protected MTG_Observer
{
	Q_OBJECT
public:
	MTG_PlayerView(QWidget *aParent = 0);
	virtual ~MTG_PlayerView();

	bool setPlayer(MTG_Player *aPlayer);
	MTG_Player *player() const;

	void clear();

protected:
	void changeGame(MTG_Game *aGame);						  
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);

private Q_SLOTS:
    void play();
	void open();
	void open(MTG_Card::State_t aState);
	void update();
	void checked(IDCard_t aIDCard, bool aChecked);

private:
	Ui::PlayerView ui;
	MTG_Player *mPlayer;
	MTG_Card::State_t mCurrCards;
	Mana_t mMana;
};


#endif //MTG_PLAYER_VIEW_HPP