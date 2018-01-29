/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoundManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 14:34:49 by lfourque          #+#    #+#             */
/*   Updated: 2018/01/29 17:46:00 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SoundManager.hpp"

SoundManager::SoundManager() : masterVolume(0.0f), musicVolume(MIX_MAX_VOLUME / 2), effectsVolume(MIX_MAX_VOLUME / 2) {
    if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, BYTES_PER_OUTPUT_SAMPLE) == -1) {
       throw std::runtime_error(Mix_GetError());
    }
    Mix_AllocateChannels(10);
    
    music = Mix_LoadMUS("assets/sounds/carnivalrides.ogg");
    boom = Mix_LoadWAV("assets/sounds/explosions_explode.wav");
    boom2 = Mix_LoadWAV("assets/sounds/explosions_explodemini.wav");
    bonus = Mix_LoadWAV("assets/sounds/pickup_bonus.wav");
    hover = Mix_LoadWAV("assets/sounds/hover.wav");
    click = Mix_LoadWAV("assets/sounds/click2.wav");
    countdown = Mix_LoadWAV("assets/sounds/countdown.wav");

    updateVolume();
    
    SEventManager & event = SEventManager::getInstance();
    event.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SoundManager::playMusic));
    event.registerEvent(Event::NEW_GAME, MEMBER_CALLBACK(SoundManager::playCountdown));
    event.registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(SoundManager::playBoom));
    event.registerEvent(Event::BONUS_ACTIVATE, MEMBER_CALLBACK(SoundManager::playPickupBonus));
    event.registerEvent(Event::MASTER_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setMasterVolume));
    event.registerEvent(Event::MUSIC_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setMusicVolume));
    event.registerEvent(Event::EFFECTS_VOLUME_UPDATE, MEMBER_CALLBACK(SoundManager::setEffectsVolume));
    event.registerEvent(Event::UI_AUDIO, MEMBER_CALLBACK(SoundManager::playUISound));
}

SoundManager::~SoundManager() {
    Mix_FreeChunk(boom);
    Mix_FreeChunk(boom2);
    Mix_FreeChunk(bonus);
    Mix_FreeChunk(hover);
    Mix_FreeChunk(click);
    Mix_FreeChunk(countdown);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void    SoundManager::playUISound(void *s) {
    UIAudio::Enum   *sound = static_cast<UIAudio::Enum*>(s);

    switch (*sound) {
        case UIAudio::HOVER: Mix_PlayChannel(-1, hover, 0); break;
        case UIAudio::CLICK: Mix_PlayChannel(-1, click, 0); break;
    }
    //delete sound;
}

void    SoundManager::playPickupBonus(void *) {
    Mix_PlayChannel(-1, bonus, 0);
}

void    SoundManager::playCountdown(void *) {
    Mix_PlayChannel(-1, countdown, 0);
}

void    SoundManager::playBoom(void *) {
    std::uniform_int_distribution<int> distribution(0,1);
    int b = distribution(randomGenerator);
    if (b)
        Mix_PlayChannel(-1, boom, 0);
    else
        Mix_PlayChannel(-1, boom2, 0);
}

void    SoundManager::playMusic(void *p) {
    GameParams  gp = *static_cast<GameParams*>(p);
    if (gp.get_game_mode() == GameMode::CAMPAIGN) {
        Mix_PlayMusic(music, -1);
    }
}

void    SoundManager::updateVolume() {
    Mix_VolumeMusic(masterVolume * musicVolume);
    Mix_VolumeChunk(boom, masterVolume * effectsVolume);
    Mix_VolumeChunk(boom2, masterVolume * effectsVolume);
    Mix_VolumeChunk(bonus, masterVolume * effectsVolume);
    Mix_VolumeChunk(hover, masterVolume * effectsVolume);
    Mix_VolumeChunk(click, masterVolume * effectsVolume);
    Mix_VolumeChunk(countdown, masterVolume * effectsVolume);
}

void    SoundManager::setMasterVolume(void * v) {
    masterVolume = *static_cast<float*>(v);
    updateVolume();
}

void    SoundManager::setEffectsVolume(void * v) {
    effectsVolume = *static_cast<float*>(v);
    updateVolume();
}

void    SoundManager::setMusicVolume(void * v) {
    musicVolume = *static_cast<float*>(v);
    updateVolume();
}


SoundManager::SoundManager(SoundManager const &s) : masterVolume(s.masterVolume), musicVolume(s.musicVolume), effectsVolume(s.effectsVolume) {
}

SoundManager   &SoundManager::operator=(SoundManager const &rhs){
    masterVolume = rhs.masterVolume;
    musicVolume = rhs.musicVolume;
    effectsVolume = rhs.effectsVolume;
    return *this;
}