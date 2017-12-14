
# include "AI.hpp"
# include "Bomb.hpp"
# include <glm/ext.hpp>


class Spot {
    public:
        bool                _free;
        bool                _safe;
        bool                _has_bonus;
        int                 _dist;
        
        Spot() : _free(false) {};
        Spot(bool f, bool s) : _free(f), _safe(s), _has_bonus(false), _dist(-1) {};
        ~Spot() {};
};

AI::AI(Player *player) : _player(player), _debug_cubes(new std::vector<glm::vec2>()), _went_far(false), _going_safely(false){
    SEventManager & em = SEventManager::getInstance();
    em.raise(Event::AIPTR, _debug_cubes);
}  

AI::~AI() {

}





/////////////////////////           UTILS           //////////////////////


Event::Enum     AI::endEvents(Event::Enum e){
    switch (e){
        case Event::PLAYER_LEFT:    return Event::END_PLAYER_LEFT;
        case Event::PLAYER_RIGHT:   return Event::END_PLAYER_RIGHT;
        case Event::PLAYER_UP:      return Event::END_PLAYER_UP;
        case Event::PLAYER_DOWN:    return Event::END_PLAYER_DOWN;
        default:                     return Event::END_PLAYER_LEFT;
    }
}

glm::vec2     AI::dirToVec(Event::Enum e){
    switch (e){
        case Event::PLAYER_LEFT:    return glm::vec2(-1, 0) ;
        case Event::PLAYER_RIGHT:   return glm::vec2(1, 0) ;
        case Event::PLAYER_UP:      return glm::vec2(0, 1) ;
        case Event::PLAYER_DOWN:    return glm::vec2(0, -1);
        default:                     return glm::vec2(0, 0);
    }
}

glm::ivec2     AI::dirToIVec(Event::Enum e){
    switch (e){
        case Event::PLAYER_LEFT:    return glm::ivec2(-1, 0) ;
        case Event::PLAYER_RIGHT:   return glm::ivec2(1, 0) ;
        case Event::PLAYER_UP:      return glm::ivec2(0, 1) ;
        case Event::PLAYER_DOWN:    return glm::ivec2(0, -1);
        default:                     return glm::ivec2(0, 0);
    }
}

