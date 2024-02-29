#pragma once

#include <ostream>

class Vector2
{
public:
	float x, y;
	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}
};
