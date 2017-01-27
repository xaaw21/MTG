#ifndef MTG_CARDS_VIEW_HPP
#define MTG_CARDS_VIEW_HPP

#include <QWidget>
#include <QList>
#include "mtg_engine.hpp"

class MTG_CardsView	: public QWidget
{
	Q_OBJECT
public:
	MTG_CardsView(QWidget *aParent = 0);
	virtual ~MTG_CardsView();

	void setCards(const MTG_CardSet &aCards);
	MTG_CardSet cards() const;
	MTG_CardSet checkedCards() const;

	void setCheckable(bool aCheckable);
	bool isCheckable() const;

public Q_SLOTS:
	bool setChecked(IDCard_t aIDCard, bool aChecked);
	void clear();
	void refresh();

Q_SIGNALS:
	void checked(IDCard_t aIDCard, bool aChecked);

protected:
	void paintEvent(QPaintEvent *aEvent);
	void mousePressEvent(QMouseEvent *aEvent);
	void resizeEvent(QResizeEvent *aEvent);
	void relocate();

private:

	struct Item
	{
		MTG_Card Card;
		bool Checked;
		QRectF Rect;
	};

	MTG_Card::State_t mType;
	bool mCheckable;
	QList<Item> mItems;
};


#endif //MTG_CARDS_VIEW_HPP