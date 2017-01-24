
#include "mtg_engine.hpp"
#include <iostream>

/*******************   MTG_ConsoleView   *********************************/

class MTG_ConsoleView : public MTG_View
{
public:
	MTG_ConsoleView();
	~MTG_ConsoleView();

protected:
	void changeRunde(Runde aRunde);
	void changePhase(Phase aPhase);
};

MTG_ConsoleView::MTG_ConsoleView()
	:MTG_View()
{

}

MTG_ConsoleView::~MTG_ConsoleView()
{

}

void MTG_ConsoleView::changeRunde(Runde aRunde) {
	std::cout << "New Runde:  " << aRunde << std::endl;
}

void MTG_ConsoleView::changePhase(Phase aPhase) {
	std::cout << "Change phase:  " << aPhase << std::endl;
}

/*******************   main   *********************************/

int main(int argv,char **argc)
{
	MTG_Robot robot1, robot2;
	MTG_Game game;

	MTG_ConsoleView view;
	view.setGame(&game);


	while (game.next()) {

	}

	return 0;
}