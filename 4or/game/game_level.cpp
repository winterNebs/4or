#include "game_level.h"

GameLevel::GameLevel(std::string f) {
	load(f);
}
void GameLevel::load(std::string file) {
	objects.clear();
	svgReader test = svgReader(file);
	init(test.getShapes());
}
void GameLevel::init(std::vector<shape*> blockData) {
	///Calculate max size;
	for (auto i : blockData) {
		GameObject* obj = new GameObject(ResourceManager::getTexture("container"));
		objects.push_back(obj->initRect(glm::vec2(i->x,i->y), glm::vec2(i->width,i->height)));
		obj->body->setStatic();
		obj->body->restitution = 0.2f;
		obj->body->dynamicFriction = 0.2f;
		obj->body->staticFriction = 0.4f;

	}
}
void GameLevel::draw(SpriteRenderer &renderer) {
	for (auto i : objects) {
		i->draw(renderer);
	}
}
GLboolean GameLevel::isCompleted() {
	return GL_FALSE;
}
void GameLevel::setPlayer(GameObject* p) {
	player = p;
	objects.push_back(player);
}
GameObject* GameLevel::getPlayer() {
	return player;
}
GameLevel::~GameLevel() {
	for (auto i : objects) {
		delete i;
	}
}
void GameLevel::update(GLfloat dt) {
	
}
void IntegrateForces(Body *b, float dt) {
	if (b->im == 0.0f) {
		return;
	}
	b->velocity += (b->force * b->im + gravity) * (dt / 2.0f);
	b->angularVelocity += b->torque * b->iI * (dt / 2.0f);
}

void IntegrateVelocity(Body *b, float dt) {
	if (b->im == 0.0f)
		return;

	b->position += b->velocity * dt;
	b->orient += b->angularVelocity * dt;
	b->setOrient(b->orient);
	IntegrateForces(b, dt);
}

void GameLevel::step() {
	// Generate new collision info
	contacts.clear();
	for (int i = 0; i < objects.size(); ++i) {
		Body *A = objects[i]->body;

		for (int j = i + 1; j < objects.size(); ++j) {
			Body *B = objects[j]->body;
			if (A->im == 0 && B->im == 0)
				continue;
			Manifold m(A, B);
			m.solve();
			if (m.contact_count) {
				contacts.emplace_back(m);
			}
		}
	}

	// Integrate forces
	for (int i = 0; i < objects.size(); ++i)
		IntegrateForces(objects[i]->body, m_dt);

	// Initialize collision
	for (int i = 0; i < contacts.size(); ++i)
		contacts[i].init();

	// Solve collisions
	for (int j = 0; j < m_iterations; ++j)
		for (int i = 0; i < contacts.size(); ++i)
			contacts[i].applyImpulse();

	// Integrate velocities
	for (int i = 0; i < objects.size(); ++i)
		IntegrateVelocity(objects[i]->body, m_dt);

	// Correct positions
	for (int i = 0; i < contacts.size(); ++i)
		contacts[i].positionCorrection();

	// Clear all forces
	for (int i = 0; i < objects.size(); ++i)
	{
		Body *b = objects[i]->body;
		b->force = glm::vec2(0);
		b->torque = 0;
	}
}

