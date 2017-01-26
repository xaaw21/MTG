#include <QApplication>

#include "mtg_game_view.hpp"

int main(int argv,char **argc)
{
	QApplication app(argv, argc);

	MTG_Robot robot;
	robot.setName("Robot");

	MTG_Robot robot2;
	robot.setName("Robot2");

	MTG_Human human;
	human.setName("Human");

	MTG_Game game;
	game.setPlayers(&robot, &robot2);

	MTG_GameView view;
	view.setGame(&game);
	view.show();

	game.start();
	game.next();

	//view.resize(800,600);

	return app.exec();
}