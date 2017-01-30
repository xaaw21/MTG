#ifndef MTG_EVENTS_HPP
#define MTG_EVENTS_HPP

#include "mtg_types.hpp"

class MTG_Player;
class MTG_Game;

/*************************************
 *
 * Здесь собраны все события которые может отправлять игра(MTG_Game) своим наблюдателям
 *
 ************************************/

//Базовый класс события
class MTG_ENGINE_EXPORT MTG_Event
{
	friend MTG_Game;
public:

	enum Type
	{
		E_NoneEvent = 0,
		E_GameEvent,
		E_PhaseEvent,
		E_PlayerEvent,
		E_WinEvent
	};

	virtual ~MTG_Event();

	Type type() const;
	const MTG_Game *game() const;

protected:
	MTG_Event(Type aType);

private:
	Type mType;
	MTG_Game *mGame;
};

//Сосбытие состояния игры(Старт - Стоп)
class MTG_ENGINE_EXPORT MTG_GameEvent : public MTG_Event
{
public:
	MTG_GameEvent(int aState);
	~MTG_GameEvent();

	int state() const;

private:
	int mState;
};

//Событие начала новой фазы
class MTG_ENGINE_EXPORT MTG_PhaseEvent : public MTG_Event
{
public:
	MTG_PhaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards = MTG_CardMap());
	~MTG_PhaseEvent();

	Phase_t phase() const;
	Round_t round() const;
	MTG_CardMap cards() const;

private:
	Phase_t mPhase;
	Round_t mRound;
	MTG_CardMap mCards;
};

//Событие игрока - игрок походил
class MTG_ENGINE_EXPORT MTG_PlayerEvent : public MTG_Event
{
public:
	MTG_PlayerEvent(Phase_t aPhase, MTG_Player *aPlayer,const MTG_CardSet &aCards);
	~MTG_PlayerEvent();

	Phase_t phase() const;
	MTG_CardSet cards() const;
	MTG_Player* player() const;

private:
	MTG_Player *mPlayer;
	Phase_t mPhase;
	MTG_CardSet mCards;
};

//Событие обозначающее победу одного из игроков, либо ничью
class MTG_ENGINE_EXPORT MTG_WinEvent : public MTG_Event
{
public:
	MTG_WinEvent(MTG_Player *aWinPlayer);
	~MTG_WinEvent();

	MTG_Player* playerWin() const;

private:
	MTG_Player *mWinPlayer;
};


#endif //MTG_EVENTS_HPP