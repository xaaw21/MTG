#include "mtg_game_view.hpp"

#include <QApplication>
#include "ui_splash_view.h"

#define TIMEOUT_NEXT_PHASE 2000
#define TIMEOUT_NEXT_PHASE_FINISH 5000

/**************************    MTG_GameView    **********************************************/

class SplashView : public QWidget
{
	Q_OBJECT
public:
	SplashView(QWidget *aParent = 0, QGraphicsEffect *aEffect = 0);
	~SplashView();

public Q_SLOTS:
   void show(const QString &aMessage = QString());
   void hide();

private:
	Ui::SplashView ui;
	QGraphicsEffect *mEffect;
};

SplashView::SplashView(QWidget *aParent, QGraphicsEffect *aEffect)
	:QWidget(aParent),
	mEffect(aEffect)
{
	ui.setupUi(this);

	connect(ui.sv_btn_ok,SIGNAL(clicked()),this,SLOT(hide()));
}

SplashView::~SplashView()
{

}
void SplashView::show(const QString &aMessage) {
	if (mEffect) mEffect->setEnabled(true);
	ui.sp_lbl_msg->setText(aMessage);
	QWidget::show();
}

void SplashView::hide() {
	if (mEffect) mEffect->setEnabled(false);
	QWidget::hide();
}


/**************************    MTG_GameView    **********************************************/



MTG_GameView::MTG_GameView(QWidget *aParent)
	:QWidget(aParent),
	MTG_Observer(),
	mSplash(0)
{
	ui.setupUi(this);

	mEffect.setBlurHints(QGraphicsBlurEffect::PerformanceHint);
	mEffect.setBlurRadius(1.8);
	ui.substrate->setGraphicsEffect(&mEffect);

	ui.gv_btn_group_phase->setId(ui.gv_btn_start_phase,(int)E_StartPhase);
	ui.gv_btn_group_phase->setId(ui.gv_btn_invocation_phase, (int)E_InvocationPhase);
	ui.gv_btn_group_phase->setId(ui.gv_btn_attack_phase, (int)E_AttackPhase);
	ui.gv_btn_group_phase->setId(ui.gv_btn_finish_phase, (int)E_FinishPhase);

	connect(ui.StartButton,SIGNAL(clicked()),this,SLOT(start()));
	connect(ui.btnLogout, SIGNAL(clicked()), this, SLOT(exit()));
	
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(next()));

	mSplash = new SplashView(this,&mEffect);
	mSplash->hide();
}

MTG_GameView::~MTG_GameView()
{
	
}

void MTG_GameView::changeGame(MTG_Game *aGame) {
	ui.gv_lbl_status->clear();

	if(aGame) {
		auto players = aGame->players();
		ui.PlayerViewFirst->setPlayer(players.first);
		ui.PlayerViewSecond->setPlayer(players.second);
		ui.BoardView->setGame(aGame);
		ui.LogView->setGame(aGame);

		ui.gv_lbl_status->setText("Начните новую игру");
	}
	else {
		ui.PlayerViewFirst->clear();
		ui.PlayerViewSecond->clear();
		ui.BoardView->clear();
		ui.LogView->clear();
	}

	update(::E_NonePhase);
}

void MTG_GameView::gameEvent(MTG_Game::State_t aState) {
	switch (aState)
	{
	case MTG_Game::E_StopState: break;
	case MTG_Game::E_StartState: {
		ui.gv_lbl_status->setText("Запуск игры");
		break;
	}
	}
}

void MTG_GameView::phaseEvent(Phase_t aPhase, Round_t aRound, const MTG_CardMap &aCards) {
	UNUSED(aCards);

	update(aPhase);

	switch (aPhase)
	{
	case E_StartPhase: {
		ui.gv_lbl_round->setText(QString("Раунд №%1").arg(aRound));
		ui.gv_lbl_status->setText("Переход к следующей фазе.\nЖдите");
		mTimer.start(TIMEOUT_NEXT_PHASE);
		break;
	}
	case E_InvocationPhase: 
	case E_AttackPhase:  ui.gv_lbl_status->setText("Игроки ходят"); break;
	case E_FinishPhase: {
		ui.gv_lbl_status->setText("Переход к следующей фазе.\nЖдите");
		mTimer.start(TIMEOUT_NEXT_PHASE_FINISH);
		break;
	}
	default: break;
	}
}

void MTG_GameView::playerEvent(Phase_t aPhase, MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	UNUSED(aPlayer);
	UNUSED(aCards);

	switch (aPhase)
	{
	case E_InvocationPhase:
	case E_AttackPhase: {
		auto players = mGame->players();
		if (players.first->state() == MTG_Player::E_PlayedState && players.second->state() == MTG_Player::E_PlayedState) {
			ui.gv_lbl_status->setText("Переход к следующей фазе.\nЖдите");
			mTimer.start(TIMEOUT_NEXT_PHASE);
		}
		break;
	}
	default: break;
	}
}

void MTG_GameView::winEvent(MTG_Player *aPlayerWin) {
	QString str_win_player = "Ничья";
	if (aPlayerWin) str_win_player = QString::fromStdString(aPlayerWin->name());
	ui.gv_lbl_status->setText(QString("Игра закончена.\nПобедитель: %1").arg(str_win_player));
	mSplash->show(QString("Игра закончена.\nПобедитель: %1").arg(str_win_player));
}

void MTG_GameView::resizeEvent(QResizeEvent *aEvent) {
	UNUSED(aEvent);
	mSplash->resize(this->size());
}

void MTG_GameView::next() {
	mGame->next();
}

void MTG_GameView::update(Phase_t Phase) {
	switch (Phase)
	{
	case E_NonePhase: {
		ui.gv_btn_group_phase->setExclusive(false);
		QAbstractButton *btn = ui.gv_btn_group_phase->checkedButton();
		if(btn) btn->setChecked(false);
		ui.gv_lbl_round->setText("Информация");
		break;
	} 
	default: {
		ui.gv_btn_group_phase->setExclusive(true);
		QAbstractButton *btn = ui.gv_btn_group_phase->button((int)Phase);
		if(btn) btn->setChecked(true);
		break;
	}
	}
}

void MTG_GameView::start() {
	if (!mGame) return;
	mGame->start();
	mTimer.start(1000);
}

void MTG_GameView::exit() {
	qApp->exit();
}

#include "mtg_game_view.moc"