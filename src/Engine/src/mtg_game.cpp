#include "mtg_game.hpp"

#include "mtg_player.hpp"
#include "mtg_events.hpp"
#include <algorithm>

static IDObserver_t GLOBAL_ID_OBSERVER(1);

MTG_Game::MTG_Game()
	:mState(E_StopState),
	mRound(0),
	mPhase(E_NonePhase),
	mLoopEvents(false),
	mEvents(),
	mPlayers(0,0),
	mObservers()	
{
	  
}

MTG_Game::~MTG_Game() {
	clear();
}

MTG_Game::State_t MTG_Game::state() const {
	return mState;
}

Round_t MTG_Game::round() const {
	return mRound;
}

Phase_t MTG_Game::phase() const {
	return mPhase;
}

bool MTG_Game::setPlayers(MTG_Player *aFirstPlayer, MTG_Player *aSecondPlayer) {
	if (!aFirstPlayer || !aSecondPlayer || aFirstPlayer == aSecondPlayer) return false;
	
	clear();
	mPlayers = std::pair<MTG_Player*, MTG_Player*>(aFirstPlayer, aSecondPlayer);
	aFirstPlayer->mGame = this;
	aSecondPlayer->mGame = this;
	return true;
}

std::pair<MTG_Player*, MTG_Player*> MTG_Game::players() const {
	return mPlayers;
}

MTG_Player* MTG_Game::player(Role_t aRole) const {
	if (!mPlayers.first || !mPlayers.second) return 0;
	if (mPlayers.first->role() == aRole) return mPlayers.first;
	else if(mPlayers.second->role() == aRole) return mPlayers.second;
	return 0;
}

MTG_Player* MTG_Game::playerNext(const MTG_Player *aPlayer) const {
	if (aPlayer == mPlayers.first) return mPlayers.second;
	else if(aPlayer == mPlayers.second) return mPlayers.first;
	return 0;
}

IDObserver_t MTG_Game::addObserver(const Observer_t &aObserver) {
	auto result = mObservers.insert(std::make_pair(GLOBAL_ID_OBSERVER, aObserver));
	if (result.first == mObservers.end()) return INVALID_ID_OBSERVER;
	GLOBAL_ID_OBSERVER++;
	return result.first->first;
}
																
bool MTG_Game::delObserver(IDObserver_t aIDObserver) {
	return mObservers.erase(aIDObserver);
}

void MTG_Game::clear() {
	stop();
	if (mPlayers.first) mPlayers.first->mGame = 0;
	if (mPlayers.second) mPlayers.second->mGame = 0;

	mPlayers = std::pair<MTG_Player*, MTG_Player*>(0, 0);
}

bool MTG_Game::start() {
	if (!mPlayers.first || !mPlayers.second) return false;
	stop();
	mState = E_StartState;
	mPhase = E_NonePhase;
	mRound = 0;

	std::function<void(MTG_Player*)> reset_player = [this](MTG_Player *aPlayer) -> void {
		aPlayer->mDeck.reset();
		aPlayer->mDeck.disturb();
		aPlayer->mCards.clear();
		aPlayer->mMana = 0;
		aPlayer->mHealth = 15;
		aPlayer->mRole = ::E_NoneRole;
		aPlayer->mState = MTG_Player::E_NoneState;
	};

	reset_player(mPlayers.first);
	reset_player(mPlayers.second);

	MTG_GameEvent *game_event = new MTG_GameEvent(mState);
	game_event->mGame = this;
	this->event(game_event);
	return true;
}
									
bool MTG_Game::stop() {
	std::for_each(mEvents.begin(), mEvents.end(), [](const MTG_Event* aEvent) { delete aEvent;});
	mEvents.clear();
	if (mState != E_StopState) {
		mState = E_StopState;
		MTG_GameEvent *game_event = new MTG_GameEvent(mState);
		game_event->mGame = this;
		this->event(game_event);
	}
	return true;
}

Phase_t MTG_Game::next() {
	if (mState != E_StartState) return ::E_NonePhase;

	mPhase = NextPhase(mPhase);

	switch (mPhase)
	{
	case E_StartPhase: {
		nextStart();
		break;
	}
	case E_InvocationPhase: {
		nextInvocation();
		break;
	}
	case E_AttackPhase: {
		nextAttack();
		break;
	}
	case E_FinishPhase: {
		nextFinish();
		break;
	}
	default: return E_NonePhase;
	}

	return mPhase;
}

