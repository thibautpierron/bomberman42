/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:45:55 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/21 12:27:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "bomberman.hpp"
# include "AGameEntity.hpp"
# include "Bomb.hpp"
# include "SEventManager.hpp"

class Bomb;

class Player : public AGameEntity {
public:
    Player(const glm::vec2 & pos, int nb, PlayerColor::Enum color);

    glm::vec2	getGraphicalDirection() const;
    void		setGraphicalDirection(glm::vec2 dir);
    glm::vec2	getNewDirection(void);

    int         getPlayerNb(void);
    PlayerColor::Enum    getColor(void);

    void		newDirLeft(void);
    void		newDirRight(void);
    void		newDirUp(void);
    void		newDirDown(void);
    void		endNewDirLeft(void);
    void		endNewDirRight(void);
    void		endNewDirUp(void);
    void		endNewDirDown(void);

    int			getBombCount(void) const;
    int			getMaxBombNb(void) const;
    void		addBombToCount(void);

    void        bombUp(void);
    void        flameUp(void);
    void        speedUp(void);

    int         getFlameNb(void) const;
    float       getSpeedMult(void) const ;

    void        bomb_explodes_callback(void *bomb);
    void		spawn_flame_callback(void *flame);
    void    	enemy_move_callback(void *enemy);

private:
    int         _player_number;
    int         _max_bomb;
    int         _flame_nb;
    float       _speed_mult;
    int         _bomb_count;
    bool        left, right, up, down;
	glm::vec2	_graphicalDirection;
    PlayerColor::Enum   _color;
};

#endif