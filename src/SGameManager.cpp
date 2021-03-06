/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SGameManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 14:36:37 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/30 15:38:30 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SGameManager.hpp"
#include "SEventManager.hpp"

void        SGameManager::loadSlot(void *s){
    if (_current_slot != nullptr){
        _current_slot->save();
    }
    for (auto &&i : _slots){
        if (i->get_save() == *static_cast<Save::Enum*>(s))
            _current_slot = i;
    }
    _current_slot->use();
}

SGameManager::SGameManager() : 
    _window(1600, 900),
    _game(nullptr),
    _camera(glm::vec3(4.f, -4.5f, 8.f), glm::vec3(5.f, 5.f, 0.f)), 
    _gui(_window, _camera), 
    _renderer(_window.getWin(), _camera),
    _current_slot(nullptr),
    _dev_mode(false),                                                   // <---- DEV MODE !
    _game_is_active(false), _quit_game(false){
    _slots[0] = new Slot(Save::SLOT1);
    _slots[1] = new Slot(Save::SLOT2);
    _slots[2] = new Slot(Save::SLOT3);

    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::QUIT_GAME, MEMBER_CALLBACK(SGameManager::quit_game));
    em.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SGameManager::new_game));
    em.registerEvent(Event::RESTART_GAME, MEMBER_CALLBACK(SGameManager::restart_game));
    em.registerEvent(Event::GAME_FINISH, MEMBER_CALLBACK(SGameManager::game_finish));
    em.registerEvent(Event::GAME_WIN, MEMBER_CALLBACK(SGameManager::game_win));
    em.registerEvent(Event::END_END_ANIMATION, MEMBER_CALLBACK(SGameManager::end_end_animation));

    em.registerEvent(Event::LOAD_SLOT, MEMBER_CALLBACK(SGameManager::loadSlot));
}

SGameManager::~SGameManager() {
    if (_current_slot){
        _current_slot->save();
    }
}

void        SGameManager::manage(void) {
    SEventManager &em = SEventManager::getInstance();
    if (!_dev_mode)
    {
        Menu::Enum me = Menu::Enum(Menu::SELECT_SLOT);
        em.raise(Event::GUI_TOGGLE, &me);
    }
    else
    {
        // NEW_GAME event should raise GameParams
        GameMode::Enum gme = GameMode::Enum(GameMode::CAMPAIGN);
        em.raise(Event::NEW_GAME, &gme);
    }

    while (!_quit_game)
    {
		_window.eventManager(_gui.getContext());

        if (_game_is_active || _post_game_display){
            _game->compute();
            glm::vec2 * player_pos = _game->getPlayerPos();
            _camera.update(_window.getMouseX(), _window.getMouseY(), player_pos);
            _renderer.render(_game->getMap(), _game->getEntityList());
            delete player_pos;
        }
        else
        {
            // Animation::Enum anim = Animation::Enum::WIN;
            //SEventManager::getInstance().raise(Event::END_ANIMATION, &anim);
        }
		_gui.render(_game_is_active || _post_game_display);
		_window.initGL();
		SDL_GL_SwapWindow(_window.getWin());
    }
}


SGameManager &	SGameManager::getInstance(){
    static SGameManager gm;
    return gm;
}

void            SGameManager::quit_game(void *){
    _quit_game = true;
}

void            SGameManager::new_game(void *p){
    GameParams *gp = static_cast<GameParams *>(p);
    if (_game != nullptr){
        delete _game;
        Animation::Enum anim = Animation::Enum::WIN;
        SEventManager::getInstance().raise(Event::END_ANIMATION, &anim);
    }
    _game = new GameEngine(*gp);
    _game_is_active = true;
    _current_game_params = gp;

    Animation::Enum a = Animation::START;
    SEventManager::getInstance().raise(Event::START_ANIMATION, &a);
}

void            SGameManager::restart_game(void *){
    if (_game != nullptr)
        delete _game;

    _game_is_active = true;
    Animation::Enum anim = Animation::Enum::WIN;
    SEventManager::getInstance().raise(Event::END_ANIMATION, &anim);
    _game = new GameEngine(*_current_game_params);

    Animation::Enum a = Animation::START;
    SEventManager::getInstance().raise(Event::START_ANIMATION, &a);
}

void            SGameManager::game_finish(void *){
    _game_is_active = false;
    _post_game_display = true;


    Animation::Enum anim = Animation::Enum::WIN;
    SEventManager::getInstance().raise(Event::START_ANIMATION, &anim);
    SEventManager::getInstance().raise(Event::GAME_PAUSE, nullptr);
}


void            SGameManager::game_win(void *){
    if (_game->getGameParams().get_game_mode() == GameMode::CAMPAIGN)
    {
        int stars = _game->getStarsCampaign();
        if (stars != -1 && stars > _current_slot->get_stars_campaign(_game->getGameParams().get_level()))
        {
            _current_slot->set_stars_campaign(_game->getGameParams().get_level(), stars);
            std::vector<int> ugly_tmp = _current_slot->get_all_stars_campaign();
            SEventManager::getInstance().raise(Event::UPDATE_ALL_CAMPAIGN_STARS, &ugly_tmp);
        }
    }
}

// void            SGameManager::newGame(GameMode::Enum gm){
//     (void)gm;
// }
// void            SGameManager::loadGame(Save::Enum s) {
//     (void)s;
// }
// void            SGameManager::saveGame(Save::Enum s) {
//     (void)s;
// }
Slot &          SGameManager::getCurrentSlot() const {
    return *_current_slot;
}

Slot &          SGameManager::getSlot(int index) const {
    if (index >= 0 && index <= 2)
        return *(_slots[index]);
    else
        throw std::out_of_range("SGameManager::getSlot(): wrong index");
}

bool            SGameManager::is_game_active() const {
    return _game_is_active;
}

void            SGameManager::end_end_animation(void *){
    _post_game_display = false;
    _game_is_active = false;
    Animation::Enum anim = Animation::Enum::WIN;
    SEventManager::getInstance().raise(Event::END_ANIMATION, &anim);
}