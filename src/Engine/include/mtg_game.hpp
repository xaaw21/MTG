#ifndef MTG_GAME_HPP
#define MTG_GAME_HPP

#include "mtg_types.hpp"
#include <functional>
#include <map>

#define INVALID_ID_OBSERVER 0

class MTG_Player;
class MTG_Event;

typedef int IDObserver;
typedef std::function<void(const MTG_Event*)> MTG_Observer;

class MTG_ENGINE_EXPORT MTG_Game
{
	friend MTG_Player;
public:
	MTG_Game();
	virtual ~MTG_Game();

	enum State
	{
		E_StopState = 0,
		E_StartState
	};

	State state() const;
	Runde runde() const;
	Phase phase() const;
	
	bool setPlayers(MTG_Player *aFirstPlayer, MTG_Player *aSecondPlayer);
	MTG_Player* playerFirst() const;
	MTG_Player* playerSecond() const;
	MTG_Player* playerAttack() const;
	MTG_Player* playerWin() const;
	MTG_Player* playerNext(const MTG_Player *aPlayer) const;

	IDObserver addObserver(const MTG_Observer &aObserver);
	bool delObserver(IDObserver aIDObserver);

	void clear();	//terminated
	bool start();
	bool stop();
	bool next();

protected:
	virtual void event(const MTG_Event *aEvent);

private:
	bool play(MTG_Player *aPlayer,const MTG_CardSet &aCards);

private:
	State mState;
	Runde mRunde;
	Phase mPhase;
	bool mLoopEvents;
	std::list<const MTG_Event*> mEvents;
	MTG_Player *mFirstPlayer, *mSecondPlayer, *mAttackPlayer;
	std::map<IDObserver,MTG_Observer> mObservers;
};

#endif //MTG_GAME_HPP