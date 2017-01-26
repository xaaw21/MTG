#include "mtg_log_view.hpp"

MTG_LogView::MTG_LogView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer()
{
	ui.setupUi(this);
}

MTG_LogView::~MTG_LogView()
{

}