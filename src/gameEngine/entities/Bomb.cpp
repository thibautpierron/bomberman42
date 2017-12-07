/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bomb.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 10:27:35 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 22:30:33 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bomb.hpp"
#include <iostream>

Bomb::Bomb(const glm::vec2 & pos, Player *p) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::BOMB), flameNb(2), player(p){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(2000);
    SEventManager::getInstance().registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(Bomb::explode_if_touched));
}

Bomb::~Bomb(){
}

int     Bomb::getFlameNb(void) const {
    return flameNb;
}

void    Bomb::update(void){
    std::chrono::milliseconds   time_since_creation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creation_time);
    if (time_since_creation > ms_before_explode) {
        explode();
    }
}

void    Bomb::explode_if_touched(void *p){
    if (glm::round(static_cast<IGameEntity *>(p)->getPosition()) == getPosition() && getState() != State::DYING)
        explode();
}

void    Bomb::explode(void){
    setState(State::DYING);
    SEventManager::getInstance().raise(Event::BOMB_EXPLODES, this);
}

std::chrono::milliseconds const                                &Bomb::get_ms_before_explode(void) const {
    return ms_before_explode;
}
std::chrono::time_point<std::chrono::steady_clock> const       &Bomb::get_creation_time(void) const {
    return creation_time;
}

const Player                                                   *Bomb::getPlayer(void) const {
    return player;
}


// gameEngine/entities/Bomb.cpp\


// if (find(actions.begin(), actions.end(), Action::SPAWN_BOMB) != actions.end()){
//     _entityList.push_back(new Bomb(p->getPosition()));
// }