#include "ecs.h"

#include <entt/entt.hpp>

namespace null
{
	entt::registry g_registry;

	class ObjectImpl : public Object
	{
	public:
		ObjectImpl(Scene* pParentScene, ObjectRoot* pParentObject)
			: m_parent(pParentObject), m_parentScene(pParentScene)
		{
			m_id = g_registry.create();
		}

		bool operator==(Object* other)
		{
			return m_id == dynamic_cast<ObjectImpl*>(other)->m_id;
		}

		Object* Add() override
		{
			ObjectImpl* obj = new ObjectImpl(m_parentScene, this);
			m_children.push_back(obj);
			return obj;
		}

		void Detach(Object* obj) override
		{
			for (auto it = m_children.begin(); it != m_children.end(); ) {
				if ((*it)->m_id == dynamic_cast<ObjectImpl*>(obj)->m_id)
					it = m_children.erase(it);
				else
					it++;
			}
		}

		void Release() override
		{
			for (auto child : m_children)
				child->Release();
			g_registry.destroy(m_id);
			m_parent->Detach(this);
			delete this;
		}

		Trasformation& GetTransformation() override
		{
			return g_registry.get<Trasformation>(m_id);
		}

		XMMATRIX GetRelativeTransformation() override
		{
			return GetTransformation().GetMatrix() * m_parent->GetRelativeTransformation();
		}


	private:
		entt::entity m_id;
		ObjectRoot* m_parent;
		Scene* m_parentScene;
		std::vector<ObjectImpl*> m_children;
	};

	class SceneImpl : public Scene
	{
	public:

		Object* Add() override
		{
			ObjectImpl* obj = new ObjectImpl(this, this);
			m_objects.push_back(obj);
			return obj;
		}

		void Detach(Object* obj) override
		{
			for (auto it = m_objects.begin(); it != m_objects.end(); ) {
				if (dynamic_cast<ObjectImpl*>(obj)->operator==((*it)))
					it = m_objects.erase(it);
				else
					it++;
			}
		}

		void Release()
		{
			for (auto child : m_objects)
				child->Release();
			delete this;
		}

		XMMATRIX GetRelativeTransformation() override
		{
			return m_globalTransform;
		}

	private:
		XMMATRIX m_globalTransform = XMMatrixSet(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		std::vector<ObjectImpl*> m_objects;
	};
}