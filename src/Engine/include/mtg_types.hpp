#ifndef MTG_TYPES_HPP
#define MTG_TYPES_HPP

#include "mtg_global.hpp"
#include <cstdint>
#include <array>
#include <list>
#include <vector>
#include <map>

#define INVALID_ID_CARD -1
#define COUNT_CARDS 20
#define IS_VALID_ID_CARD(ID) (ID > 0 && ID <= COUNT_CARDS)

typedef int8_t IDCard_t;
typedef int8_t Health_t;
typedef uint16_t Mana_t;
typedef uint16_t Round_t;

enum Phase_t
{
	E_NonePhase = 0,
	E_StartPhase,
	E_InvocationPhase,
	E_AttackPhase,
	E_FinishPhase
};

enum Role_t
{
	E_NoneRole = 0,
	E_AttackRole,
	E_ProtectionRole
};

struct Card_t
{
	const uint8_t Attack;
	const uint8_t Protection;
	const uint8_t Cost;
};

static const std::array<Card_t, COUNT_CARDS> CARDS = { {
	{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },
	{ 2,1,2 },{ 2,1,2 },{ 2,1,2 },
	{ 1,3,2 },{ 1,3,2 },{ 1,3,2 },{ 1,3,2 },
	{ 2,4,3 },{ 2,4,3 },
	{ 3,2,3 },{ 3,2,3 },{ 3,2,3 },
	{ 4,3,4 },
	{ 3,5,4 },{ 3,5,4 }
} };

MTG_ENGINE_EXPORT Card_t CardFromID(IDCard_t aIDCard,bool *ok = 0);
MTG_ENGINE_EXPORT Phase_t NextPhase(Phase_t aPhase);


class MTG_ENGINE_EXPORT MTG_Card
{
public:
	//Type_t
	enum State_t
	{
		E_NoneState = 0,
		E_OpenState,
		E_InvocationState,
		E_ProtectionState,
		E_AttackState,
		E_DeadState
	};

	MTG_Card(State_t aState = E_OpenState);
	MTG_Card(IDCard_t aIDCard, State_t aState = E_OpenState);
	~MTG_Card();

	uint8_t attack() const;
	uint8_t protection() const;
	uint8_t cost() const;

	operator IDCard_t() const;
	operator Card_t() const;

	//Data
	IDCard_t ID;
	State_t State;
	Health_t Health;
};

class MTG_ENGINE_EXPORT MTG_CardSet : public std::list<MTG_Card>
{
public:
	MTG_CardSet();
	~MTG_CardSet();

	MTG_CardSet cards(MTG_Card::State_t aState) const;
};

class MTG_ENGINE_EXPORT MTG_Deck : public std::vector<IDCard_t>
{
public:
	MTG_Deck();
	~MTG_Deck();

	IDCard_t card();
	MTG_CardSet cards(uint8_t aCount = 1);
	void reset();
	void disturb();
};

class MTG_Player;

typedef std::map<MTG_Player*, MTG_CardSet> MTG_CardMap;

#endif //MTG_TYPES_HPP