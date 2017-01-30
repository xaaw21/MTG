
#include "mtg_engine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace Engine;

std::string PhaseToString(Phase_t aPhase) {
	switch (aPhase)
	{
	case E_StartPhase: return "Start";
	case E_InvocationPhase:	return "Invocation";
	case E_AttackPhase:	return "Attack";
	case E_FinishPhase:	return "Finish";
	default: break;
	}

	return std::string();
}

std::string RoleToString(Role_t aRole) {
	switch (aRole)
	{
	case E_AttackRole: return "Attack";
	case E_ProtectionRole: return "Protected";
	default: break;
	}

	return std::string();
}

std::string SetToString(const MTG_CardSet &aCards) {
	if (aCards.empty()) return "None";
	std::string str;
	static std::function<std::string(MTG_Card)> CardToString = [](MTG_Card aCard) -> std::string {
		return "{ ID(" + std::to_string(aCard.ID) + ") A:" + std::to_string(aCard.attack()) + " P:"+ std::to_string(aCard .protection()) + " C:" + std::to_string(aCard.cost())  + " }";
	};

	for (auto it = aCards.begin(), end = aCards.end(); it != end; it++) {
		str += CardToString(*it) + " ";
	}	
	return str;
}

/*******************   MTG_ConsoleView   *********************************/

class MTG_ConsoleView : public MTG_Observer
{
public:
	MTG_ConsoleView();
	~MTG_ConsoleView();

protected:
	void changeGame(MTG_Game *aGame);
	void gameEvent(MTG_Game::State_t aState);
	void phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards);
	void playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards);
	void winEvent(MTG_Player *aPlayerWin);
};

MTG_ConsoleView::MTG_ConsoleView()
	:MTG_Observer()
{

}

MTG_ConsoleView::~MTG_ConsoleView()
{

}

void MTG_ConsoleView::changeGame(MTG_Game *aGame) {
	UNUSED(aGame);
}

void MTG_ConsoleView::gameEvent(MTG_Game::State_t aState) {
	switch (aState)
	{
	case MTG_Game::E_StartState: {
		std::cout << "*******************   Game Start               *********************" << std::endl << std::endl;
		std::cout << "Players: " << game()->players().first->name() << " and "<< game()->players().second->name()  << std::endl;
		std::cout << std::endl;
		break;
	}
	case MTG_Game::E_StopState: std::cout <<  "*******************   Game Stop                *********************" << std::endl << std::endl;break;
   }
}

void MTG_ConsoleView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {

	switch(aPhase)
	{
	case E_StartPhase: {
		std::cout << "*******************   Start Phase(Round - " << (int)aRound << ")   *********************" << std::endl << std::endl;
		for (auto it = aCards.begin(), end = aCards.end(); it != end; it++) {
			std::cout << "Player: " << it->first->name() << " - " << RoleToString(it->first->role()) << std::endl;
			std::cout << "      Properties:       " << "Health - " << (int)it->first->health() << " Mana - " << (int)it->first ->mana() << std::endl;
			std::cout << "      New Cards:        " << SetToString(it->second) << "   Deck(" << it->first->deck().size() << ")" << std::endl;
			std::cout << "      Open Cards:       " << SetToString(it->first->cards(MTG_Card::E_OpenState)) << std::endl;
			std::cout << "      Invocation Cards: " << SetToString(it->first->cards(MTG_Card::E_InvocationState)) << std::endl;
			std::cout << "      Protected Cards:  " << SetToString(it->first->cards(MTG_Card::E_ProtectionState)) << std::endl;
			std::cout << std::endl;
		}
		break;
	}
	case E_InvocationPhase: std::cout << "*******************   Invocation Phase         *********************" << std::endl << std::endl;break;
	case E_AttackPhase:	std::cout << "*******************   Attack Phase             *********************" << std::endl << std::endl; break;
	case E_FinishPhase: {
		std::cout << "*******************   Finish Phase             *********************" << std::endl << std::endl;
		for (auto it = aCards.begin(), end = aCards.end(); it != end; it++) {
			std::cout << "Player: " << it->first->name() << " - " << RoleToString(it->first->role()) << std::endl;
			std::cout << "      Properties:       " << "Health - " << (int)it->first->health() << " Mana - " << (int)it->first->mana() << std::endl;
			std::cout << "      Dead Cards:        " << SetToString(it->second.cards(MTG_Card::E_DeadState)) << std::endl;
			std::cout << std::endl;
		}
		std::cout << std::endl;
		break;
	}
	default: break;
	}
}

void MTG_ConsoleView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) { 
	UNUSED(aPhase);

	std::cout << "Play("<< aPlayer ->name() << "): " << RoleToString(aPlayer->role()) << " - " << SetToString(aCards) << std::endl << std::endl;
}

void MTG_ConsoleView::winEvent(MTG_Player *aPlayerWin) {
	std::cout << "*******************   Player Win:             *********************  " << std::endl << std::endl;
	if(aPlayerWin) std::cout << "Winner: " << aPlayerWin->name() << std::endl;
	std::cout << std::endl;
}


/*******************   main   *********************************/



int main(int argv,char **argc)
{
	UNUSED(argv);
	UNUSED(argc);

	MTG_Robot robot1, robot2;
	robot1.setName("Robot 1");
	robot2.setName("Robot 2");

	MTG_Game game;
	game.setPlayers(&robot1,&robot2);

	MTG_ConsoleView view;
	view.setGame(&game);

	std::chrono::milliseconds msec(500);

	if (game.start()) {
		while (::E_NonePhase != game.next()) { std::this_thread::sleep_for(msec); }
	}
	else {
		std::cout << "Game not started" << std::endl;
	}

	std::cout << "Press any key to exit...";
	std::cin.get();

	return 0;
}