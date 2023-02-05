#pragma once
#include <DirectXMath.h>

using namespace DirectX;
namespace null
{
#pragma region TRANSFORMATION

	struct Trasformation
	{
		XMMATRIX GetMatrix()
		{
			return XMMatrixScalingFromVector(scaling) * XMMatrixRotationRollPitchYawFromVector(rotation) * XMMatrixTranslationFromVector(position);
		}
		XMVECTOR position;
		XMVECTOR rotation;
		XMVECTOR scaling;
	};

#pragma endregion
}

