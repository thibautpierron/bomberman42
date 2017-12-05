/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 13:37:11 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/05 10:37:39 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEM_HPP
# define PARTICLESYSTEM_HPP

# include <glm/gtc/matrix_transform.hpp>
# include <OpenGL/gl3.h>
# include <vector>
# include "Shader.hpp"

class ParticleSystem {
	public:
		enum type { FIRE, BOMB, RAIN };
		
		ParticleSystem(glm::vec3 position, type t);
		~ParticleSystem();

		void	draw(Shader *shader);
	private:
		ParticleSystem();

		void	updateFire(Shader *shader);
		void	updateBomb(Shader *shader);
		void	updateRain(Shader *shader);
		void	setInstanceBuffer(std::vector<glm::mat4> const & data);

		unsigned int particleNbr;
		std::vector<glm::vec3>	positions;
		type					type;
		glm::vec3				oriPos;
		// glm::vec3	velocity;
		// glm::vec4	color;
		// float life;

		GLuint vbo;
		GLuint vao;
		GLuint ebo;
		GLuint ibo;
};

#endif