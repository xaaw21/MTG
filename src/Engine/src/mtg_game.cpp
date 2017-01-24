#include "mtg_game.hpp"

#include "mtg_player.hpp"
#include "mtg_events.hpp"

static IDObserver GLOBAL_ID_OBSERVER(1);

MTG_Game::MTG_Game()
	:mState(E_StopState),
	mFirstPlayer(0),
	mSecondPlayer(0),
	mAttackPlayer(0),
	mPhase(E_NonePhase),
	mRunde(0),
	mLoopEvents(false)
{

}

MTG_Game::~MTG_Game() {

}

MTG_Game::State MTG_Game::state() const {
	return mState;
}

Runde MTG_Game::runde() const {
	return mRunde;
}

Phase MTG_Game::phase() const {
	return mPhase;
}

bool MTG_Game::setPlayers(MTG_Player *aFirstPlayer, MTG_Player *aSecondPlayer) {
	if (mState != E_StopState) return false;
	if (!aFirstPlayer || !aSecondPlayer) return false;
	
	mFirstPlayer = aFirstPlayer;
	mSecondPlayer = aSecondPlayer;
	mSecondPlayer->mGame = mFirstPlayer->mGame = this;

	return true;
}

MTG_Player* MTG_Game::playerFirst() const {
	return mFirstPlayer;
}

MTG_Player* MTG_Game::playerSecond() const {
	return mSecondPlayer;
}

MTG_Player* MTG_Game::playerAttack() const {
	return mAttackPlayer;
}

MTG_Player* MTG_Game::playerNext(const MTG_Player *aPlayer) const {
	if (aPlayer == mFirstPlayer) return mSecondPlayer;
	else if(aPlayer == mSecondPlayer) return mFirstPlayer;
	return 0;
}

IDObserver MTG_Game::addObserver(const MTG_Observer &aObserver) {
	auto result = mObservers.insert(std::make_pair(GLOBAL_ID_OBSERVER, aObserver));
	if (result.first == mObservers.end()) return INVALID_ID_OBSERVER;
	GLOBAL_ID_OBSERVER++;
	return result.first->first;
}
																
bool MTG_Game::delObserver(IDObserver aIDObserver) {
	return mObservers.erase(aIDObserver);
}

void MTG_Game::clear() {
	stop();
	mFirstPlayer = mSecondPlayer = 0;
}

bool MTG_Game::start() {
	if (!mFirstPlayer || !mSecondPlayer) return false;
	mState = E_StartState;
	mPhase = E_NonePhase;
	mRunde = 0;
	mAttackPlayer = 0;

	//mFirstPlayer->reset();
	//mSecondPlayer->reset();

	MTG_GameEvent game_event(mState);
	this->event(&game_event);
	return true;
}
									
bool MTG_Game::stop() {
	mState = E_StopState;
	MTG_GameEvent game_event(mState);
	this->event(&game_event);
	return true;
}

