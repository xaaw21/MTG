#ifndef MTG_GAME_HPP
#define MTG_GAME_HPP

#include "mtg_types.hpp"
#include <functional>
#include <map>
#include <utility>

#define INVALID_ID_OBSERVER -1

class MTG_Player;
class MTG_Event;

typedef int IDObserver_t;
typedef std::function<void(const MTG_Event*)> Observer_t;

class MTG_ENGINE_EXPORT MTG_Game
{
	friend MTG_Player;
public:
	MTG_Game();
	virtual ~MTG_Game();

	enum State_t
	{
		E_StopState = 0,
		E_StartState
	};

	State_t state() const;
	Round_t round() const;
	Phase_t phase() const;
	
	bool setPlayers(MTG_Player *aFirstPlayer, MTG_Player *aSecondPlayer);
	std::pair<MTG_Player*, MTG_Player*> players() const;
	MTG_Player* player(Role_t aRole) const;
	MTG_Player* playerNext(const MTG_Player *aPlayer) const;

	IDObserver_t addObserver(const Observer_t &aObserver);
	bool delObserver(IDObserver_t aIDObserver);

	void clear();	//terminated
	bool start();
	bool stop();
	Phase_t next();

protected:
	virtual void event(const MTG_Event *aEvent);

private:
	bool play(MTG_Player *aPlayer,const MTG_CardSet &aCards);

private:
	State_t mState;
	Round_t mRound;
	Phase_t mPhase;
	bool mLoopEvents;
	std::list<const MTG_Event*> mEvents;
	std::pair<MTG_Player*, MTG_Player*> mPlayers;
	std::map<IDObserver_t, Observer_t> mObservers;
};

#endif //MTG_GAME_HPP