void MTG_Game::nextStart() {
	std::function<MTG_CardSet(MTG_Player*, int)> open_cards = [this](MTG_Player *aPlayer, int aCount) -> MTG_CardSet {
		MTG_CardSet cards = aPlayer->mDeck.cards(aCount);
		aPlayer->mCards.insert(aPlayer->mCards.end(), cards.begin(), cards.end());
		return cards;
	};

	std::function<void(MTG_Player*)> init_palyer = [this](MTG_Player *aPlayer) -> void {
		//Существа в "Атаке" и "Призывается" переходят в боеготовность , а так же восстанавливаем здоровье всех существ
		for (auto it = aPlayer->mCards.begin(), end = aPlayer->mCards.end(); it != end; it++) {
			if (it->State == MTG_Card::E_AttackState || it->State == MTG_Card::E_InvocationState) 
				it->State = MTG_Card::E_ProtectionState;
			it->Health = it->protection();
		}

		//Даем маны
		aPlayer->mMana = mRound;
		aPlayer->mState = MTG_Player::E_NoneState;
	};

	MTG_Player *attack_player = player(E_AttackRole);
	MTG_Player *protected_player = player(E_ProtectionRole);
	MTG_CardMap cards_map;

	//Инициализация фазы начала боя
	uint8_t open_attack = 1, open_protection = 1;
	//Переходим на следующий раунд
	mRound++;

	//Если это самый первый раунд, то выбираем атакующего игрока
	if (1 == mRound) {
		std::srand((int)this);
		attack_player = (std::rand() % 2) ? mPlayers.first : mPlayers.second;
		open_attack = 3;
		open_protection = 4;
	}
	//Иначе атакующее право переходит другому игроку
	else attack_player = playerNext(player(E_AttackRole));

	//Определяем защитника
	protected_player = playerNext(attack_player);

	//Инициализируем игроков	
	attack_player->mRole = E_AttackRole;
	protected_player->mRole = E_ProtectionRole;
	init_palyer(attack_player);
	init_palyer(protected_player);


	cards_map[attack_player] = open_cards(attack_player, open_attack);
	cards_map[protected_player] = open_cards(protected_player, open_protection);

	MTG_PhaseEvent *phase_event = new MTG_PhaseEvent(mPhase, mRound, cards_map);
	phase_event->mGame = this;
	this->event(phase_event);
}

void MTG_Game::nextInvocation() {
	player(E_AttackRole)->mState = MTG_Player::E_PlayState;
	player(E_ProtectionRole)->mState = MTG_Player::E_PlayState;

	MTG_PhaseEvent *phase_event = new MTG_PhaseEvent(mPhase, mRound);
	phase_event->mGame = this;
	this->event(phase_event);
}

void MTG_Game::nextAttack() {
	player(E_AttackRole)->mState = MTG_Player::E_PlayState;
	player(E_ProtectionRole)->mState = MTG_Player::E_NoneState;

	MTG_PhaseEvent *phase_event = new MTG_PhaseEvent(mPhase, mRound);
	phase_event->mGame = this;
	this->event(phase_event);
}

