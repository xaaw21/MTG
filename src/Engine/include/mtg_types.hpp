#ifndef MTG_TYPES_HPP
#define MTG_TYPES_HPP

#include "mtg_global.hpp"
#include <cstdint>
#include <array>
#include <list>
#include <vector>

#define INVALID_ID_CARD 0
#define COUNT_CARDS 20
#define IS_VALID_ID_CARD(ID) (ID > 0 && ID <= COUNT_CARDS)

typedef uint8_t IDCard;
typedef int8_t Health;
typedef uint8_t Mana;
typedef uint16_t Runde;

struct MTG_Card
{
	const uint8_t ID;
	uint8_t Attack;
	int8_t	Protection;
	uint8_t Cost;

	void restore() {}
	bool valid() const { return true; }
};

static const std::array<MTG_Card, COUNT_CARDS> TYPE_CARDS = { {
	{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },{ 1,1,1 },
	{ 2,1,2 },{ 2,1,2 },{ 2,1,2 },
	{ 1,3,2 },{ 1,3,2 },{ 1,3,2 },{ 1,3,2 },
	{ 2,4,3 },{ 2,4,3 },
	{ 3,2,3 },{ 3,2,3 },{ 3,2,3 },
	{ 4,3,4 },
	{ 3,5,4 },{ 3,5,4 }
} };

MTG_ENGINE_EXPORT MTG_Card CardFromID(IDCard aIDCard);

template<typename T>
class MTG_CardSet : public std::list<T>
{
public:
	MTG_CardSet() {}
	~MTG_CardSet() {}

	bool push(const MTG_CardSet<T> &aCardSet) { return true; }
};

template<>
class MTG_ENGINE_EXPORT MTG_CardSet<IDCard> : public std::list<IDCard>
{
public:
	bool take(IDCard aIDCard) { return true; }
	bool push(IDCard aIDCard) { return true; }
};

template<>
class MTG_ENGINE_EXPORT MTG_CardSet<MTG_Card> : public std::list<MTG_Card>
{
public:
	bool take(IDCard aIDCard) { return true; }
	bool push(IDCard aIDCard) { return true; }
};


class MTG_ENGINE_EXPORT MTG_Deck
{
public:
	MTG_Deck() {}
	~MTG_Deck() {}

	IDCard pop() { return 0; }
	int count() const;
	bool empty() const;
	void reset() {}
	void disturb() {}

private:
	MTG_CardSet<IDCard> mCards;
};


enum CardState
{
	E_OpenCard = 0,
	E_CallCard,
	E_ProtectionCard,
	E_AttackCard,
	E_OpenClose
};

enum Phase
{
	E_NonePhase = 0,
	E_StartPhase,
	E_InvocationPhase,
	E_AttackPhase,
	E_FinishPhase
};


#endif //MTG_TYPES_HPP