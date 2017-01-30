#include "mtg_log_view.hpp"

#include <QItemDelegate>
#include <QTime>
#include <QPainter>

/**********************************    Function   ******************************************/

QString CardsToString(const MTG_CardSet &aCards) {
	if (aCards.empty()) return "None";
	QString str;
	for (auto it = aCards.begin(), end = aCards.end(); it != end; it++) {
		Card_t card = CardFromID(it->ID);
		str += QString("{A: %1, H: %2, C: %3}\n").arg(card.Attack).arg(it->Health).arg(card.Cost);
	}	
	return str;
}

QString CardsToString(const MTG_CardMap &aCards) {
	QString str;
	for (auto it = aCards.begin(), end = aCards.end(); it != end; it++) {
		str += QString("%1:\n%2").arg(QString::fromStdString(it->first->name())).arg(CardsToString(it->second));
	}
	return str;
}


/**********************************    LogDelegate   ******************************************/

class LogDelegate : public QItemDelegate
{
public:
	LogDelegate(QObject *aParent = 0):QItemDelegate(aParent){}
	~LogDelegate() {}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		QRect rect = option.rect.adjusted(5,0,-5,0);
		painter->setPen(QPen(QColor(142,155,188),2));
		painter->setBrush(QColor(219,227,231));
		painter->drawRoundedRect(rect,2,2);

		painter->setPen(QColor(41, 57, 85));
		painter->drawText(rect, index.data().toString(),QTextOption(Qt::AlignCenter));
	}
};

/**********************************    MTG_LogView   ******************************************/

MTG_LogView::MTG_LogView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer()
{
	ui.setupUi(this);
	ui.History->setItemDelegate(new LogDelegate(this));
}

MTG_LogView::~MTG_LogView()
{

}

void MTG_LogView::changeGame(MTG_Game *aGame) {
	UNUSED(aGame);
	refresh();
}

void MTG_LogView::gameEvent(MTG_Game::State_t aState) {
	switch (aState)
	{
	case MTG_Game::E_StartState: {
		refresh();
		print("Начало игры");
		break;
	}
	case MTG_Game::E_StopState: {
		print("Конец игры");
		break;
	}
	}
}

void MTG_LogView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	switch (aPhase)
	{
	case E_StartPhase: print(QString("Фаза: Начало\nРаунд: %1\nНовые карты:\n%2").arg(aRound).arg(CardsToString(aCards))); break;
	case E_InvocationPhase:	print("Фаза: Призыв"); break;
	case E_AttackPhase: print("Фаза: Атака"); break;
	case E_FinishPhase:	print("Фаза: Завершение"); break;
	default: break;
	}
}

void MTG_LogView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	UNUSED(aPhase);

	print(QString("%1 походил:\n %2").arg(QString::fromStdString(aPlayer->name())).arg(CardsToString(aCards)));
}

void MTG_LogView::winEvent(MTG_Player *aPlayerWin) {
	if (aPlayerWin) print(QString("Победил: %1").arg(QString::fromStdString(aPlayerWin->name())));
	else print("Ничья");
}

void MTG_LogView::refresh() {
	ui.History->clear();
}

void MTG_LogView::print(const QString &aMessage) {
	QString str(QTime::currentTime().toString("hh:mm:ss") + "\n");
	str += aMessage;
	QListWidgetItem *item = new QListWidgetItem(str);
	item->setTextAlignment(Qt::AlignCenter);
	ui.History->addItem(item);
	ui.History->scrollToItem(item);
}

