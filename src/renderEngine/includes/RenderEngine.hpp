/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 14:46:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 18:10:34 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RENDERENGINE_HPP
# define RENDERENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Shader.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Map.hpp"
# include <vector>

# include <OpenGL/gl3.h>
# include <SDL.h>

class RenderEngine {
	
	public:
		RenderEngine(SDL_Window	*win, Camera &);
		~RenderEngine();
		
		void	render(Map const & map, std::vector<IGameEntity *> const & entities);
		
	private:
		
		RenderEngine();
		SDL_Window	*win;
		Shader		*shader;
		Shader		*textureShader;
		
		Model		*playerModel;
		Model		*groundModel;
		Model		*wallModel;
		Model		*brickModel;
		Camera  	&camera;

		void	renderMap(Map const & map) const;
		void	renderWall() const;
		void	renderBrick(const std::vector<DestructibleBloc>	&) const;
		void	renderPlayer(IGameEntity const *) const;
		void	renderGround() const;
		void	setCamera(glm::mat4 const &);
};

#endif