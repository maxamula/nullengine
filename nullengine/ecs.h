#pragma once
#include "components.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace null
{
	class Object;

	class ObjectRoot
	{
	public:
		virtual Object* Add() = 0;
		virtual void Detach(Object* obj) = 0;
		virtual XMMATRIX GetRelativeTransformation() = 0;
	};

	class Object : public ObjectRoot
	{
	public:
		virtual void Release() = 0;
		virtual Trasformation& GetTransformation() = 0;
	};

	class Scene : public ObjectRoot
	{
	public:
		virtual void Release() = 0;
	};

	Scene& CreateScene();
}