bool MTG_Game::next() {
	if (mState != E_StartState) return;

	if (mInfo.phase == E_FinishPhase) mInfo.phase = E_StartPhase;
	else mInfo.phase = (Phase)(mInfo.phase + 1);

	switch (mInfo.phase)
	{
	case E_StartPhase: {

		static std::function<void(MTG_Player*)>	open_card = [this](MTG_Player *aPlayer) -> void {
			aPlayer->mCardsOpen.push(aPlayer->mDeck.pop());
		};

		std::function<void(MTG_Player*)> init_palyer = [this](MTG_Player *aPlayer) -> void {
			aPlayer->mCardsProtection.push(aPlayer->mCardsAttack);
			aPlayer->mCardsProtection.push(aPlayer->mCardsInvocation);
			aPlayer->mCardsAttack.clear();
			aPlayer->mCardsInvocation.clear();
			aPlayer->mMana = mInfo.runde;
			aPlayer->mState = E_NoWalk;
		};

		std::function<MTG_Player*(MTG_Player*)> next_player = [this](MTG_Player *aPlayer) -> MTG_Player* {
			if (aPlayer == mFirstPlayer) return mSecondPlayer;
			return mFirstPlayer;
		};


		if (0 == mInfo.runde) {
			mAttackPlayer = (std::rand() % 2) ? mFirstPlayer : mSecondPlayer;
			open_card(next_player(mAttackPlayer));
		}
		else {
			mAttackPlayer = next_player(mAttackPlayer);
			open_card(mFirstPlayer);
			open_card(mSecondPlayer);
		}

		mInfo.runde++;

		init_palyer(mFirstPlayer);
		init_palyer(mSecondPlayer);
		

		MTG_RundeEvent runde_event(mInfo.runde);
		this->event(&runde_event);

		break;
	}
	case E_CallPhase: {
		mFirstPlayer->mState = E_Walk;
		mSecondPlayer->mState = E_Walk;
		break;
	}
	case E_AttackPhase: {
		mFirstPlayer->mState = E_WaitWalk;
		mSecondPlayer->mState = E_WaitWalk;
		mAttackPlayer->mState = E_Walk;
		break;
	}
	case E_FinishPhase: {
		mFirstPlayer->mState = E_NoWalk;
		mSecondPlayer->mState = E_NoWalk;

		
		MTG_CardSet first_set, second_set;
		while (true) {
			MTG_Card first_card = CardFromID(mFirstPlayer->mCardsAttack.front());
			MTG_Card second_card = CardFromID(mFirstPlayer->mCardsAttack.front());

			if (!first_card.isValid() && !second_card.isValid()) break;
			else if (!first_card.isValid()) {
				mFirstPlayer->mHealth -= second_card.Attack;
				second_set.push_back(second_card.ID);
			}
			else if (!second_card.isValid()) {
				mSecondPlayer->mHealth -= first_card.Attack;
				first_set.push_back(first_card.ID);
			}
			else {
				second_card.Protection -= first_card.Attack;
				first_card.Protection -= second_card.Attack;
				if (second_card.Protection <= 0) second_set.push_back(second_card.ID);
				if (first_card.Protection <= 0) first_set.push_back(first_card.ID);
			}
		}

		mFirstPlayer->mCardsAttack = first_set;
		mSecondPlayer->mCardsAttack = second_set;

		if (mFirstPlayer->mHealth == mSecondPlayer->mHealth == 0) {}
		else if(mFirstPlayer->mHealth == 0){}
		else if (mSecondPlayer->mHealth == 0) {}

		break;
	}
	}

	MTG_PhaseEvent phase_event(mInfo.phase);
	this->event(&phase_event);

}

bool MTG_Game::play(MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	if (aPlayer->mState != E_Walk) return false;
	aPlayer->mState = E_AlredyWalk;

	std::function<MTG_Player*(MTG_Player*)> next_player = [this](MTG_Player *aPlayer) -> MTG_Player* {
		if (aPlayer == mFirstPlayer) return mSecondPlayer;
		return mFirstPlayer;
	};

	switch (mInfo.phase)
	{
	case E_AttackPhase: {
		next_player(mAttackPlayer)->mState = E_Walk;
		break;
	}
	default: return false;
	}

	MTG_PlayerEvent player_event(mInfo.phase, aCards);
	this->event(&player_event);

	return true;
}

void MTG_Game::event(const MTG_Event *aEvent) {

	mEvents.push_back(aEvent);
	if (mLoopEvents) return;
	mLoopEvents = true;

	while(true){
		if (mEvents.empty()) break;
		const MTG_Event *event = mEvents.front();
		mEvents.pop_front();

		//work
		switch (event->type())
		{
		case MTG_Event::E_PlayerEvent: {
			const MTG_PlayerEvent *player_event = (const MTG_PlayerEvent*)event;
			this->playerNext(player_event->player())->event(event);
			break;
		}
		default: {
			mFirstPlayer->event(aEvent);
			mSecondPlayer->event(aEvent);
		}
		}

		for (auto it = mObservers.begin(), end = mObservers.end(); it != end; it++) {
			MTG_Observer observer = it->second;
			observer(aEvent);
		}

		delete event;
	}

	mLoopEvents = false;
}