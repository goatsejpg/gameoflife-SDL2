#ifndef STRUCTS_H
#define STRUCTS_H

struct Vec2 {
	short x,y;

	Vec2 () = default;
	Vec2 (short _x, short _y) {x=_x;y=_y;}
};

struct Vec2INT {
	int x,y;
	
	Vec2INT () = default;
	Vec2INT (int _x, int _y) {x=_x;y=_y;}
};

using V = const Vec2&;

bool operator==(V a, V b) {
	return a.x == b.x && a.y == b.y;
}
bool operator!=(V a, V b) {
	return !(a == b);
}

using Vi = const Vec2INT&;

bool operator==(Vi a, Vi b) {
	return a.x == b.x && a.y == b.y;
}
bool operator!=(Vi a, Vi b) {
	return !(a == b);
}

// Vec2 and Vec2INT comparisons

bool operator==(Vi a, V b) {
	return a.x == (int)b.x && a.y == (int)b.y;
}

bool operator!=(Vi a, V b) {
	return !(a == b);
}

Vec2 Vec2INTtoVec2 (const Vec2INT& a) {
	return Vec2((short)a.x,(short)a.y);
}

#endif // STRUCTS_H
