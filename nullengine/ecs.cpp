#include "ecs.h"

#include <entt/entt.hpp>

namespace null
{
	class ObjectImpl : public Object
	{
		friend class SceneImpl;
	public:
		ObjectImpl(SceneImpl* pParentScene, ObjectBase* pParentObject);

		void Add() override;

		void Detach(Object* obj) override;

		void Release() override;


	private:
		entt::entity m_id;
		ObjectBase* m_parent;
		SceneImpl* m_parentScene;
		std::vector<ObjectImpl*> m_children;
	};

	class SceneImpl : public Scene
	{
		friend class ObjectImpl;
	public:

		void Add() override
		{
			ObjectImpl* obj = new ObjectImpl(this, this);
			m_objects.emplace_back(obj);
		}

		void Detach(Object* obj) override
		{
			for (auto it = m_objects.begin(); it != m_objects.end(); ) {
				if ((*it)->m_id == dynamic_cast<ObjectImpl*>(obj)->m_id)
				{
					it = m_objects.erase(it);
				}
				it++;
			}
		}

	private:
		entt::registry m_registry;
		std::vector<ObjectImpl*> m_objects;
	};	

	class ObjectImpl : public Object
	{
		friend class SceneImpl;
	public:
		ObjectImpl(SceneImpl* pParentScene, ObjectBase* pParentObject)
			: m_parent(pParentObject), m_parentScene(pParentScene)
		{
			m_id = m_parentScene->m_registry.create();
		}

		void Add() override
		{
			ObjectImpl* obj = new ObjectImpl(m_parentScene, this);
			m_children.emplace_back(obj);
		}

		void Detach(Object* obj) override
		{
			for (auto it = m_children.begin(); it != m_children.end(); ) {
				if ((*it)->m_id == dynamic_cast<ObjectImpl*>(obj)->m_id)
				{
					it = m_children.erase(it);
				}
				it++;
			}
		}

		void Release() override
		{
			m_parent->Detach(this);
			for (auto child : m_children)
				child->Release();
			m_parentScene->m_registry.destroy(m_id);
			delete this;
		}

		/*XMMATRIX Transformation()
		{
			if (m_parent != NULL)
				return GetCurrentTransform * m_parent->Transformation();
			else
				return GetCurrentTransform;
		}*/


	private:
		entt::entity m_id;
		ObjectBase* m_parent;
		SceneImpl* m_parentScene;
		std::vector<ObjectImpl*> m_children;
	};
}