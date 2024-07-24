#pragma once
#include "MakeMatrix.h"
#include "Vector3.h"
struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2);
//void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjection, const Matrix4x4& viewportMatrix, uint32_t color);