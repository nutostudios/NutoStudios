#include <math.h>
#include <iostream>

template<class TYPE>
class vec3 {
public: 
	TYPE x, y, z;

	vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	vec3(const vec3& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}

	vec3(const TYPE& x, const TYPE& y, const TYPE& z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3 operator +(const vec3& v) {
		vec3 ret;

		ret.x = this->x + v.x;
		ret.y = this->y + v.y;
		ret.z = this->z + v.z;

		return ret;
	}

	vec3 operator -(const vec3& v) {
		vec3 ret;

		ret.x = this->x - v.x;
		ret.y = this->y - v.y;
		ret.z = this->z - v.z;

		return ret;
	}

	const vec3& operator +=(const vec3& v) {

		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}

	const vec3& operator -=(const vec3& v) {

		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	bool operator ==(const vec3& v) {
		return (this->x == v.x && this->x == v.x && this->x == v.x);
	}

	bool operator !=(const vec3& v) {
		return (!(this->x == v.x && this->x == v.x && this->x == v.x));
	}

	bool is_zero(const vec3& v) {
		return (this->x == 0 && this->x == 0 && this->x == 0);
	}

	void zero() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	TYPE distance_to(const vec3& v) const {
		TYPE dx = v.x - this->x;
		TYPE dy = v.y - this->y;
		TYPE dz = v.z - this->z;

		return (sqrt(dx + dy + dz));	
	}

	void normalize() {
		TYPE dividend = sqrt((this->x)*(this->x) + (this->y)*(this->y) + (this->z)*(this->z));
		this->x /= dividend;
		this->y /= dividend;
		this->z /= dividend;
	}

	TYPE dot_product(const vec3& v) const {
		return (this->x*v.x + this->y*v.y + this->z*v.z);
	};

	TYPE angle_between(const vec3& v) const {
		return acos(this->dot_product(v) /
			(sqrt((this->x)*(this->x) + (this->y)*(this->y) + (this->z)*(this->z)) *
				sqrt((v.x)*(v.x) + (v.y)*(v.y) + (v.z)*(v.z))));
			
	}
};

typedef vec3<int> vec3Di;
typedef vec3<float> vec3Df;

int main() {
	vec3Df vecA = vec3Df(1.0f, 3.0f, 2.0f);
	vec3Df vecB = vec3Df(16.0f, -3.0f, 0.0f);

	float angle = vecA.angle_between(vecB);

	std::cout << angle << std::endl;

	system("pause");
	return 0;
}
