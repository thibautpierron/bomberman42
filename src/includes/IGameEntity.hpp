/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGameEntity.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:47:02 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 03:40:47 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IGAMEENTITY_HPP
# define IGAMEENTITY_HPP

# include <glm/gtc/matrix_transform.hpp>

namespace State{
	enum Enum { STANDING, MOVING, DYING };
}

namespace Type{
	enum Enum { PLAYER, BOMB};
}

class IGameEntity {
	public:
		virtual ~IGameEntity() {};
		virtual glm::vec2	getPosition() const = 0;
		virtual glm::vec2	getDirection() const = 0;
		virtual State::Enum	getState() const = 0;
		virtual float		getSpeed() const = 0;
		virtual Type::Enum	getType() const = 0;

		virtual void    	setPosition(const glm::vec2 &) = 0;
		virtual void    	setDirection(const glm::vec2 &) = 0;
		virtual void    	setState(const State::Enum &) = 0;
		virtual void    	setSpeed(const float &) = 0;
		virtual void    	setType(const Type::Enum &) = 0;
};

#endif