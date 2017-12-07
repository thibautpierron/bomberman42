
#include "AI.hpp"
# include "Bomb.hpp"



class Spot {
    public:
        bool                _free;
        bool                _safe;
        int                 _dist;
        
        Spot() : _free(false) {};
        Spot(bool f, bool s) : _free(f), _safe(s) {};
        ~Spot() {};
};

AI::AI(Player *player) : _player(player), _debug_cubes(new std::vector<glm::vec2>()){
    SEventManager & em = SEventManager::getInstance();
    em.raise(Event::AIPTR, _debug_cubes);
}  

AI::~AI() {

}

bool      AI::can_place_bomb(void){
    return (_player->getBombCount() < _player->getMaxBombNb());
}

bool    AI::is_safe(glm::ivec2 pos, std::vector<IGameEntity *> & entities){
    for (auto it : entities){
        if (it->getType() == Type::BOMB && ((it->getPosition().x == pos.x && std::abs(it->getPosition().y - pos.y) <= static_cast<Bomb*>(it)->getFlameNb())
        || (it->getPosition().y == pos.y && std::abs(it->getPosition().x - pos.x) <= static_cast<Bomb*>(it)->getFlameNb())))
            return false;
        if (it->getType() == Type::FLAME && (it->getPosition().x == pos.x && it->getPosition().y == pos.y))
            return false;
    }
    return true;
}

void    AI::updateMapDistRec(glm::ivec2 pos, int rec){
    auto it = _map.find(pos);
    if (it != _map.end()){
        if (it->second._free && (it->second._dist == -1 || rec < it->second._dist)){
            it->second._dist = rec;
            updateMapDistRec(glm::ivec2(pos.x + 1, pos.y), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x, pos.y + 1), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x, pos.y - 1), rec + 1);
            updateMapDistRec(glm::ivec2(pos.x - 1, pos.y), rec + 1);
        }
    }
}

void    AI::updateObjective(void){
    updateMapDistRec(glm::round(_player->getPosition()), 0);
    auto min = &(*(_map.begin()));
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && (i.second._dist < min->second._dist || min->second._dist == -1))
            min = &i;
    }
    _objective = min->first;
}

void    AI::updateMap(Map const & map, std::vector<IGameEntity *> & entities){
    glm::vec2 mapSize = map.getSize();
    
    _map.clear();
    for(unsigned int i = 0; i < mapSize.x; i++) {
        for(unsigned int j = 0; j < mapSize.y; j++) {
            _map.insert(std::pair<glm::vec2, Spot>(glm::vec2(i, j), Spot(!map.hasBloc(glm::vec2(i, j)), is_safe(glm::ivec2(i, j), entities))));
        }
    }
}

void    AI::run_to_safety(Map const & map, std::vector<IGameEntity *> & entities){
    (void)map;
    (void)entities;
    if (is_safe(glm::round(_player->getPosition()), entities))
        return;
    updateObjective();
    // runToObjective();
}

void    AI::compute(Map const & map, std::vector<IGameEntity *> & entities) {
    (void)entities;
    updateMap(map, entities);
    updateDebugCubes(map, entities);
    // if (can_place_bomb()){
    //     SEventManager::getInstance().raise(SPAWN_BOMB, _player);
    // }
    run_to_safety(map, entities);
}

bool    AI::shouldAppearInDebug(glm::ivec2 pos){
    // return _map[pos]._free && _map[pos]._safe;
    return pos == _objective;
}

void    AI::updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) {
    glm::vec2 mapSize = map.getSize();

    (void)entities;
    _debug_cubes->clear();
    if (_map.size() == 0)
        return;

    for(unsigned int i = 0; i < mapSize.x; i++) {
        for(unsigned int j = 0; j < mapSize.y; j++) {
            if (shouldAppearInDebug(glm::ivec2(i, j)))
                _debug_cubes->push_back(glm::vec2(i, j));
        }
    }
}