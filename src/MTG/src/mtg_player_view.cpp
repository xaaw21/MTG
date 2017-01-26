#include "mtg_player_view.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QFrame>

#define WIDTH_CARD 120
#define HEIGHT_CARD 150
#define SIZE_CARD QSize(WIDTH_CARD,HEIGHT_CARD)

/***************************   PanelView   **************************************/

class PanelView : public QFrame
{
public:
	PanelView(QWidget *aParent = 0);
	~PanelView();

};

PanelView::PanelView(QWidget *aParent)
	:QFrame(aParent)
{
	setFrameShape(QFrame::Box);
}

PanelView::~PanelView()
{

}

/***************************   BoardView   **************************************/

class BoardView : public PanelView
{
	Q_OBJECT
public:
	BoardView(QWidget *aParent);
	virtual ~BoardView();

	void setType(MTG_Card::State_t aType);
	MTG_Card::State_t type() const;

	void setSelectable(bool aSelectable);
	bool selectable() const;

	void update(const MTG_CardSet &aCards);
	void append();
	MTG_CardSet cards() const;
	MTG_CardSet selected() const;

	void clear();

Q_SIGNALS:
	void selectCard(IDCard_t aIDCard,bool aSelect);

protected:
	void paintEvent(QPaintEvent *aEvent);
	void mousePressEvent(QMouseEvent *aEvent);
	void resizeEvent(QResizeEvent *aEvent);
	void relocate();

private:

	struct Item
	{
		MTG_Card Card;
		bool Select;	
		QRectF Rect;
	};

	MTG_Card::State_t mType;
	bool mSelectable;
	std::list<Item> mCards;
};

BoardView::BoardView(QWidget *aParent)
	:PanelView(aParent),
	mType(MTG_Card::E_NoneState),
	mSelectable(false)
{

}

BoardView::~BoardView()
{

}

void BoardView::setType(MTG_Card::State_t aType) {
	mType = aType;
}

MTG_Card::State_t BoardView::type() const {
	return mType;
}

void BoardView::setSelectable(bool aSelectable) {
	mSelectable = aSelectable;
}

bool BoardView::selectable() const {
	return mSelectable;
}
void BoardView::update(const MTG_CardSet &aCards) {
	MTG_CardSet cards_update = aCards;
	//Проходим по имеющимся картам и проверяем на их актуальность - лишние удаляем
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		bool find = false;
		for (auto it_update = cards_update.begin(), end_update = cards_update.end(); it_update != end_update; it_update++) {
			if (it_cards->Card.ID == it_update->ID) {
				find = true;
				cards_update.erase(it_update);
				break;
			}
		}

		if (!find) {
			it_cards = mCards.erase(it_cards);
			if (it_cards == end_cards) break;
		}
	}

	//Добавляем новые карты
	for (auto it_update = cards_update.begin(), end_update = cards_update.end(); it_update != end_update; it_update++) {
		Item item;
		item.Card = *it_update;
		item.Select = false;
		item.Rect.setSize(SIZE_CARD);		
		mCards.push_back(item);
	}

	relocate();
}

MTG_CardSet BoardView::cards() const {
	MTG_CardSet cards;
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		cards.push_back(it_cards->Card);
	}

	return cards;
}

MTG_CardSet BoardView::selected() const {
	MTG_CardSet cards;
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		if (it_cards->Select) cards.push_back(it_cards->Card);
	}

	return cards;
}

void BoardView::clear() {
	mCards.clear();
}

void BoardView::paintEvent(QPaintEvent *aEvent) {

	static std::function<void(QPainter*,const Item&)> paintItem = [](QPainter *aPainter, const Item &aItem) -> void {
		QRectF rect = aItem.Rect;
		if (aItem.Select) rect.adjust(0,-10,0,-10);
		aPainter->drawRect(rect);
		aPainter->drawText(rect,QString::number(aItem.Card.ID),QTextOption(Qt::AlignCenter));
	};

	QPainter painter(this);
	//painter.setBrush(Qt::red);
	painter.drawRect(this->rect());
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		paintItem(&painter,*it_cards);
	}
}

void BoardView::mousePressEvent(QMouseEvent *aEvent) {
	if (!mSelectable) return;
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		if (it_cards->Rect.contains(aEvent->pos())) {
			it_cards->Select = !it_cards->Select;
			emit selectCard(it_cards->Card.ID, it_cards->Select);
			repaint();
			break;
		}
	}
}

void BoardView::resizeEvent(QResizeEvent *aEvent) {
	relocate();
}

void BoardView::relocate() {
	QRect geometry = this->rect();
	geometry.adjust(15,0,-15,0);
	geometry.setHeight(HEIGHT_CARD);
	geometry.moveCenter(this->rect().center());

	
	double need_width = mCards.size()  * WIDTH_CARD + (mCards.size() - 1) * 5;
	double shift = WIDTH_CARD + 5;
	double space = geometry.width() - need_width;
	if (space < 0) {
		shift -= space / (mCards.size());
	}

	double x = geometry.x();
	double y = geometry.y();
	for (auto it_cards = mCards.begin(), end_cards = mCards.end(); it_cards != end_cards; it_cards++) {
		it_cards->Rect.moveTo(x,y);
		x += shift;
	}

	this->repaint();
}

