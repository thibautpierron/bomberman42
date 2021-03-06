/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:24:20 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 12:34:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BombManager.hpp"
#include "glm/ext.hpp"

BombManager::BombManager(Map *map, std::vector<IGameEntity *> *entityList) : 
_map(map), _entityList(entityList), _flames_to_add(new std::vector<IGameEntity *>()) {
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::PLAYER_SPAWN_BOMB, MEMBER_CALLBACK(BombManager::spawn_bomb));
    em.registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(BombManager::spawn_flame));
    em.registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(BombManager::bomb_explodes));
    em.registerEvent(Event::FLAME_DISAPEAR, MEMBER_CALLBACK(BombManager::flames_disapear));
}

BombManager::~BombManager() {
    SEventManager &em = SEventManager::getInstance();
    em.unRegisterEvent(Event::PLAYER_SPAWN_BOMB, this);
    em.unRegisterEvent(Event::SPAWN_FLAME, this);
    em.unRegisterEvent(Event::BOMB_EXPLODES, this);
    em.unRegisterEvent(Event::FLAME_DISAPEAR, this);

    delete _flames_to_add;
}

void              BombManager::spawn_bomb(void *p){
    Player *player = static_cast<Player *>(p);
    for (auto it = _entityList->begin(); it != _entityList->end(); it++){
        if (glm::round((*it)->getPosition()) == glm::round(player->getPosition()) && (*it)->getType() != Type::PLAYER)
            return ;

    }
    if (player->getBombCount() >= player->getMaxBombNb())
        return;
    player->addBombToCount();
    Bomb *b = new Bomb(glm::round(player->getPosition()), static_cast<Player *>(p));
    _entityList->push_back(b);
    SEventManager::getInstance().raise(Event::SPAWN_BOMB, b);
}

// Creates flames and stuffs
void              BombManager::bomb_explodes(void *p){
    std::vector<IGameEntity *>	*new_flames = explode(static_cast<Bomb *>(p));
    for (auto it = new_flames->begin(); it != new_flames->end(); it++){
        SEventManager::getInstance().raise(Event::SPAWN_FLAME, *it);
    }
    delete new_flames;
}

void              BombManager::spawn_flame(void *p){
    _flames_to_add->push_back(static_cast<Flame *>(p));
}

// Make flames disapear
void              BombManager::flames_disapear(void *){
}


std::vector<IGameEntity *>	*BombManager::explodeOneDir(int flames, glm::vec2 pos, glm::vec2 &dir){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();

    if (_map->hasBloc(pos)){
        if (_map->removeDestructibleBlocs(pos)){
            ret->push_back(new Flame(pos, false));
        }
    }
    else {
        ret->push_back(new Flame(pos, false));
        if (flames > 0){
            std::vector<IGameEntity *>	*tmp = explodeOneDir(--flames, pos + dir, dir);
            if (tmp->size() > 0)
                ret->insert(ret->end(), tmp->begin(), tmp->end());
            delete tmp;
        }
    }
    return ret;
}

std::vector<IGameEntity *>	*BombManager::explode(Bomb const *bomb){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();
    std::vector<IGameEntity *>	*tmp = NULL;

    ret->push_back(new Flame(bomb->getPosition(), true));
    // Four directions : Right, Up, Left, Down
    std::array<glm::vec2, 4> dir = {{ glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(-1, 0), glm::vec2(0, -1) }};

    // Looping over directions
    for (auto i = dir.begin(); i != dir.end(); i++){
        tmp = explodeOneDir(bomb->getFlameNb() - 1, bomb->getPosition() + *i, *i);
        if (tmp->size() > 0)
            ret->insert(ret->end(), tmp->begin(), tmp->end());
        delete tmp;
    }
    return ret;
}


void    BombManager::update(void){
    for (auto i = _entityList->begin(); i != _entityList->end(); i++){
        if ((*i)->getType() == Type::BOMB || (*i)->getType() == Type::FLAME)
            (*i)->update();
    }
    // Erase Bomb/Flames that needs to disapear
    for (auto j = _entityList->begin(); j != _entityList->end(); ++j){
        if (((*j)->getType() == Type::FLAME || (*j)->getType() == Type::BOMB) && (*j)->getState() == State::DYING){
            // First call destructors
            delete *j;
            *j = nullptr;
        }
    }
    // Then remove from vector
    _entityList->erase( std::remove(_entityList->begin(), _entityList->end(), nullptr), _entityList->end());

    // Add new Flames
    if (_flames_to_add->size() > 0){
        _entityList->insert(_entityList->end(), _flames_to_add->begin(), _flames_to_add->end());
        _flames_to_add->clear();
    }
}




BombManager::BombManager(void) : 
_map(new Map()), _entityList(new std::vector<IGameEntity *>()), _flames_to_add(new std::vector<IGameEntity *>()) {
}

BombManager::BombManager(BombManager const &bm) : 
_map(bm._map), _entityList(bm._entityList), _flames_to_add(new std::vector<IGameEntity *>()) {
}

BombManager   &BombManager::operator=(BombManager const &rhs){
    _map = rhs._map;
    _entityList = rhs._entityList;
    _flames_to_add = rhs._flames_to_add;
    return *this;
}