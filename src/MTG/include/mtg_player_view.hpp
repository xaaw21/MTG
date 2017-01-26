#ifndef MTG_PLAYER_VIEW_HPP
#define MTG_PLAYER_VIEW_HPP

#include <QWidget>
#include <QMap>
#include "mtg_engine.hpp"

class PanelView;

class MTG_PlayerView : public QWidget, protected MTG_Observer
{
public:
	MTG_PlayerView(QWidget *aParent = 0);
	virtual ~MTG_PlayerView();

	enum Orientation_t
	{
		E_TopOrientation = 0,
		E_BottomOrientation
	};

	void setPlayer(MTG_Player *aPlayer);
	MTG_Player *player() const;

	void setOrientation(Orientation_t aOrientation);
	Orientation_t orientation() const;

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);
	void resizeEvent(QResizeEvent *aEvent);
	void updateGeometry();
	void update();

private:
	MTG_Player *mPlayer;
	Orientation_t mOrientation;
	QMap<int,PanelView*> mPanels;
};


#endif //MTG_PLAYER_VIEW_HPP