Event::Enum    AI::vecToDir(glm::vec2 v){
    if (std::abs(v.x) < 0.1f && std::abs(v.y) < 0.1f)
        return Event::PLAYER_MOVE;
    if (std::abs(v.x) > std::abs(v.y)){
        return (v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    else {
        return (v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
}


std::vector<Event::Enum>  AI::vecToDirs(glm::vec2 v){
    std::vector<Event::Enum> ret;
    if (std::abs(v.x) > 0.05f){
        ret.push_back(v.x > 0 ? Event::PLAYER_RIGHT : Event::PLAYER_LEFT);
    }
    if (std::abs(v.y) > 0.05f) {
        ret.push_back(v.y > 0 ? Event::PLAYER_UP : Event::PLAYER_DOWN);
    }
    return ret;
}











bool        AI::would_be_blocked_by_bomb(void){
    Bomb    *potential_bomb = new Bomb(glm::round(_player->getPosition()), _player);
    glm::ivec2  tmp_pos = glm::ivec2(glm::round(_player->getPosition()));
    updateMapWithEntity(potential_bomb);
    delete potential_bomb;
    bool ret = aimClosestSafeSpace(&tmp_pos);
    
    // true if the above didn't find any target
    return !ret;
}


bool      AI::can_place_bomb(void){
    return (_player->getBombCount() < _player->getMaxBombNb());
}





/////////////////////////       OBJECTIVE UPDATE        ////////////////////////////////

bool    AI::aimClosestSafeSpace(glm::ivec2 *obj){
    bool ret = false;
    resetMapDist();
    updateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && i.second._safe && (i.second._dist < _map[*obj]._dist || _map[*obj]._dist == -1 || !_map[*obj]._safe)){
            *obj = i.first;
            _going_safely = false;
            ret = true;
        }
    }
    return ret;
}


static bool is_good_bonus_spot(Spot &s){
    return (s._dist != -1 && s._free && s._safe && s._has_bonus);
}

bool    AI::lookForBonus(glm::ivec2 *obj){
    bool ret = false;
    resetMapDist();
    SAFEupdateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (is_good_bonus_spot(i.second) && (i.second._dist < _map[*obj]._dist ||!is_good_bonus_spot(_map[*obj]))){
            *obj = i.first;
            _going_safely = true;
            ret = true;
        }
    }
    return (ret || is_good_bonus_spot(_map[*obj]));
}

bool    AI::aimFarthestSafeSpace(glm::ivec2 *obj){
    bool ret = false;
    if (!_went_far)
        return ret;
    _went_far = false;
    resetMapDist();
    SAFEupdateMapDistRec(glm::round(_player->getPosition()), 0);
    for (auto i : _map){
        if (i.second._dist != -1 && i.second._free && i.second._safe && (i.second._dist > _map[*obj]._dist || _map[*obj]._dist == -1 || !_map[*obj]._safe)){
            *obj = i.first;
            _going_safely = true;
            ret = true;
        }
    }
    return ret;
}


/////////////////       INTERNAL MAP        ///////////////////////////////////////

void    AI::resetMapDist(void){
    for (auto i = _map.begin(); i != _map.end(); i++){
        (*i).second._dist = -1;
    }
}

// Fills the map's _dist field, going recursively from the initial position <pos>.
// the <rec> argument should be 0.
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

// Does the same as the above function except the recursion doesn't fill the spots that aren't safe.
void    AI::SAFEupdateMapDistRec(glm::ivec2 pos, int rec){
    auto it = _map.find(pos);
    if (it != _map.end()){
        if (it->second._free && it->second._safe && (it->second._dist == -1 || rec < it->second._dist)){
            it->second._dist = rec;
            SAFEupdateMapDistRec(glm::ivec2(pos.x + 1, pos.y), rec + 1);
            SAFEupdateMapDistRec(glm::ivec2(pos.x, pos.y + 1), rec + 1);
            SAFEupdateMapDistRec(glm::ivec2(pos.x, pos.y - 1), rec + 1);
            SAFEupdateMapDistRec(glm::ivec2(pos.x - 1, pos.y), rec + 1);
        }
    }
}

void    AI::markBombRangeAsUnsafe(glm::ivec2 pos, glm::ivec2 dir, int range){
    if (_map[pos]._free == false)
        return;
    _map[pos]._safe = false;
    if (range > 0){
        markBombRangeAsUnsafe(pos + dir, dir, range - 1);
    }
}

void    AI::updateMapWithEntity(IGameEntity *entity){
    glm::ivec2 pos(entity->getPosition());
    if (entity->getType() == Type::BOMB){
        if (pos != glm::ivec2(glm::round(_player->getPosition())))
            _map[pos]._free = false;
        _map[pos]._safe = false;
        markBombRangeAsUnsafe(pos + glm::ivec2(-1, 0), glm::ivec2(-1, 0), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos + glm::ivec2(1, 0), glm::ivec2(1, 0), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos + glm::ivec2(0, -1), glm::ivec2(0, -1), static_cast<Bomb *>(entity)->getFlameNb());
        markBombRangeAsUnsafe(pos + glm::ivec2(0, 1), glm::ivec2(0, 1), static_cast<Bomb *>(entity)->getFlameNb());
    }
    else if (entity->getType() == Type::FLAME){
         _map[pos]._safe = false;
         _map[pos]._free = false;
    }
    else if (entity->getType() == Type::BONUS){
         _map[pos]._has_bonus = true;
    }
}


void    AI::updateMap(Map const & map, std::vector<IGameEntity *> & entities){
    glm::vec2 mapSize = map.getSize();
    _map.clear();
    for(int i = -1; i < mapSize.x + 1; i++) {
        for(int j = -1; j < mapSize.y + 1; j++) {
                                                                    // See Spot class defined at the start of this file.
            _map.insert(std::pair<glm::vec2, Spot>(glm::vec2(i, j), Spot(!map.hasBloc(glm::vec2(i, j)), true)));
        }
    }
    for (auto i : entities){
        updateMapWithEntity(i);
    }
}





/////////////////////////////       RUN         ////////////////////////////////////////

void    AI::runToObjective(){
    std::vector<Event::Enum>     min_dir;


    // Here we update the _dist field of the map, but with the _objective as the origin (rather than the _player's position)
    // That way we can easily determine wich Spot next to the player's position is the closest from the _objective.
    resetMapDist();
    if (_going_safely)
        SAFEupdateMapDistRec(_objective, 0);
    else
        updateMapDistRec(_objective, 0);
    
    Event::Enum dirs[4] = {Event::PLAYER_LEFT, Event::PLAYER_RIGHT, Event::PLAYER_UP, Event::PLAYER_DOWN};

    int             min_dist = _map[glm::ivec2(glm::round(_player->getPosition()))]._dist;
    
    // Iterates through the 4 possible directions
    // Determine the direction with the minimal distance from the objective.
    for (auto it : dirs){
        if (min_dist > _map[glm::ivec2(glm::round(_player->getPosition())) + AI::dirToIVec(it)]._dist && _map[glm::ivec2(glm::round(_player->getPosition())) + AI::dirToIVec(it)]._dist != -1){
            min_dir.clear();
            min_dir.push_back(it);
            min_dist = _map[glm::ivec2(glm::round(_player->getPosition())) + AI::dirToIVec(it)]._dist;
        }
    }

    // If the Spot with the minimal distance is the _player's position
    // a.k.a. we're very close from the _ojective
    if (min_dir.size() == 0 || glm::length(glm::vec2(_objective) - _player->getPosition()) < 1.f){
        // We calculate the direction(s) we want to take using the difference between the _objective and _player position vectors
        min_dir = AI::vecToDirs(glm::vec2(_objective) - _player->getPosition());
        if (min_dir.size() == 0) // if we're on the _objective.
            _went_far = true;    // this boolean is used to determine if we should update the _objective when we're trying to "go far". See aimFarthestSafeSpace
    }

    // We raise the event that ends the player's movement if the movement was previously called and isn't anymore
    for (auto i : _last_dir) {
        if (std::find(min_dir.begin(), min_dir.end(), i) == min_dir.end()){
            SEventManager::getInstance().raise(AI::endEvents(i), _player);
        }
    }
    // Remove ended directions from _last_dir
    _last_dir.erase(std::remove_if(_last_dir.begin(), _last_dir.end(), [&min_dir](Event::Enum e) {
                return (std::find(min_dir.begin(), min_dir.end(), e) == min_dir.end());
            }), _last_dir.end());
    
    // Add new movement to _last_dir for future use.
    for (auto i : min_dir){
        if (_last_dir.size() == 0 || std::find(_last_dir.begin(), _last_dir.end(), i) == _last_dir.end()){
            _last_dir.push_back(i);
        }
        SEventManager::getInstance().raise(i, _player);
    }

}












//////////////////////////      COMPUTE         /////////////////////////////



void    AI::compute(Map const & map, std::vector<IGameEntity *> & entities) {
    updateMap(map, entities);
    if (can_place_bomb()){
        if (would_be_blocked_by_bomb()){
            aimFarthestSafeSpace(&_objective);
        } else {
            SEventManager::getInstance().raise(Event::SPAWN_BOMB, _player);
            aimClosestSafeSpace(&_objective);
        }
    } else {
        if (!lookForBonus(&_objective)){
            aimClosestSafeSpace(&_objective);
        }
    }
    updateDebugCubes(map, entities);
    runToObjective();
    
}


///////////////////////////      DEBUG       //////////////////////////////

bool    AI::shouldAppearInDebug(glm::ivec2 pos){
    return _map[pos]._has_bonus;
}

void    AI::updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) {
    glm::vec2 mapSize = map.getSize();

    (void)entities;
    (void)mapSize;
    _debug_cubes->clear();
    if (_map.size() == 0)
        return;



    // for (auto i : _map) {
    //     if (shouldAppearInDebug(i.first))
    //         _debug_cubes->push_back(i.first);
    // }
    _debug_cubes->push_back(_objective);
    
}