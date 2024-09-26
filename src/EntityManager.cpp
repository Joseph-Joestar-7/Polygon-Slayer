#include "EntityManager.h"
#include "Entity.h"
#include <algorithm>

EntityManager::EntityManager(){}

void EntityManager::update()
{
	for (const auto e : m_entitiesToAdd) {
		m_entities.push_back(e);
		m_entitiesMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec_] : m_entitiesMap) {
		removeDeadEntities(entityVec_);
	}
}

void EntityManager::removeDeadEntities(entityVec& vec)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[](auto it) { return !it->isActive(); }),
		vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);
	return entity;
}

const entityVec& EntityManager::getEntities()
{
	return m_entities;
}

const entityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entitiesMap[tag];
}