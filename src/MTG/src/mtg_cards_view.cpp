#include "mtg_cards_view.hpp"

#include <QPainter>
#include <QMouseEvent>

#define WIDTH_CARD 120
#define HEIGHT_CARD 150
#define SIZE_CARD QSize(WIDTH_CARD,HEIGHT_CARD)


QImage ImageCards[COUNT_CARDS];

MTG_CardsView::MTG_CardsView(QWidget *aParent)
	:QWidget(aParent),
	mCheckable(false),
	mIDEnter(INVALID_ID_CARD),
	mAlignment(Qt::AlignLeft)
{
	static bool init = false;
	if (!init) {
		for (int i = 0; i < COUNT_CARDS; i++) {
			ImageCards[i] = QImage(QString(":/Resources/cards/%1.png").arg(i));
		}
		init = true;
	}

	this->setMouseTracking(true);
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

void MTG_CardsView::setAlignment(Qt::Alignment aAlignment) {
	mAlignment = aAlignment;
	relocate();
}

Qt::Alignment MTG_CardsView::alignment() const {
	return mAlignment;
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
	this->repaint();
}

void MTG_CardsView::refresh() {
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		it_cards->Checked = false;
	}

	this->repaint();
}

#define CHECK_CARD_SHIFT 10

void MTG_CardsView::paintEvent(QPaintEvent *aEvent) {

	static std::function<void(QPainter*, const Item&)> paintItem = [](QPainter *aPainter, const Item &aItem) -> void {
		QRectF rect = aItem.Rect;
		if (aItem.Checked) rect.adjust(0, -CHECK_CARD_SHIFT, 0, -CHECK_CARD_SHIFT);
		aPainter->drawImage(rect, ImageCards[aItem.Card.ID]);
		rect.adjust(5,95,-5,-15);

		Card_t card = CardFromID(aItem.Card.ID);
		
		double shift = rect.height() / 3;
		rect.setHeight(shift);

		aPainter->drawText(rect, QString("Attack: %1").arg(card.Attack), QTextOption(Qt::AlignCenter));
		rect.translate(0, shift);

		aPainter->save();
		
		if (aItem.Card.Health != aItem.Card.protection()) aPainter->setPen(Qt::red);
		if (aItem.Card.Health <= 0) {
			QFont f = aPainter->font();
			f.setBold(true);
			aPainter->setFont(f);
			aPainter->drawText(rect, QString("Health: Dead"), QTextOption(Qt::AlignCenter));
		}
		else aPainter->drawText(rect, QString("Health: %1").arg(aItem.Card.Health), QTextOption(Qt::AlignCenter));

		aPainter->restore();

		rect.translate(0, shift);
		aPainter->drawText(rect, QString("Cost: %1").arg(card.Cost), QTextOption(Qt::AlignCenter));
	};

	QPainter painter(this);
	Item item_enter;
	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		if (it_cards->Card.ID == mIDEnter) {
			item_enter = *it_cards;
			continue;
		}
		paintItem(&painter, *it_cards);
	}

	if (IS_VALID_ID_CARD(mIDEnter)) {
		paintItem(&painter, item_enter);
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

	QWidget::mousePressEvent(aEvent);
}

void MTG_CardsView::mouseMoveEvent(QMouseEvent *aEvent) {
	for (auto it_cards = mItems.rbegin(), end_cards = mItems.rend(); it_cards != end_cards; it_cards++) {
		if (it_cards->Rect.contains(aEvent->pos())) {
			if (mIDEnter == it_cards->Card.ID) return;
			mIDEnter = it_cards->Card.ID;
			repaint();
			return;
		}
	}

	if (IS_VALID_ID_CARD(mIDEnter)) {
		mIDEnter = INVALID_ID_CARD;
		repaint();
	}
	

	QWidget::mouseMoveEvent(aEvent);
}

void MTG_CardsView::leaveEvent(QEvent *aEvent) {
	if (IS_VALID_ID_CARD(mIDEnter)) {
		mIDEnter = INVALID_ID_CARD;
		repaint();
	}

	QWidget::leaveEvent(aEvent);
}

void MTG_CardsView::resizeEvent(QResizeEvent *aEvent) {
	relocate();
}

#define SPACE_BETWEEN_CARDS 10

void MTG_CardsView::relocate() {
	if (mItems.isEmpty()) {
		this->update();
		return;
	}

	QRect geometry = this->rect();
	geometry.setHeight(HEIGHT_CARD);
	geometry.moveCenter(this->rect().center());

	double x = geometry.x();
	double y = geometry.y();

	double width_card = WIDTH_CARD + SPACE_BETWEEN_CARDS;
	double need_width = width_card * mItems.size();

	if (mAlignment == Qt::AlignHCenter) {
		double shift = (geometry.width() - need_width) / 2;
		if (shift > 0) x += shift;
	}

	double space_card = (geometry.width() - width_card )/ mItems.size();

	double shift = width_card;
	if (space_card < width_card) shift = space_card;

	for (auto it_cards = mItems.begin(), end_cards = mItems.end(); it_cards != end_cards; it_cards++) {
		it_cards->Rect.moveTo(x, y);
		x += shift;
	}

	this->update();
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