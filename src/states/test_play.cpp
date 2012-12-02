#include "stdafx.h"
#include "test_play.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "objects/game_object.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	TestPlay::TestPlay() :
		background_(),
		selected_animon_(NULL),
		test_group(100, 100, 200, 200),
		test_label(100, 100, 100, 100, "HI")
	{
		GameEventManager* manager = GameEventManager::GetInstance();
		std::set<KeyObject*>& keys = manager->GetKeyObjectsFor(this);

		keys.insert(new Animon(500, 300, refs::textures::objects::NEKO, sf::Color::White, L"ねこ"));

		sf::RenderWindow* window = Game::GetInstance()->GetWindow();

		int offset = 300;
		test_group.SetPosition(sf::Vector2f(offset/2, offset/2));
		test_group.SetSize(sf::Vector2f(window->getSize().x - offset, window->getSize().y - offset));
		test_group.SetZ(1);

		test_label.GetStyle()->SetNormalColor(sf::Color::Red);
		test_group.Add(&test_label);
	}

	TestPlay::~TestPlay() {}

	void TestPlay::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = PLAYING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(refs::textures::backgrounds::OFFICE));
		// The image might be a little too big, so scale it so it fits in the window
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		Game::GetInstance()->AddUIElements();

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		GameEventManager* game_event_manager = GameEventManager::GetInstance();
		std::set<KeyObject*> keys = game_event_manager->GetKeyObjectsFor(this);
		for (std::set<KeyObject*>::iterator i = keys.begin(); i != keys.end(); ++i) {
			event_manager->AddGameObject(*i);
		}

		Event load_event;
		load_event.type = Event::E_LOAD_STATE;
		event_manager->AddEvent(load_event);
		
		event_manager->RegisterListener(Event::E_GAME_EVENT, this, NULL);
		event_manager->RegisterListener(Event::E_MOUSE_PRESSED, this, NULL);
		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this, NULL);

		//event_manager->AddGameObject(&test_group);
		Game::GetInstance()->GetTileListLabel()->SetString(tools::RomajiToHiragana("anekoaan"));
	}

	void TestPlay::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(refs::textures::backgrounds::WINDOW);

		Game::GetInstance()->RemoveUIElements();

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		GameEventManager* game_event_manager = GameEventManager::GetInstance();
		std::set<KeyObject*>& keys = game_event_manager->GetKeyObjectsFor(this);
		for (std::set<KeyObject*>::iterator i = keys.begin(); i != keys.end(); ++i)
			event_manager->RemoveGameObject(*i);

		event_manager->UnregisterListener(Event::E_GAME_EVENT, this, NULL);
		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, NULL);
		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this, NULL);

		event_manager->RemoveGameObject(&test_group);
	}

	GameState TestPlay::Update(float elapsed_time) {
		return game_state_;
	}
	
	void TestPlay::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void TestPlay::PostDraw(sf::RenderTarget *target) {
	}

	void TestPlay::ProcessEvent(Event event) {
			switch (event.type) {
			case Event::E_MOUSE_PRESSED:
				if (event.focus == NULL) {
					if (selected_animon_ != NULL)
						selected_animon_->SetFocused(false);
					selected_animon_ = NULL;
				}
				break;
			case Event::E_GAME_EVENT:
				switch (event.gameEvent) {
				case GameEvent::KEY_CLICK:
					if (selected_animon_ != NULL)
						selected_animon_->SetFocused(false);
					selected_animon_ = (Animon*) event.focus;
					selected_animon_->SetFocused(true);
					break;
				}
				break;
			case Event::E_HIRAGANA_DRAWN:
				if (selected_animon_ != NULL && selected_animon_->IsWordWritten(event)) {
					selected_animon_->Grow();
					Game::GetInstance()->GetTileList()->Clear();
				}
				break;
			}
	}

	void TestPlay::RegisterAnimon(Animon* animon) {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->AddGameObject(animon);
		event_manager->UnregisterListener(Event::E_CLICKED, this, animon);
	}
	
	void TestPlay::UnregisterAnimon(Animon* animon) {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RemoveGameObject(animon);
		event_manager->UnregisterListener(Event::E_CLICKED, this, animon);
	}
};