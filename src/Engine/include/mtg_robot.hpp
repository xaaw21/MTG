#ifndef MTG_ROBOT_HPP
#define MTG_ROBOT_HPP

#include "mtg_player.hpp"

class MTG_ENGINE_EXPORT MTG_Robot : public MTG_Player
{
public:
	MTG_Robot();
	~MTG_Robot();

protected:
	bool invocation();
	bool attack();				    
	bool protection(const MTG_CardSet &aAttackCards);
};

#endif //MTG_ROBOT_HPP