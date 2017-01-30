#include <QApplication>

#include "mtg_game_view.hpp"

//#define GAME_ROBOTS - если обьявлен - то играть будут два робота меджу собой

int main(int argv,char **argc)
{
	QApplication app(argv, argc);

	//создаем игрока - робота
	MTG_Robot robot;
	robot.setName("Робот");

#ifdef GAME_ROBOTS
	//создаем игрока - робота
	MTG_Robot robot2;
	robot2.setName("Робот_2");
#else
	//создаем игрока - человека
	MTG_Human human;						                  
	human.setName("Человек");
#endif

	//создаем игру
	MTG_Game game;
#ifdef GAME_ROBOTS
	//сажаем игроков за игру
	game.setPlayers(&robot, &robot2);
#else
	//сажаем игроков за игру
	game.setPlayers(&human, &robot);
#endif

	//создаем ототбражение игры
	MTG_GameView view;
	//устанавливаем игру для оттображения
	view.setGame(&game);
	view.showMaximized();
	//view.show();

	return app.exec();
}