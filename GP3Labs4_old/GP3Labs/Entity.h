#pragma once
#include "pch.h"
#include "Transform.h"
#include "Component.h"
#include "Log.h"

class Entity
{
private:
	std::vector<Component*> m_components;
	Transform* m_transform = nullptr;

public:
	Entity();

	void AddComponent(Component* c);
	template<class T> void AddComponent();
	template<class T> T* GetComponent();

	void OnUpdate(float deltaTime);
	void OnRender();

	Transform* GetTransform() { return m_transform; };
};

template<class T>
void Entity::AddComponent()
{
	T* t = new T();
	Component* c = dynamic_cast<Component*>(t);
	if (c != nullptr)
	{
		c->m_entity = this;
		m_components.push_back(c);
	}
	else
	{
		LOG_DEBUG("Can't add component, does not inherit from Component",Log::Error);
	}
}

//Done by CONNER!!!!
//entity GetComponent function template goes HERE! for the ECS system requirement
template<class T>
T* Entity::GetComponent()
{
	for (auto& comp : m_components)
	{
		T* dCast = dynamic_cast<T*>(comp);
		if (dCast != nullptr)
		{
			return dCast;
		}
	}
	return nullptr;
}


