/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NuklearGUI.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 12:19:12 by lfourque          #+#    #+#             */
/*   Updated: 2018/01/24 11:44:11 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUKLEAR_GUI_HPP
# define NUKLEAR_GUI_HPP

# include <SDL.h>
# include <SDL_mixer.h>
# include <vector>
# include <stack>
# include "bomberman.hpp"
# include "Camera.hpp"
# include "SEventManager.hpp"
# include "Sdl_gl_win.hpp"
# include "GameParams.hpp"
# include "Player.hpp"
# include "Bonus.hpp"
# include <stb_image.h>


class NuklearGUI
{
    public:
        NuklearGUI(Sdl_gl_win &, Camera &);
        ~NuklearGUI();

        struct nk_context *     getContext() const;
        void                    render(bool);

        Sdl_gl_win&             getSGW() const;
        Camera&                 getCamera() const;
        
        
    private:
        Sdl_gl_win              &win;
        Camera                  &camera;
        SEventManager           &event;
        struct nk_context       *ctx;
        struct nk_font_atlas    *atlas;
        struct nk_font          *bigFont;
        struct nk_font          *mediumFont;
        struct nk_font          *smallFont;
        
        float                   menuWidth;
        float                   menuHeight;
        int                     windowWidth;
        int                     windowHeight;
        float                   optionHeight;
        bool                    screenFormatUpdate;
        
        float                   _masterVolume;
        float                   _effectsVolume;
        float                   _musicVolume;
        std::vector<int>        _starsCampaign;

        bool                    _reset_options_display;
        bool                    _reset_key_bindings_display;
        
        std::chrono::time_point<std::chrono::steady_clock>          count_down_start_time;

        std::chrono::time_point<std::chrono::steady_clock>          start_time;
        float                                                       fps;
        int                                                         frames;
        void                                                        update_fps(void);
        
        Screen::Format          screenFormat;
        
        std::stack<Menu::Enum>              _active_menu;
        std::map<Event::Enum, SDL_Keycode>  _keysMap;
        SDL_Keycode                       * _keyToChange;

        PlayerColor::Enum                   _human_player_color;
        glm::ivec3                          _human_player_bonus;
        
        void        setupProportions();
        void        setupFont();
        void        handleKey(void *);
        void        toggle(void *);
        void        bindKeyToEvent(Event::Enum, std::map<Event::Enum, SDL_Keycode> &);
        void        hover(int) const;
        
        void        renderBackgroundImage();
        void        renderHUD();           
        void        initRenderCountDown();
        void        renderCountDown();
        void        renderDebug();
        void        renderMenu();
        void        renderOptions();
        void        renderKeyBindings();
        void        renderLevelSelection();
        void        renderGameModeSelectionMenu();
        void        renderNewBrawlMenu();
        void        renderStartMenu();
        void        renderHowToPlayMenu();
        void        renderGameOverMenu();
        void        renderBrawlWinMenu();
        void        renderSelectSlot(void);
        void        renderSlot(int, int, std::string, std::string);
        
        void        setMasterVolume(void * v);
        void        setEffectsVolume(void * v);
        void        setMusicVolume(void * v);
        void        updateScreenFormat(void *f);
        void        updateHumanPlayerBonus(void *p);
        void        resetHumanPlayerBonus(void *);
        void        setStarsCampaign(void * starsCampaign);
        void        startAnimation(void *a);

        struct nk_image         loadImage(std::string const, GLint);

        std::string             toString(SDL_DisplayMode const &) const;
        std::string             toString(Screen::WindowMode) const;
        std::string             toString(Camera::Mode) const;
        std::string             toString(PlayerColor::Enum) const;
        std::string             toString(Difficulty::Enum) const;

        NuklearGUI();
        NuklearGUI(NuklearGUI const &);
        NuklearGUI &    operator=(NuklearGUI const &);
};

#endif