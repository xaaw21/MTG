#include "mtg_log_view.hpp"

#include <QItemDelegate>
#include <QTime>
#include <QPainter>


class LogDelegate : public QItemDelegate
{
public:
	LogDelegate() {}
	~LogDelegate() {}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		//option.rect
		//painter->setPen(Qt::white);
		//painter->setBrush(QColor(255,242,157,150));
		
		//painter->drawRoundedRect(option.rect, 8, 8);
		painter->setRenderHint(QPainter::Antialiasing, true);
		//QImage img = QImage(":/Resources/simple-button.png");
		//img = img.scaled(option.rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
		//painter->drawImage(option.rect, img);

		QRect rect = option.rect.adjusted(5,0,-5,0);
		painter->setPen(QPen(QColor(142,155,188),2));
		painter->setBrush(QColor(219,227,231));
		painter->drawRoundedRect(rect,2,2);

		painter->setPen(QColor(41, 57, 85));
		painter->drawText(rect, index.data().toString(),QTextOption(Qt::AlignCenter));
	}
};


MTG_LogView::MTG_LogView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer()
{
	ui.setupUi(this);
	ui.History->setItemDelegate(new LogDelegate);
}

MTG_LogView::~MTG_LogView()
{

}

void MTG_LogView::changeGame(MTG_Game *aGame) {
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
	case E_StartPhase: print("Начало боя"); break;
	case E_InvocationPhase:	print("Призыв существ"); break;
	case E_AttackPhase: print("Подготовка к бою"); break;
	case E_FinishPhase:	print("Завершения боя"); break;
	}
}

void MTG_LogView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	print(QString("Игрок походил: %1").arg(QString::fromStdString(aPlayer->name())));
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

