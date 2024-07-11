#include "AABB.h"

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	bool isHit = false;
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		isHit = true;
	} else {
		isHit = false;
	}
	return isHit;
}

//void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjection, const Matrix4x4& viewportMatrix, uint32_t color) {
//	Vector3 top[4] = {
//	    {aabb.min.x, aabb.max.y, aabb.min.z},
//	    {aabb.min.x, aabb.max.y, aabb.max.z},
//	    {aabb.max.x, aabb.max.y, aabb.max.z},
//	    {aabb.max.x, aabb.max.y, aabb.min.z},
//	};
//	Vector3 bottom[4] = {
//	    {aabb.min.x, aabb.min.y, aabb.min.z},
//	    {aabb.min.x, aabb.min.y, aabb.max.z},
//	    {aabb.max.x, aabb.min.y, aabb.max.z},
//	    {aabb.max.x, aabb.min.y, aabb.min.z},
//	};
//
//	for (int index = 0; index < 4; ++index) {
//		top[index] = Transform(Transform(top[index], viewProjection), viewportMatrix);
//		bottom[index] = Transform(Transform(bottom[index], viewProjection), viewportMatrix);
//	}
//	// 上面と下面をつなぐ辺
//	for (int index = 0; index < 4; ++index) {
//		Novice::DrawLine((int)top[index].x, (int)top[index].y, (int)bottom[index].x, (int)bottom[index].y, color);
//	}
//	// 上面
//	Novice::DrawLine((int)top[0].x, (int)top[0].y, (int)top[1].x, (int)top[1].y, color);
//	Novice::DrawLine((int)top[1].x, (int)top[1].y, (int)top[2].x, (int)top[2].y, color);
//	Novice::DrawLine((int)top[2].x, (int)top[2].y, (int)top[3].x, (int)top[3].y, color);
//	Novice::DrawLine((int)top[3].x, (int)top[3].y, (int)top[0].x, (int)top[0].y, color);
//	// 下面
//	Novice::DrawLine((int)bottom[0].x, (int)bottom[0].y, (int)bottom[1].x, (int)bottom[1].y, color);
//	Novice::DrawLine((int)bottom[1].x, (int)bottom[1].y, (int)bottom[2].x, (int)bottom[2].y, color);
//	Novice::DrawLine((int)bottom[2].x, (int)bottom[2].y, (int)bottom[3].x, (int)bottom[3].y, color);
//	Novice::DrawLine((int)bottom[3].x, (int)bottom[3].y, (int)bottom[0].x, (int)bottom[0].y, color);
//}
