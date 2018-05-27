#include "game_object.h"

#include <iostream>
#include <math.h>
#include <fstream>
GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), 
	isSolid(false){

}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color, glm::vec2 vel):
	position(pos), size(size), velocity(vel), color(color), rotation(0.0f), sprite(sprt), 
	isSolid(false) {
}
GLfloat magnitude(glm::vec2 input) {
	return sqrt(pow(input.x, 2) + pow(input.y, 2)); //dont sqrt if possible
}
GLfloat calcAng(glm::vec2 input) {
	GLfloat ang = acos(input.x / magnitude(input));
	if (input.y < 0) {
		ang += (GLfloat)(4 * atan(1));
	}
	return ang;
}
void GameObject::resolveCollision(GameObject* obj, GLfloat dt) {
	glm::vec2 rv = obj->getVel() - velocity;
	float velAlongNormal = glm::dot(rv, normal(obj));

	if (velAlongNormal > 0) {
		return;
	}

	float e;
	if (mat.restitution >= obj->mat.restitution) {
		e = obj->mat.restitution;
	}
	else {
		e = mat.restitution;
	}
	float j = -(1 + e) * velAlongNormal;
	j /= mass_data.invMass + obj->mass_data.invMass;
	glm::vec2 impulse = j * normal(obj);
	velocity -= mass_data.invMass * impulse;
	obj->velocity += obj->mass_data.invMass * impulse;

}
GLboolean GameObject::collide(GameObject* obj) { ///Two axis collision
	//glm::vec2 intperPos = interpolate(dt);
	//glm::vec2 objPos = obj->interpolate(dt);

	bool colX = position.x + size.x >= obj->position.x && obj->position.x + obj->size.x >= position.x;
	bool colY = position.y + size.y >= obj->position.y && obj->position.y + obj->size.y >= position.y;
	return colX && colY;
}
void GameObject::positionCorrection(GameObject* obj) {
	const GLfloat slop = 0.01; // usually 0.01 to 0.1
	const GLfloat percent = 0.2; // 0.2-0.8
								 //glm::vec2 correction = penetrationDepth//
}
glm::vec2 GameObject::normal(GameObject* obj) {
	///Axis rectangle assumption
	///Check collision first
	std::ofstream file;
	file.open(".\\logs\\normals.txt", std::ios_base::app);
	file << "==CALC NORMAL==\n";
	file << "Collider: ";
	for (unsigned int i = 0; i < getVerticies().size(); i++) {
		file << i << "(" << getVerticies()[i].x << ", " << getVerticies()[i].y << ")\t";
	}
	file << "\n";
	file << "Collidee: ";
	for (unsigned int i = 0; i < obj->getVerticies().size(); i++) {
		file << i << "(" << obj->getVerticies()[i].x << ", " << obj->getVerticies()[i].y << ")\t";
	}
	file << "\n";
	GLfloat closest = INFINITY;
	int indexj = -1;
	int indexi = -1;

	for (unsigned int i = 0; i < getVerticies().size(); i++) {
		for (unsigned int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcDist(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j + 1) % (obj->getVerticies().size())]);
			file << "i:" << i << " j:" << j << " dist:" << temp << "\n";
			if (closest > temp && temp >= 0) {
				closest = temp;
				indexj = j;
				indexi = i;
			}
		}
	}
	file << "indexj:" << indexj << " closest:" << closest << "\n";
	//std::cout << "j:" << indexj << ", j+1:" << (indexj + 1) % obj->getVerticies().size() << std::endl;
	if (indexj == -1) {
		return glm::vec2(0, 0);
	}
	int indexj1 = (indexj + 1) % obj->getVerticies().size();
	GLfloat dx = obj->getVerticies()[indexj].x - obj->getVerticies()[indexj1].x;
	GLfloat dy = obj->getVerticies()[indexj].y - obj->getVerticies()[indexj1].y;
	glm::vec2 linevec(dx, dy);
	glm::vec2 pointvec(obj->getVerticies()[indexj] - getVerticies()[indexi]);

	glm::vec2 normal = glm::vec2(-dy, dx);
	normal = glm::normalize(normal);
	GLfloat diff = calcAng(linevec) - calcAng(pointvec);
	file.close();
	std::cout << diff / 2 * atan(1) << std::endl;
	if (diff < 0 && diff >= -2 * atan(1)) {
		return -normal;
	}
	if (diff < -6 * atan(1)) {
		return normal;
	}
	if (diff > 0 && diff <= 2 * atan(1)) {
		return normal;
	}
	if (diff > 6 * atan(1)) {
		return -normal;
	}
	
	return glm::vec2(0);
}
GLfloat GameObject::getCloseDist(GameObject* obj) {
	GLfloat closest = INFINITY;
	int indexj = -1;

	for (unsigned int i = 0; i < getVerticies().size(); i++) {
		for (unsigned int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcDist(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j + 1) % (obj->getVerticies().size())]);
			if (closest > temp && temp >= 0) {
				closest = temp;
			}
		}
	}
	return closest;
}
GLfloat GameObject::calcDist(glm::vec2 dir, glm::vec2 point, glm::vec2 line1, glm::vec2 line2) {
	//Dir * time = 
	//function y = (dir.y/dir.x)(x-point.x)+point.y
	//function y = ((line1.y-line2.y)/(line1.x-line1.y))(x-line1.x) + line1.y
	//Redo
	std::ofstream file;
	file.open(".\\logs\\normals.txt", std::ios_base::app);
	file << "Vertex\t: (" << point.x << ", " << point.y << ")\n";
	file << "Dir\t\t: (" << dir.x << ", " << dir.y << ")\n";
	
	file << "Line1\t: (" << line1.x << ", " << line1.y << ")\n";
	file << "Line2\t: (" << line2.x << ", " << line2.y << ")\n";
	GLfloat slope1 = dir.y / dir.x;								//M1
	GLfloat slope2 = (line1.y - line2.y) / (line1.x - line2.x);	//M2
	GLfloat yCo1 = 1;
	GLfloat yCo2 = 1;
	GLfloat const1 = point.y - (slope1 * point.x);
	GLfloat const2 = line1.y - (slope2 * line1.x);
	file << "slope1, slope2\t: " << slope1 << ", " << slope2 << "\n";
	file << "yCo1, yCo2\t\t: " << yCo1 << ", " << yCo2 << "\n";
	file << "const1, const2\t: " << const1 << ", " << const2 << "\n";

	if (slope1 == slope2 ) {
		return -1;
	}
	///Bad coding lol
	if (dir.x == 0) {
		slope1 = 1;
		const1 = point.x;
		yCo1 = 0;
	}
	if (line1.x - line2.x == 0) {
		slope2 = 1;
		const2 = line1.x;
		yCo2 = 0;
	}
		//std::cout << slope1 << ", " << slope2 << ", " << yCo1 << ", " << yCo2 << ", " << const1 << ", " << const2 << std::endl;
	/*
	-slope1 x + yCo1 y = const1
	-slope2 x + yCo2 y = const2
	[-slope1 yCo1 C]
	[D E F]
	*/
	
	GLfloat xval = ((const1 * yCo2) - (yCo1 * const2)) / (((-slope1) * yCo2) - (yCo1 * (-slope2)));
	GLfloat yval = (((-slope1) * const2) - (const1 * (-slope2))) / (((-slope1) * yCo2) - (yCo1 * (-slope2)));
	file << "POI\t\t: (" << xval << ", " << yval << ")\n";
	if (dir.y == 0 && line1.x - line2.x == 0) {
		xval = line1.x;
		yval = point.y;
	}
	if (dir.x == 0 && line1.y - line2.y == 0) {
		xval = point.x;
		yval = line1.y;
	}
	//std::cout << (xval / dir.x) << ", " << (yval / dir.y) << std::endl;
	/*GLfloat xval = (point.y - line1.y + (line1.x * slope2) + (point.x * slope1)) /
		(slope2 - slope1);
	return xval / point.x;*/


	if (((xval >= line1.x && xval <= line2.x) || (xval >= line2.x && xval <= line1.x)) &&
		((yval >= line1.y && yval <= line2.y) || (yval >= line2.y && yval <= line1.y))) {
		return magnitude(glm::vec2(xval,yval)-point);
	}

	else return -1;
}
std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}
GLfloat GameObject::calcTime(GLfloat dist) {
	//std::cout << "Dist:\t" << dist << " Accel:\t" << sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2));
	if (abs(acceleration.x) > 0 || abs(acceleration.y) > 0) {
		return sqrt((dist) / sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2)));
	}
	return (dist) / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}


void GameObject::move(GLfloat dt) {

}
glm::vec2 GameObject::interpolate(GLfloat dt) {
	return position;
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
	//std::cout << position.x << ", " << position.y << std::endl;
}
///Get/Set Land
glm::vec2 GameObject::getPos() const {
	return position;
}
glm::vec2 GameObject::getSize() const {
	return size;
}
glm::vec2 GameObject::getVel() const {
	return velocity;
}
void GameObject::move(GLfloat dt) {

	velocity += mass_data.invMass * force * dt;
	position += velocity * dt;
}
GLboolean GameObject::relocate(glm::vec2 loc) {
	position = loc;
	return true;
}