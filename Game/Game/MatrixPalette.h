﻿#pragma once

#include "Math.h"

// ボーンの最大数
const size_t MAX_SKELETON_BONES = 196;

struct MatrixPalette
{
	Matrix4 mEntry[MAX_SKELETON_BONES];
};