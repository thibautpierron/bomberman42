/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoundManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 14:32:15 by lfourque          #+#    #+#             */
/*   Updated: 2018/01/30 18:31:19 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOUND_MANAGER_HPP
# define SOUND_MANAGER_HPP

# include <SDL.h>
# include <SDL_mixer.h>
# include <iostream>
# include <random>
# include "SEventManager.hpp"
# include "GameParams.hpp"
# include "bomberman.hpp"
# include "Bonus.hpp"
# include "Player.hpp"

# define SAMPLE_RATE 44100
# define BYTES_PER_OUTPUT_SAMPLE 1024

class SoundManager
{
    public:
        SoundManager();
        ~SoundManager();

    private:
        Mix_Music   *menu_music;
        Mix_Music   *victory_music;
        Mix_Music   *lose_music;
        Mix_Music   *brawl_music;
        Mix_Music   *campaign_music;
        Mix_Chunk   *boom;
        Mix_Chunk   *boom2;
        Mix_Chunk   *death;
        Mix_Chunk   *death2;
        Mix_Chunk   *bonus;
        Mix_Chunk   *hover;
        Mix_Chunk   *click;

        Mix_Music   *current_music;

        float       masterVolume;
        float       musicVolume;
        float       effectsVolume;

        std::default_random_engine  randomGenerator;

        void    playMusic(void *);
        void    restartMusic(void *p);
        void    playMenuMusic(void *);
        void    playBoom(void *);
        void    playPickupBonus(void *);
        void    playUISound(void *);
        void    playDeath(void *);

        void    playWinMusic(void *);
        void    playLoseMusic(void *);

        void    setMasterVolume(void *);
        void    setEffectsVolume(void *);
        void    setMusicVolume(void *);
        void    updateVolume();

        SoundManager(SoundManager const &);
        SoundManager &  operator=(SoundManager const &);

};

# endif