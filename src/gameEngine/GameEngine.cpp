/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/20 10:49:19 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"
#include "IndestructibleBloc.hpp"
#include "DestructibleBloc.hpp"

GameEngine::GameEngine(GameParams &gp) : _map(new Map()), 
_entityList(new std::vector<IGameEntity *>()), 
_bonusManager(new BonusManager(_entityList)),
_enemyManager(new EnemyManager(_entityList)),
_bombManager(new BombManager(_map, _entityList)), 
_playerManager(new PlayerManager()),
_gameParams(gp),
_winManager(nullptr) {
	loadMap("maps/brawl_0.json");
	if (_gameParams.get_game_mode() == GameMode::BRAWL){
		placeBrawlPlayers();
	}

}

GameEngine::~GameEngine() {}

void	GameEngine::compute() {
	// auto start = std::chrono::system_clock::now();

    if (this->_loader.getState() == -1 || !_winManager)
        return ;
	_playerManager->compute(*_map, *_entityList);
	_collisionsManager.moves(*_map, *_entityList);
	_bombManager->update();
	_bonusManager->update();
	_enemyManager->update();
	_winManager->update(*_map, *_entityList);

	// auto end = std::chrono::system_clock::now();
	// auto elapsed = end - start;
	// std::cout << "Game Engine compute time : " << elapsed.count() << std::endl;

}

Map const &		GameEngine::getMap() const {
	return *(this->_map);
}

std::vector<IGameEntity * > &	GameEngine::getEntityList() {
	return *(this->_entityList);
}


glm::vec2 const 				* GameEngine::getPlayerPos(void) const{
	for (auto i = _entityList->begin(); i != _entityList->end(); i++){
		if ((*i)->getType() == Type::PLAYER){
			return new glm::vec2((*i)->getPosition());
		}
	}
	return new glm::vec2(0., 0.);
}

glm::vec2				GameEngine::placeBrawlAI(Player *human, int i){
	glm::vec2	v(human->getPosition());
	if (i == 0 || i == 2)
		v.y = human->getPosition().y == 0.f ? _map->getSize().y - 1.f : 0.f;
	if (i == 0 || i == 1 )
		v.x = human->getPosition().x == 0.f ? _map->getSize().x - 1.f : 0.f;
	return v;
}

void					GameEngine::placeBrawlPlayers(void){
	// Set the Human Player at a random corner
	Player *human = new Player(glm::vec2((rand() % 2) * (_map->getSize().x - 1), (rand() % 2) * (_map->getSize().y - 1)), 0);
	this->_entityList->push_back(human);
	_playerManager->setHumanPlayer(human);
	for (int i = 0; i < _gameParams.get_brawl_enemy_nb(); i++){
		Player *p = new Player(placeBrawlAI(human, i), i + 1);
		_playerManager->addPlayer(p);
		this->_entityList->push_back(p);
	}
}

void					GameEngine::loadMap(const char *path){
    rapidjson::Value * grid;
    rapidjson::Value * sun;

	this->_loader.setPath(path);
	if (this->_loader.load() != 1)
	{
		return ;
	}
	grid = this->_loader.getValue("grid");
	if (grid && !grid[0].IsArray())
    	return ;

	// MAP
    for (unsigned int i = 0 ; i < grid[0].Size() ; i++)
    {
    	if (!grid[0][i].IsArray())
    		return ;
    	for (unsigned int j = 0 ; j < grid[0][i].Size() ; j ++) {
    		if (i == 0)
    			this->_map->setSize(glm::vec2(grid[0][i].Size(), grid[0].Size()));
    		if (!grid[0][i][j].IsInt())
    			return ;
			
    		int entityType = grid[0][i][j].GetInt();

    		if (entityType == -2) // undestroyable bloc
    			this->_map->addIndestructibleBlocs(IndestructibleBloc(glm::vec2(j,i)));
    		if (entityType == 0 && ((rand() % 10) < 6)) // destroyable bloc
    			this->_map->addDestructibleBlocs(DestructibleBloc(glm::vec2(j,i)));
    		// if (entityType == 0) // case vide
    		// 	break;
			
    		// if (entityType >=1 && entityType <=5) {// players (1 is human, the rest is an AI)
    		// 	glm::vec2		vec(static_cast<float>(j), static_cast<float>(i));
    		// 	Player *	player = new Player(vec, entityType - 1);
			// 	if (player->getPlayerNb() == 0)
			// 		_playerManager->setHumanPlayer(player);
			// 	else
			// 		_playerManager->addPlayer(player);
    		// 	this->_entityList->push_back(player);
    		// }
		}
	}
	// OTHER ATTRIBUTES
	rapidjson::Value *win;
	win = this->_loader.getValue("win");
	if (!win || !win->HasMember("condition") || !win->HasMember("spot") || !win[0]["spot"].IsArray() || win[0]["spot"].Size() != 2)
		return;
	_winManager = new WinManager(static_cast<WinCondition::Enum>(win[0]["condition"].GetInt()), glm::ivec2(win[0]["spot"][0].GetFloat(), win[0]["spot"][1].GetFloat()));

    sun = this->_loader.getValue("sun");
    if (!sun || !sun->HasMember("pos") || !sun[0]["pos"].IsArray() || sun[0]["pos"].Size() != 3)
        return ;
    if (!sun[0]["pos"][0].IsFloat() || !sun[0]["pos"][1].IsFloat() || !sun[0]["pos"][2].IsFloat())
        return ;
    this->_map->setSunPos(glm::vec3(sun[0]["pos"][0].GetFloat(), sun[0]["pos"][1].GetFloat(), sun[0]["pos"][2].GetFloat()));

    if (!sun || !sun->HasMember("color") || !sun[0]["color"].IsArray() || sun[0]["color"].Size() != 3)
        return ;
    if (!sun[0]["color"][0].IsFloat() || !sun[0]["color"][1].IsFloat() || !sun[0]["color"][2].IsFloat())
        return ;
    this->_map->setSunColor(glm::vec3(sun[0]["color"][0].GetFloat(), sun[0]["color"][1].GetFloat(), sun[0]["color"][2].GetFloat()));

	rapidjson::Value *enemies = this->_loader.getValue("enemies");
	if (!enemies || !enemies->IsArray())
		return;
	for (unsigned int i = 0 ; i < enemies[0].Size() ; i++){
		if (!enemies[0][i].HasMember("pos") || !enemies[0][i].HasMember("type") || !enemies[0][i]["pos"].IsArray() || enemies[0][i]["pos"].Size() != 2 || !enemies[0][i]["pos"][0].IsInt() || !enemies[0][i]["pos"][1].IsInt() || !enemies[0][i]["type"].IsInt())
			return;
		_entityList->push_back(new Enemy(glm::vec2(enemies[0][i]["pos"][0].GetInt(), enemies[0][i]["pos"][1].GetInt()), static_cast<EnemyType::Enum>(enemies[0][i]["type"].GetInt())));
	}

}