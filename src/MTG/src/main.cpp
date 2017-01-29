#include <QApplication>

#include "mtg_game_view.hpp"

#define GAME_ROBOTS

int main(int argv,char **argc)
{
	QApplication app(argv, argc);

	MTG_Robot robot;
	robot.setName("Робот");

#ifdef GAME_ROBOTS
	MTG_Robot robot2;
	robot2.setName("Робот_2");
#else
	MTG_Human human;						                  
	human.setName("Человек");
#endif

	MTG_Game game;
#ifdef GAME_ROBOTS
	game.setPlayers(&robot, &robot2);
#else
	game.setPlayers(&human, &robot);
#endif

	MTG_GameView view;
	view.setGame(&game);
	view.showMaximized();
	//view.show();
	

   
	MTG_Deck deck;
	MTG_CardsView cards_view;
	cards_view.setCards(deck.cards(4));
	cards_view.setAlignment(Qt::AlignHCenter);
	//cards_view.show();
	//view.resize(1000,1000);
  


	return app.exec();
}