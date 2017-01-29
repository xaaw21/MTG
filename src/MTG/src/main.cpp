#include <QApplication>

#include "mtg_game_view.hpp"

int main(int argv,char **argc)
{
	QApplication app(argv, argc);

	MTG_Robot robot;
	robot.setName("Robot");

	MTG_Robot robot2;
	robot2.setName("Robot2");

	MTG_Human human;
	human.setName("Human");

	MTG_Game game;
	game.setPlayers(&robot, &robot2);

	MTG_GameView view;
	view.setGame(&game);
	view.show();


	MTG_Deck deck;
	MTG_CardsView cards_view;
	cards_view.setCards(deck.cards(10));
	//cards_view.show();


	//view.resize(800,600);

	return app.exec();
}