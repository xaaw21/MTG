#ifndef MTG_CARDS_VIEW_HPP
#define MTG_CARDS_VIEW_HPP

#include <QWidget>
#include <QList>
#include "mtg_engine.hpp"

using namespace Engine;

/*************************************
 *
 * Графическое представление набора карт 
 *
 ************************************/

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

	//TODO: поддерживаются только HCenter и HLeft
	void setAlignment(Qt::Alignment aAlignment);
	Qt::Alignment alignment() const;

public Q_SLOTS:
	bool setChecked(IDCard_t aIDCard, bool aChecked);
	void clear();
	void refresh();

Q_SIGNALS:
	void checked(IDCard_t aIDCard, bool aChecked);

protected:
	void paintEvent(QPaintEvent *aEvent);
	void mousePressEvent(QMouseEvent *aEvent);
	void mouseMoveEvent(QMouseEvent *aEvent);
	void leaveEvent(QEvent *aEvent);
	void resizeEvent(QResizeEvent *aEvent);
	void relocate();

private:

	struct Item
	{
		MTG_Card Card;
		bool Checked;
		QRectF Rect;
	};

	bool mCheckable;
	QList<Item> mItems;
	IDCard_t mIDEnter;
	Qt::Alignment mAlignment;
};


#endif //MTG_CARDS_VIEW_HPP