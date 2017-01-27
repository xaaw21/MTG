#include "mtg_cards_view.hpp"

#include <QPainter>
#include <QMouseEvent>

#define WIDTH_CARD 120
#define HEIGHT_CARD 150
#define SIZE_CARD QSize(WIDTH_CARD,HEIGHT_CARD)

MTG_CardsView::MTG_CardsView(QWidget *aParent)
	:QWidget(aParent)
{

}

MTG_CardsView::~MTG_CardsView()
{

}

void MTG_CardsView::setCards(const MTG_CardSet &aCards) {
	mItems.clear();
	for (auto it_cards = aCards.begin(), end_cards = aCards.end(); it_cards != end_cards; it_cards++) {
		Item item;
		item.Card = *it_cards;
		item.Checked = false;
		item.Rect.setSize(SIZE_CARD);
		mItems.push_back(item);
	}

	relocate();
}

MTG_CardSet MTG_CardsView::cards() const {
	MTG_CardSet cards;
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		cards.push_back(it_cards->Card);
	}

	return cards;
}

MTG_CardSet MTG_CardsView::checkedCards() const {
	MTG_CardSet cards;
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		if (it_cards->Checked) cards.push_back(it_cards->Card);
	}

	return cards;
}

void MTG_CardsView::setCheckable(bool aCheckable) {
	mCheckable = aCheckable;
	refresh();
}

bool MTG_CardsView::isCheckable() const {
	return mCheckable;
}

bool MTG_CardsView::setChecked(IDCard_t aIDCard, bool aChecked) {
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		if (aIDCard == it_cards->Card.ID) {
			it_cards->Checked = aChecked;
			this->repaint();
			return true;
		}
	}

	return false;
}

void MTG_CardsView::clear() {
	mItems.clear();
}

void MTG_CardsView::refresh() {
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		it_cards->Checked = false;
	}

	this->repaint();
}

void MTG_CardsView::paintEvent(QPaintEvent *aEvent) {

	static std::function<void(QPainter*, const Item&)> paintItem = [](QPainter *aPainter, const Item &aItem) -> void {
		QRectF rect = aItem.Rect;
		if (aItem.Checked) rect.adjust(0, -10, 0, -10);
		aPainter->drawRect(rect);
		aPainter->drawText(rect, QString::number(aItem.Card.ID), QTextOption(Qt::AlignCenter));
	};

	QPainter painter(this);
	//painter.setBrush(Qt::red);
	painter.drawRect(this->rect());
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		paintItem(&painter, *it_cards);
	}
}

void MTG_CardsView::mousePressEvent(QMouseEvent *aEvent) {
	if (!mCheckable) return;
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		if (it_cards->Rect.contains(aEvent->pos())) {
			it_cards->Checked = !it_cards->Checked;
			emit checked(it_cards->Card.ID, it_cards->Checked);
			repaint();
			break;
		}
	}
}

void MTG_CardsView::resizeEvent(QResizeEvent *aEvent) {
	relocate();
}

void MTG_CardsView::relocate() {
	QRect geometry = this->rect();
	geometry.adjust(15, 0, -15, 0);
	geometry.setHeight(HEIGHT_CARD);
	geometry.moveCenter(this->rect().center());


	double need_width = mItems.size()  * WIDTH_CARD + (mItems.size() - 1) * 5;
	double shift = WIDTH_CARD + 5;
	double space = geometry.width() - need_width;
	if (space < 0) {
		shift -= space / (mItems.size());
	}

	double x = geometry.x();
	double y = geometry.y();
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		it_cards->Rect.moveTo(x, y);
		x += shift;
	}

	this->repaint();
}



/*

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

*/