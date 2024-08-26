#pragma once

#include <cmath>

namespace ST
{
	/*
	namespace impl
	{
		template <int dimension, class value>
		class vector { };
		
		template <int dimension, class value>
		class vector<2, value> 
		{
			union {
				struct
				{
					value x,y;
				};
				value v[2];
			};
		};

		template <int dimension, class value>
		class vector<3, value>
		{
			union { 
				struct
				{
					value x,y,z;
				}
				value v[3];
			};
		};
	};
	*/

//template <class value, int dimension, basetype = impl::vector<dimension, value> >
template <class value>
class Vector3T 
{
public:
	// The following constructor is to allow stRectf to work 
	Vector3T(value xt, value yt) : x(xt), y(yt), z(0)		{}
	// Vector3T(int x1, int y1, int z1) : x(x1), y(y1), z(z1)  {}
	Vector3T( Vector3T & arg ) : x(arg.x), y(arg.y), z(arg.z)   {}
	Vector3T()													{}
	union
	{
		struct
		{
			float x,y,z;	// x, y, and z
		};

		float v[3];
	};

	Vector3T &  set(const value xt,const value yt,const value zt) {x = xt; y=yt; z=zt; return *this;} 
	Vector3T &  operator()(const value xt, const value yt, const value zt) {x = xt; y=yt; z=zt; return *this;}

	value dot( const Vector3T &arg2  ) const
	{
		return (x * arg2.x + y * arg2.y + z * arg2.z);
	}

	Vector3T &	normalize()
	{
		if (x != y != z != 0)
		{
			value mag = x * x + y * y + z * z;
			mag = 1 / std::sqrt(mag);
			x *= mag;
			y *= mag;
			z *= mag;
		}
		return *this;
	}


	// this = a x b
	Vector3T &	cross( const Vector3T  &arg1, const Vector3T  &arg2 )
	{
		x = arg1.y * arg2.z - arg1.z * arg2.y;
		y = arg1.x * arg2.z - arg1.z * arg2.x;
		z = arg1.x * arg2.y - arg1.y * arg1.x;
		return *this;
	}

	Vector3T &  scale( const value arg1 )
	{
		x *= arg1;
		y *= arg1;
		z *= arg1;
		return *this;
	}

	Vector3T & clamp(const value min, const value max)
	{
		if(x < min) x = min; else if(x > max) x = max;
		if(y < min) y = min; else if(y > max) y = max;
		if(z < min) z = min; else if(z > max) z = max;
		return *this;
	}


	Vector3T & clampLength( const value length )
	{
		if(length > 0.0f)
		{
			value sqrLength = squaredMagnitude();
			if(sqrLength > length * length)
			{
				value coef = length / std::sqrt(sqrLength);
				x *= coef;
				y *= coef;
				z *= coef;
			}
		}
		return *this;
	}

	Vector3T & lerp( const Vector3T & a, const Vector3T & b, value percent )
	{
		x = a.x + percent * (b.x - a.x);
		y = a.y + percent * (b.y - a.y);
		z = a.z + percent * (b.z - a.z);
		return *this;
	}

	value squaredMagnitude()
	{
		return x * x + y * y + z * z;
	}

	value magnitude()
	{
		return std::sqrt(x*x + y*y + z*z);
	}

	value    squaredDistance(const Vector3T & b)
	{
		return ((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) + (z-b.z)*(z-b.z));
	}

	value    distance(const Vector3T & b)
	{
		return std::sqrt((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) + (z-b.z)*(z-b.z));
	}

	Vector3T & setLength(const value length)
	{
		float NewLength = length / magnitude();
		x *= NewLength;
		y *= NewLength;
		z *= NewLength;
		return *this;
	}

	// Operators
	Vector3T   operator - ()
	{
		x = -x; y= -y; z = -z;
		return *this;
	}

	Vector3T   operator + ( const Vector3T & arg1) const  // Add add = this + arg1
	{
		return Vector3T(x + arg1.x, y + arg1.y, z + arg1.z);
	}

	Vector3T & operator +=( const Vector3T & arg1)  // Add this = this + arg1
	{
		x += arg1.x;
		y += arg1.y;
		z += arg1.z;
		return *this;
	}

	Vector3T   operator - ( const Vector3T & arg1) const
	{
		return Vector3T(x-arg1.x,y-arg1.y,z-arg1.z);
	}

	Vector3T & operator -=( const Vector3T & arg1)
	{
		x -= arg1.x;
		y -= arg1.y;
		z -= arg1.z;
		return *this;
	}

	Vector3T   operator * ( const value b ) const
	{
		return Vector3T(x * b, y * b, z * b);
	}

	Vector3T   operator / ( const value b) const
	{
		value c = 1.0f/b;
		return Vector3T(x*c,y*c,z*c);
	}

	Vector3T & operator *= ( const value b)
	{
		x *= b;
		y *= b;
		z *= b;
		return *this;
	}
	Vector3T & operator /= ( const value b)
	{
		value c = 1.0f / b;
		x *= c;
		y *= c;
		z *= c;
		return *this;
	}

	Vector3T   operator * ( const Vector3T & b) const
	{
		return Vector3T(x*b.x,y*b.y,z*b.z);
	}

	Vector3T   operator / ( const Vector3T & b) const
	{
		return Vector3T(x/b.x,y/b.y,z/b.z);
	}

	Vector3T & operator *= ( const Vector3T & b)
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		return *this;
	}

	Vector3T & operator /= ( const Vector3T & b)
	{
		x /= b.x;
		y /= b.y;
		z /= b.z;
		return *this;
	}

	Vector3T   operator ^ (const Vector3T & b) const
	{
		return Vector3T(y * b.z - z * b.y,
						x * b.z - z * b.x,
						x * b.y - y * b.x);
	}

	value    operator | (const Vector3T & b) const
	{
		return (x * b.x + y * b.y + z * b.z);
	}

	bool	  operator == ( const Vector3T & b ) const
	{
		if(x==b.x && y==b.y && z==b.z)
			return true;
		return false;
	}

	bool	  operator != ( const Vector3T & b ) const
	{
		if(x!=b.x && y!=b.y && z!=b.z)
			return true;
		return false;
	}
};

};