void MTG_Game::nextFinish() {
	MTG_Player *attack_player = player(E_AttackRole);
	MTG_Player *protected_player = player(E_ProtectionRole);
	MTG_CardMap cards_map;

	attack_player->mState = MTG_Player::E_NoneState;
	protected_player->mState = MTG_Player::E_NoneState;

	MTG_CardSet attack_cards = attack_player->mCards.cards(MTG_Card::E_AttackState);
	MTG_CardSet protection_cards = protected_player->mCards.cards(MTG_Card::E_AttackState);

	for (auto it_attack = attack_cards.begin(), end_attack = attack_cards.end(), it_protected = protection_cards.begin(), end_protected = protection_cards.end(); it_attack != end_attack; it_attack++) {
		if (it_protected != end_protected) {
			it_protected->Health -= it_attack->attack();
			it_attack->Health -= it_protected->attack();
			if (it_protected->Health <= 0) it_protected->State = MTG_Card::E_DeadState;
			if (it_attack->Health <= 0) it_attack->State = MTG_Card::E_DeadState;
			it_protected++;
		}
		else protected_player->mHealth -= it_attack->attack();
	};

	static std::function<void(MTG_Player*, const MTG_CardSet&)> cards_merge = [this](MTG_Player *aPlayer, const MTG_CardSet &aCardsMerge) -> void {
		for (auto it_merge = aCardsMerge.begin(), end_merge = aCardsMerge.end(); it_merge != end_merge; it_merge++) {
			for (auto it_cards = aPlayer->mCards.begin(), end_cards = aPlayer->mCards.end(); it_cards != end_cards; it_cards++) {
				if (it_merge->ID == it_cards->ID) {

					it_cards->Health = it_merge->Health;
					it_cards->State = it_merge->State;

					break;
				}
			}
		}
	};

	cards_merge(attack_player, attack_cards.cards(MTG_Card::E_AttackState));
	cards_merge(protected_player, protection_cards.cards(MTG_Card::E_AttackState));

	cards_map[attack_player] = attack_cards;
	cards_map[protected_player] = protection_cards;

	MTG_PhaseEvent *phase_event = new MTG_PhaseEvent(mPhase, mRound, cards_map);
	phase_event->mGame = this;
	this->event(phase_event);

	//если защитник умер - победил атакующий
	if (protected_player->mHealth <= 0) {
		MTG_WinEvent *win_event = new MTG_WinEvent(attack_player);
		win_event->mGame = this;
		this->event(win_event);
		stop();
	}

	//если у одного из игроков закончились карты - ничья
	if (
		(protected_player->mCards.size() <= 0 && protected_player->mDeck.size() <= 0) ||
		(attack_player->mCards.size() <= 0 && attack_player->mDeck.size() <= 0)
		)
	{
		MTG_WinEvent *win_event = new MTG_WinEvent(0);
		win_event->mGame = this;
		this->event(win_event);
		stop();
	}

}


bool MTG_Game::play(MTG_Player *aPlayer, const MTG_CardSet &aCards) {
	if (aPlayer->mState != MTG_Player::E_PlayState) return false;
	aPlayer->mState = MTG_Player::E_PlayedState;

	//проверяем на присутствие карт у игрока которыми он походил
	//если карта присутствует изменяем её состояние	- иначе отбрасываем её
	MTG_CardSet play_cards;
	for (auto it = aCards.begin(), end = aCards.end(); it != end; ++it) {
		switch (mPhase)
		{
		case E_InvocationPhase: {
			if (it->cost() > aPlayer->mMana) continue;
			for (auto it_cards = aPlayer->mCards.begin(), end_cards = aPlayer->mCards.end(); it_cards != end_cards; it_cards++) {
				if (it_cards->ID == it->ID) {
					if (it_cards->State == MTG_Card::E_OpenState) {
						aPlayer->mMana -= it_cards->cost();
						it_cards->State = MTG_Card::E_InvocationState;
						play_cards.push_back(*it_cards);
					}
				}
			}

			break;
		}
		case E_AttackPhase: {
			for (auto it_cards = aPlayer->mCards.begin(), end_cards = aPlayer->mCards.end(); it_cards != end_cards; it_cards++) {
				if (it_cards->ID == it->ID) {
					if (it_cards->State == MTG_Card::E_ProtectionState) {
						it_cards->State = MTG_Card::E_AttackState;
						play_cards.push_back(*it_cards);
					}
					break;
				}
			}

			break;
		}
		default: return false;
		}
	}

	if (aPlayer->mRole == ::E_AttackRole && mPhase == E_AttackPhase) {
		this->playerNext(aPlayer)->mState = MTG_Player::E_PlayState;
	}

	MTG_PlayerEvent *player_event = new MTG_PlayerEvent(mPhase, aPlayer, aCards);
	player_event->mGame = this;
	this->event(player_event);

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

		mPlayers.first->event(event);
		mPlayers.second->event(event);

		for (auto it = mObservers.begin(), end = mObservers.end(); it != end; it++) {
			Observer_t observer = it->second;
			observer(event);
		}

		delete event;
	}

	mLoopEvents = false;
}