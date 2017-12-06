
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
		END_PLAYER_LEFT, END_PLAYER_RIGHT, END_PLAYER_UP, END_PLAYER_DOWN,
		SPAWN_BOMB,
		CAMERA_LEFT, CAMERA_RIGHT, CAMERA_UP, CAMERA_DOWN, RESET_CAMERA, FOLLOW_PLAYER, DEBUG_MODE,
		GUI_TOGGLE, GUI_BASE_MENU,
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