/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bomb.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 10:27:35 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/01 12:27:52 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bomb.hpp"
#include <iostream>

Bomb::Bomb(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::BOMB), flameNb(2){
    creation_time = std::chrono::steady_clock::now();
    ms_before_explode = std::chrono::milliseconds(2000);
}

Bomb::~Bomb(){
}

int     Bomb::getFlameNb(void) const {
    return flameNb;
}

void    Bomb::update(void){
    std::chrono::milliseconds   time_since_creation = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creation_time);
    if (time_since_creation > ms_before_explode) {
        setState(State::DYING);
    }
}

std::chrono::milliseconds const                                &Bomb::get_ms_before_explode(void) const {
    return ms_before_explode;
}
std::chrono::time_point<std::chrono::steady_clock> const       &Bomb::get_creation_time(void) const {
    return creation_time;
}


// gameEngine/entities/Bomb.cpp\


// if (find(actions.begin(), actions.end(), Action::SPAWN_BOMB) != actions.end()){
//     _entityList.push_back(new Bomb(p->getPosition()));
// }