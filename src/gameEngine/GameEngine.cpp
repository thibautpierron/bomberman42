/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 16:18:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"

GameEngine::GameEngine() : _map(new Map()) {
	rapidjson::Value * grid;

	this->_loader.setPath("maps/map.json");
	if (this->_loader.load() != 1)
	{
		return ;
	}

	grid = this->_loader.getValue("grid");
	if (!grid[0].IsArray())
    	return ;

    for (unsigned int i = 0 ; i < grid[0].Size() ; i++)
    {
    	if (!grid[0][i].IsArray())
    		return ;
    	for (unsigned int j = 0 ; j < grid[0][i].Size() ; j ++)
    	{
    		if (i == 0)
    			this->_map->setSize(glm::vec2(grid[0][i].Size(), grid[0].Size()));
    		if (!grid[0][i][j].IsInt())
    			return ;
    		switch (grid[0][i][j].GetInt())
    		{
    		case -2: // undestroyable bloc
    			this->_map->addUndestroyableBlocs(glm::vec2(j,i));
    			break;
    		case -1: // destroyable bloc
    			this->_map->addDestroyableBlocs(glm::vec2(j,i));
    			break;
    		case 0: // case vide
    			break;
    		case 1: // player set
    			glm::vec2		vec(static_cast<float>(j), static_cast<float>(i));
    			Player *	player = new Player(vec);
    			this->_entityList.push_back(player);
    			break;
    		}
    	}
    }
}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> actions) {
	for (std::vector<IGameEntity *>::iterator i = _entityList.begin(); i != _entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:	compute_player(*i, actions);
			default:			(void)actions;
		}
	}
}

glm::vec2	GameEngine::compute_direction(std::vector<Action::Enum> actions) {
	glm::vec2 v(0., 0.);
	for (std::vector<Action::Enum>::iterator i = actions.begin(); i != actions.end(); i++) {
		switch (*i) {
			case Action::LEFT: 		v.x += -1.; break;
			case Action::RIGHT: 	v.x += 1.; break;
			case Action::UP: 		v.y += 1.; break;
			case Action::DOWN: 		v.y += -1.; break;
			default:				(void)v;
		}
	}
	return (glm::normalize(v));
}

void	GameEngine::compute_player(IGameEntity *p, std::vector<Action::Enum> actions){
	glm::vec2	v = compute_direction(actions);
	(void)p;
	if (!std::isnan(v.x) && !std::isnan(v.y)){ // glm::isnan wouldn't compile for some reason...
		p->setState(State::MOVING);
		p->setDirection(v);
		p->setSpeed(0.1);
	}
	else
	{
		p->setState(State::STANDING);
		p->setSpeed(0.);
	}
	if (p->getState() == State::MOVING){
		p->setPosition(p->getPosition() + (p->getDirection() * p->getSpeed()));
	}
}

Map const &		GameEngine::getMap() const {
	return *(this->_map);
}

const std::vector<IGameEntity * >	GameEngine::getEntityList() const {
	return this->_entityList;
}