/***************************   StatusView   **************************************/

class StatusView : public PanelView
{
public:
	StatusView(QWidget *aParent = 0);
	~StatusView();

};

StatusView::StatusView(QWidget *aParent)
	:PanelView(aParent)
{

}

StatusView::~StatusView()
{

}

/***************************   proView   **************************************/


/***************************   MTG_PlayerView   **************************************/

#define STATUS_PANEL_HEIGHT 15
#define OPEN_PANEL_HEIGHT (HEIGHT_CARD + 20)

enum Panel_t
{
	E_BoardOpenPanel = 0,
	E_BoardInvokcationPanel,
	E_BoardProtectedPanel,
	E_BoardAttackPanel,
	E_StatusPanel
};

MTG_PlayerView::MTG_PlayerView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mPlayer(0),
	mOrientation(E_TopOrientation)
{

	BoardView *board_open = new BoardView(this);
	mPanels[E_BoardOpenPanel] = board_open;
	board_open->setSelectable(true);

	BoardView *board_invocation = new BoardView(this);
	mPanels[E_BoardInvokcationPanel] = board_invocation;

	BoardView *board_protected = new BoardView(this);
	mPanels[E_BoardProtectedPanel] = board_protected;

	BoardView *board_attack = new BoardView(this);
	mPanels[E_BoardAttackPanel] = board_attack;

	StatusView *status_view = new StatusView(this);
	mPanels[E_StatusPanel] = status_view;


}

MTG_PlayerView::~MTG_PlayerView()
{

}

void MTG_PlayerView::setPlayer(MTG_Player *aPlayer) {
	mPlayer = aPlayer;
	//TODO: так делать плохо
	MTG_Observer::setGame(const_cast<MTG_Game*>(mPlayer->game()));
}

MTG_Player *MTG_PlayerView::player() const {
	return mPlayer;
}

void MTG_PlayerView::setOrientation(Orientation_t aOrientation) {
	if (aOrientation == mOrientation) return;
	mOrientation = aOrientation;
	updateGeometry();
}

MTG_PlayerView::Orientation_t MTG_PlayerView::orientation() const {
	return mOrientation;
}	

void MTG_PlayerView::changeGame(MTG_Game *aGame) {

}

void MTG_PlayerView::gameEvent(MTG_Game::State_t aState) {

}

void MTG_PlayerView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	update();
}

void MTG_PlayerView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	update();
}

void MTG_PlayerView::winEvent(MTG_Player *aPlayerWin) {

}

void MTG_PlayerView::resizeEvent(QResizeEvent *aEvent) {
	updateGeometry();
}

void MTG_PlayerView::updateGeometry() {
	QSize size = this->size();
	switch (mOrientation)
	{
	case E_TopOrientation: {
		mPanels[E_StatusPanel]->setGeometry(0,size.height() - STATUS_PANEL_HEIGHT,size.width(), STATUS_PANEL_HEIGHT);

		mPanels[E_BoardOpenPanel]->setGeometry(0, mPanels[E_StatusPanel]->y() - OPEN_PANEL_HEIGHT, size.width(), OPEN_PANEL_HEIGHT);

		double height = (size.height() - (OPEN_PANEL_HEIGHT + STATUS_PANEL_HEIGHT)) / 2;
		double width_protected = size.width() * (3. / 5.);
		
		mPanels[E_BoardProtectedPanel]->setGeometry(0,mPanels[E_BoardOpenPanel]->geometry().y() - height, width_protected, height);

		double width_invocation = size.width() - width_protected;
		mPanels[E_BoardInvokcationPanel]->setGeometry(width_protected, mPanels[E_BoardProtectedPanel]->geometry().y(), width_invocation, height);

		mPanels[E_BoardAttackPanel]->setGeometry(0, mPanels[E_BoardProtectedPanel]->geometry().y() - height, size.width(), height);

		break;
	}
	case E_BottomOrientation: {

		break;
	}
	}
}


void MTG_PlayerView::update() {
	((BoardView*)mPanels[E_BoardOpenPanel])->update(mPlayer->cards(MTG_Card::E_OpenState));
	((BoardView*)mPanels[E_BoardInvokcationPanel])->update(mPlayer->cards(MTG_Card::E_InvocationState));
	((BoardView*)mPanels[E_BoardProtectedPanel])->update(mPlayer->cards(MTG_Card::E_ProtectionState));
	((BoardView*)mPanels[E_BoardAttackPanel])->update(mPlayer->cards(MTG_Card::E_AttackState));
}

#include "mtg_player_view.moc"