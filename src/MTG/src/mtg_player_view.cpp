#include "mtg_player_view.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QFrame>

/***************************   MTG_PlayerView   **************************************/

MTG_PlayerView::MTG_PlayerView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mPlayer(0),
	mCurrCards(MTG_Card::E_OpenState)
{
	ui.setupUi(this);

	connect(ui.buttonGroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(open()));
	connect(ui.cards_view,SIGNAL(checked(IDCard_t, bool)),this,SLOT(checked(IDCard_t,bool)));
	connect(ui.pv_btn_play,SIGNAL(clicked()),this,SLOT(play()));
}

MTG_PlayerView::~MTG_PlayerView()
{

}

bool MTG_PlayerView::setPlayer(MTG_Player *aPlayer) {
	if (!aPlayer || !aPlayer->game()) return false;
	clear();
	mPlayer = aPlayer;
	//TODO: так делать плохо
	MTG_Observer::setGame(const_cast<MTG_Game*>(mPlayer->game()));
	ui.pv_lbl_name->setText(QString::fromStdString(mPlayer->name()));
	return true;
}

void MTG_PlayerView::clear() {
	mPlayer = 0;
	MTG_Observer::clear();
	ui.pv_lbl_name->clear();
}

MTG_Player *MTG_PlayerView::player() const {
	return mPlayer;
}	

void MTG_PlayerView::changeGame(MTG_Game *aGame) {
	update();
}

void MTG_PlayerView::gameEvent(MTG_Game::State_t aState) {
	update();
}

void MTG_PlayerView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	switch (aPhase)
	{
	case E_StartPhase: {
		open(MTG_Card::E_OpenState);
		break;
	}
	case E_InvocationPhase: update(); break;
	case E_AttackPhase: {
		open(MTG_Card::E_ProtectionState);
		break;
	}
	case E_FinishPhase: update(); break;
	}
}

void MTG_PlayerView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	if (aPlayer = mPlayer) update();
	else {
		switch (aPhase)
		{
		case E_AttackPhase: if (mPlayer->state() == MTG_Player::E_PlayState) open(MTG_Card::E_ProtectionState); break;
		}
	}
}

void MTG_PlayerView::winEvent(MTG_Player *aPlayerWin) {

}

void MTG_PlayerView::play() {
	if (!mPlayer) return;
	mPlayer->play(ui.cards_view->checkedCards());
	//update();
}

void MTG_PlayerView::open() {
	MTG_Card::State_t state;
	QAbstractButton *check_btn = ui.buttonGroup->checkedButton();
	if (check_btn == ui.pv_btn_open) state = MTG_Card::E_OpenState;
	else if (check_btn == ui.pv_btn_invocation) state = MTG_Card::E_InvocationState;
	else if (check_btn == ui.pv_btn_protected) state = MTG_Card::E_ProtectionState;
	else return;


	open(state);
}

void MTG_PlayerView::open(MTG_Card::State_t aState)
{
	MTG_CardSet cards;
	QAbstractButton *check_btn = 0;
	switch (aState)
	{
	case MTG_Card::E_OpenState: check_btn = ui.pv_btn_open;break;
	case MTG_Card::E_InvocationState: check_btn = ui.pv_btn_invocation; break;
	case MTG_Card::E_ProtectionState: check_btn = ui.pv_btn_protected; break;
	default: return;
	 }

	ui.buttonGroup->blockSignals(true);
	check_btn->setChecked(true);
	ui.buttonGroup->blockSignals(false);
	mCurrCards = aState;
	
	update();
}

void MTG_PlayerView::update()
{
	MTG_CardSet cards;
	if(mPlayer) cards = mPlayer->cards(mCurrCards);
	ui.cards_view->setCards(cards);

	bool is_play = false;

	if (mPlayer && game()) {
		Phase_t phase = game()->phase();

		switch (mCurrCards)
		{
		case MTG_Card::E_OpenState: is_play = (phase == ::E_InvocationPhase); break;
		case MTG_Card::E_ProtectionState: is_play = (phase == E_AttackPhase); break;
		}

		is_play = (mPlayer->state() == MTG_Player::E_PlayState && is_play);
	}

	ui.pv_btn_play->setEnabled(is_play);
	ui.cards_view->setCheckable(is_play);

	mMana = 0;
	QString health = 0;
	QString deck = 0;
	QString role;
	
	if (mPlayer) {
		mMana = mPlayer->mana();
		health = QString::number(mPlayer->health());
		mMana = mPlayer->mana();
		deck = QString::number(mPlayer->deck().size());
		switch (mPlayer->role())
		{
			case ::E_AttackRole: role = "Нападающий"; break;
			case ::E_ProtectionRole: role = "Защитник"; break;
		}
	}

	ui.pv_lbl_health->setText(health);
	ui.pv_lbl_mana->setText(QString::number(mMana));
	ui.pv_lbl_deck->setText(deck);
	ui.pv_lbl_role->setText(role);
}

void MTG_PlayerView::checked(IDCard_t aIDCard, bool aChecked) {
	if (!mGame) return;
	if (mGame->phase() != ::E_InvocationPhase) return;
	Card_t card = ::CardFromID(aIDCard);
	if (aChecked) {
		if (card.Cost <= mMana) mMana -= card.Cost;
		else ui.cards_view->setChecked(aIDCard,false);
	}
	else mMana += card.Cost;

	ui.pv_lbl_mana->setText(QString::number(mMana));
}