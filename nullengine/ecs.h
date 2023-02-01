#pragma once
#include <DirectXMath.h>
#include <entt/entt.hpp>

using namespace DirectX;

namespace null
{
	class ObjectBase;
	class Object;
	class Scene;

	class ObjectBase
	{
	public:
		virtual void Add() = 0;
		virtual void Detach(Object* obj) = 0;
	};

	class Object : public ObjectBase
	{
		virtual void Release() = 0;
	};

	class Scene : public ObjectBase
	{

	};
}