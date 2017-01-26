#ifndef MTG_LOG_VIEW_HPP
#define MTG_LOG_VIEW_HPP

#include <QWidget>
#include "ui_mtg_log_view.h"
#include "mtg_engine.hpp"

class MTG_LogView : public QWidget, public MTG_Observer
{
public:
	MTG_LogView(QWidget *aParent = 0);
	~MTG_LogView();

private:
	Ui::LogView ui;
};


#endif //MTG_LOG_VIEW_HPP