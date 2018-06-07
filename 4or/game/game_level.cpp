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
		GameObject* obj = new GameObject(ResourceManager::getTexture("container"), glm::vec2(i->width, i->height), glm::vec2(i->x, i->y), 0.0f, .4f, .8f);
		objects.push_back(obj);
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
void GameLevel::setPlayer(GamePlayer* p) {
	player = p;
	objects.push_back(player);
}
GamePlayer* GameLevel::getPlayer() {
	return player;
}
GameLevel::~GameLevel() {
	for (auto i : objects) {
		delete i;
	}
}
void GameLevel::update(GLfloat dt) {
	
}
void integrateForces(Body *b, float dt) {
	if (b->im == 0.0f) {
		return;
	}
	b->velocity += (b->force * b->im + gravity) * (dt / 2.0f);
	b->angularVelocity += b->torque * b->iI * (dt / 2.0f);
}

void integrateVelocity(Body *b, float dt) {
	if (b->im == 0.0f)
		return;

	b->position += b->velocity * dt;
	b->orient += b->angularVelocity * dt;
	b->setOrient(b->orient);
	integrateForces(b, dt);
}

void GameLevel::step() {
	// Generate new collision info
	contacts.clear();
	debug.clear();
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Body *A = objects[i]->body;

		for (unsigned int j = i + 1; j < objects.size(); ++j) {
			Body *B = objects[j]->body;
			if (A->im == 0 && B->im == 0)
				continue;
			Manifold m(A, B);
			m.solve();
				//std::cout << m.contact_count << std::endl;
			if (m.contact_count) {
				contacts.emplace_back(m);
			}
		}
	}
	for (auto i : objects) {
		i->move();
	}
	// Integrate forces
	for (unsigned int i = 0; i < objects.size(); ++i)
		integrateForces(objects[i]->body, m_dt);

	// Initialize collision
	for (unsigned int i = 0; i < contacts.size(); ++i)
		contacts[i].init();

	// Solve collisions
	
	for (int j = 0; j < m_iterations; ++j){
		for (unsigned int i = 0; i < contacts.size(); ++i){
			debug.push_back(new DebugText(std::string("Contact"), contacts[i].A->position));
			contacts[i].applyImpulse();
		}
	}
	// Integrate velocities
	for (unsigned int i = 0; i < objects.size(); ++i)
		integrateVelocity(objects[i]->body, m_dt);
	// Correct positions
	for (unsigned int i = 0; i < contacts.size(); ++i)
		contacts[i].positionCorrection();

	// Clear all forces
	for (unsigned int i = 0; i < objects.size(); ++i) {
		objects[i]->update();
		Body *b = objects[i]->body;
		b->force = glm::vec2(0);
		b->torque = 0;
		b->angularVelocity = 0;
	}
}

void GameLevel::addEnemy(glm::vec2 pos) {
	GameEnemy* en = new GameEnemy(ResourceManager::getTexture("enemy"), glm::vec2(100, 200), pos, 0.1f);
	objects.push_back(en);
}