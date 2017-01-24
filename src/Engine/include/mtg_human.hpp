#ifndef MTG_HUMAN_HPP
#define MTG_HUMAN_HPP

#include "mtg_player.hpp"

class MTG_ENGINE_EXPORT MTG_Human : public MTG_Player
{
public:
	MTG_Human();
	~MTG_Human();

protected:
	bool invocation();
	bool attack();
	bool protection(const MTG_CardSet &aAttackCards);
};

#endif //MTG_ROBOT_HPP