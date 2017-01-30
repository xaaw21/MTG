#ifndef MTG_ROBOT_HPP
#define MTG_ROBOT_HPP

#include "mtg_player.hpp"

namespace Engine
{

	/*************************************
	 *
	 * Реализация Игрока - Робота с примитивной логикой игры
	 *
	 ************************************/

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

};//namespace Engine

#endif //MTG_ROBOT_HPP