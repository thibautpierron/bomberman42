/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:54:09 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/11 11:56:58 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"

Player::Player(const glm::vec2 & pos, int nb) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::PLAYER),
_player_number(nb),
_max_bomb(1),
_flame_nb(1),
_speed(1.f),
_bomb_count(0),
left(false), right(false), up(false), down(false),
_graphicalDirection(glm::vec2(0,0)){
	SEventManager::getInstance().registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(Player::bomb_explodes_callback));
	SEventManager::getInstance().registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(Player::spawn_flame_callback));
}

glm::vec2	Player::getGraphicalDirection() const
{
	return _graphicalDirection;
}

void		Player::setGraphicalDirection(glm::vec2 dir)
{
	_graphicalDirection = dir;
}

int         Player::getPlayerNb(void){
	return _player_number;
}


glm::vec2	Player::getNewDirection(void){
	glm::vec2	ret(0,0);
	if (left)
		ret.x -= 1.;
	if (right)
		ret.x += 1.;
	if (up)
		ret.y += 1.;
	if (down)
		ret.y -= 1.;
	return glm::normalize(ret);
}

void		Player::newDirLeft(void){
	left = true;
}
void		Player::newDirRight(void){
	right = true;
}
void		Player::newDirUp(void){
	up = true;
}
void		Player::newDirDown(void){
	down = true;
}

void		Player::endNewDirLeft(void){
	left = false;
}
void		Player::endNewDirRight(void){
	right = false;
}
void		Player::endNewDirUp(void){
	up = false;
}
void		Player::endNewDirDown(void){
	down = false;
}

void		Player::bomb_explodes_callback(void *bomb){
	if (static_cast<Bomb *>(bomb)->getPlayer() == this)
		_bomb_count--;
}

void		Player::spawn_flame_callback(void *flame){
	if (static_cast<IGameEntity *>(flame)->getPosition() == glm::round(getPosition()))
		SEventManager::getInstance().raise(Event::PLAYER_DIES, this);
}

int			Player::getBombCount(void) const {
	return _bomb_count;
}

int			Player::getMaxBombNb(void) const {
	return _max_bomb;
}

void		Player::addBombToCount(void) {
	_bomb_count++;
}

void        Player::bombUp(void){
	_max_bomb++;
}
void        Player::flameUp(void){
	_flame_nb++;
}
void        Player::speedUp(void){
	_speed += 0.1;
}

int         Player::getFlameNb(void) const {
	return _flame_nb;
}