/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RainParticles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 11:52:27 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/31 10:36:53 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RainParticles.hpp"

RainParticles::RainParticles() {
	running = false;
	type = RAIN;
    particleNbr = 1000;
    oriPos = glm::vec3(-10.f, -5.f, 5.f);
    // for(unsigned int i = 0; i < particleNbr; i++) {
    //     float r1 = static_cast<float>((rand() % 300)) / 10.f;
    //     float r2 = static_cast<float>((rand() % 300)) / 10.f;
    //     float r3 = static_cast<float>((rand() % 300)) / 10.f;
    //     positions.push_back(oriPos + glm::vec3(r1, r2, r3));
    // }
	createBuffers();
    setInstanceBuffer();
}

RainParticles::RainParticles(RainParticles const & src) {
	*this = src;
}

RainParticles& RainParticles::operator=(RainParticles const &) {
	return *this;
}

RainParticles::~RainParticles() {
    glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

void    RainParticles::update() {
    std::vector<glm::mat4> data;
	
	for (auto i = positions.begin(); i != positions.end(); i++){

        glm::vec3 dir = glm::vec3(0.f, 0.f, - 0.2f);
        *i += dir;

        if (i->z < -5) {
			if (running) {
				float r1 = static_cast<float>((rand() % 300)) / 10.f;
				float r2 = static_cast<float>((rand() % 300)) / 10.f;
				float r3 = static_cast<float>((rand() % 300)) / 10.f;
				*i = oriPos + glm::vec3(r1, r2, r3);
				i--;
			} else {
				positions.erase(i);
				i--;
			}
		}

        glm::mat4 transform = glm::mat4();
        transform = glm::translate(transform, *i);
        transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
        transform = glm::scale(transform, glm::vec3(0.01f, 0.5f, 1.f));
        data.push_back(transform);
    }

    if (data.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void	RainParticles::startRain() {
	running = true;
	for(unsigned int i = 0; i < particleNbr; i++) {
        float r1 = static_cast<float>((rand() % 300)) / 10.f;
        float r2 = static_cast<float>((rand() % 300)) / 10.f;
        float r3 = static_cast<float>((rand() % 300)) / 10.f;
        positions.push_back(oriPos + glm::vec3(r1, r2, r3));
    }
}

void	RainParticles::stopRain() {
	running = false;
}
