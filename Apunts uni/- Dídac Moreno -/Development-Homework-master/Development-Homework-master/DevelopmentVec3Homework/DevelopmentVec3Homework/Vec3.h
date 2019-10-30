#ifndef _VEC3_H_
#define _VEC3_H_

#include <cmath>

template <class T>

class vec3
{
public:
	
	//Default Constructor sets the vec3 to value 0 for x,y,z.
	vec3() { x = y = z = 0; }
	vec3(T newX, T newY, T newZ) { x = newX; y = newY; z = newZ; }
	vec3(const vec3 &vecToCopy) { x = vecToCopy.x; y = vecToCopy.y; z = vecToCopy.z; }
	~vec3() {}

	//OPERATORS (+,-,+=,-=,=,==)
	vec3 operator+  (const vec3 &v2) 
	{ 
		vec3 vecret;
		vecret.x = x + v2.x;
		vecret.y = y + v2.y;
		vecret.z = z + v2.z;
		return vecret;
	}
	vec3 operator-  (const vec3 &v2) 
	{
		vec3 vecret;
		vecret.x = x - v2.x;
		vecret.y = y - v2.y;
		vecret.z = z - v2.z;
		return vecret;
	}
	void operator+= (const vec3 &v2) 
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
	}
	void operator-= (const vec3 &v2) 
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
	}
	void operator=  (const vec3 &v2) {/*X*/x = v2.x;/*Y*/ y = v2.y;/*Z*/ z = v2.z;}
	
	bool operator== (const vec3 &v2) { return (x == v2.x && y == v2.y && z == v2.z);}
	
		
	// HOMEWORK METHODS
	void normalize() 
	{
		T mod = sqrt(x*x+y*y+z*z);
		x = x / mod;
		y = y / mod;
		z = z / mod;
	
	}
	void zero() { x = 0; y = 0; z = 0;}
	bool const is_zero() { return (x==0 && y==0 && z==0); }
	T distance_to(const vec3 &v2) { return sqrt((v2.x - x) ^ 2 + (v2.y - y) ^ 2 + (v2.z - z) ^ 2); }

	//Basic GETTER Methods
	T const getX() { return x; }
	T const getY() { return y; }
	T const getZ() { return z; }

	//Printing Methods
	void const printVec()
	{
		cout<< "	" << "x: " << x << "	" << "y: " << y << "	" << "z: " << z <<endl;
	}

	void const printAllVecs(vec3 vecArr[6])
	{
		for (int i = 0; i < 6; ++i)
		{
			cout << "Vector " << i << ": " << endl;
			vecArr[i].printVec();
		}
	}
private:
	T x, y, z;

};



#endif // _VEC3_H_
