#pragma once



#include "Matrix4x4.h"
#include "Vector3.h"
#pragma once
#include <assert.h>

#include <cmath>
#include <stdio.h>

Matrix4x4 Add(const Matrix4x4& mt1, const Matrix4x4& mt2);

Matrix4x4 Subtract(const Matrix4x4& mt1, const Matrix4x4& mt2);

Matrix4x4 Multiply(const Matrix4x4& mt1, const Matrix4x4& mt2);

Matrix4x4 Inverse(const Matrix4x4& mt1);

Matrix4x4 Transpose(const Matrix4x4& mt1);

Matrix4x4 MekeIdentity4x4();
// 回転
Matrix4x4 MakeTranslateMatrix(const Vector3 &translate);
// 拡大
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 同時座標変換
Vector3 Transform(const Vector3 &vector, const Matrix4x4 &matrix);

// 回転X
Matrix4x4 MakeRotateXMatrix(float radian);
// 回転Y
Matrix4x4 MakeRotateYMatrix(float radian);
// 回転Z
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
