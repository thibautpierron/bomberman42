/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Flame.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 12:24:42 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 10:56:22 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Flame.hpp"

Flame::Flame(const glm::vec2 & pos, bool b) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::FLAME),
_center(b){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(1000);
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Flame::player_move_callback));
    SEventManager::getInstance().registerEvent(Event::ENEMY_MOVE, MEMBER_CALLBACK(Flame::enemy_move_callback));
}

Flame::~Flame(){
    SEventManager::getInstance().unRegisterEvent(Event::PLAYER_MOVE, this);
    SEventManager::getInstance().unRegisterEvent(Event::ENEMY_MOVE, this);
}

void    Flame::update(void){
    std::chrono::milliseconds   time_since_creation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creation_time);
    if (time_since_creation > ms_before_explode) {
        setState(State::DYING);
        SEventManager::getInstance().raise(Event::FLAME_DISAPEAR, this);
    }
}

std::chrono::milliseconds const                                &Flame::get_ms_before_explode(void) const {
    return ms_before_explode;
}
std::chrono::time_point<std::chrono::steady_clock> const       &Flame::get_creation_time(void) const {
    return creation_time;
}

void                Flame::player_move_callback(void *player){
    if (glm::round(static_cast<IGameEntity *>(player)->getPosition()) == getPosition())
        SEventManager::getInstance().raise(Event::PLAYER_DIES, player);
}

void                Flame::enemy_move_callback(void *enemy){
    if (glm::round(static_cast<IGameEntity *>(enemy)->getPosition()) == getPosition())
        SEventManager::getInstance().raise(Event::ENEMY_DIES, enemy);
}


bool                Flame::is_centered(void) const{
    return _center;
}

Flame::Flame(void) : 
AGameEntity(glm::vec2(0., 0.), glm::vec2(0., -1.), State::STANDING, 0., Type::FLAME),
_center(false){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(1000);
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Flame::player_move_callback));
    SEventManager::getInstance().registerEvent(Event::ENEMY_MOVE, MEMBER_CALLBACK(Flame::enemy_move_callback));
}

Flame::Flame(Flame const &f) : 
AGameEntity(f._position, glm::vec2(0., -1.), State::STANDING, 0., Type::FLAME),
_center(f._center){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(1000);
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Flame::player_move_callback));
    SEventManager::getInstance().registerEvent(Event::ENEMY_MOVE, MEMBER_CALLBACK(Flame::enemy_move_callback));
}

Flame   &Flame::operator=(Flame const &rhs){
    _center = rhs._center;
    return *this;
}