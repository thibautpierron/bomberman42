
#ifndef SEVENTMANAGER_HPP
# define SEVENTMANAGER_HPP

# include <iostream>
# include <map>
# include <vector>

#define MEMBER_CALLBACK(funPtr) \
std::bind(&funPtr, this, std::placeholders::_1)

#define MEMBER_CALLBACK_WITH_INSTANCE(funPtr, instancePtr) \
std::bind(&funPtr, instancePtr, std::placeholders::_1)

typedef std::function<void(void*)> CallbackType;

namespace Event {
	enum Enum	{
		KEYDOWN, KEYUP,
		PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN,
		DROP_BOMB,
		TOGGLE,
		SCREEN_FORMAT_UPDATE,
		NEW_GAME, LOAD_GAME, QUIT_GAME,
		GAME_WIN, GAME_FINISH,
		PLAYER_MOVE
	};
}

class SEventManager {
    public:
    	~SEventManager();
		static SEventManager& getInstance();

		void	registerEvent(Event::Enum event, CallbackType);
		void	raise(Event::Enum event, void*);

	private:
		std::map<Event::Enum, std::vector<CallbackType>> _map;

        SEventManager();
        SEventManager(SEventManager const&);              // Don't Implement.
        void operator=(SEventManager const&); // Don't implement
	
};

#endif