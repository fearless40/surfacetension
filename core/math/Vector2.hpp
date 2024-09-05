#pragma once

#include <cmath>
#include "concepts.hpp"

namespace math
{
	
//template <class value, int dimension, basetype = impl::vector<dimension, value> >
template <concepts::IntegeralOrFloat ValueT>
class Vector2T 
{
public:
	// The following constructor is to allow stRectf to work 
	Vector2T(ValueT xt, ValueT yt) : x(xt), y(yt)		{}
	// Vector2T(int x1, int y1, int z1) : x(x1), y(y1), z(z1)  {}
	Vector2T( Vector2T & arg ) : x(arg.x), y(arg.y)   {}
	Vector2T()													{}
	union
	{
		struct
		{
			float x,y;	// x, y, and z
		};

		float v[2];
	};

	Vector2T &  set(const ValueT xt,const ValueT yt) {x = xt; y=yt;  return *this;} 
	Vector2T &  operator()(const ValueT xt, const ValueT yt) {x = xt; y=yt;  return *this;}

	ValueT dot( const Vector2T &arg2  ) const
	{
		return (x * arg2.x + y * arg2.y );
	}

	Vector2T &	normalize()
	{
		if (x != y != 0)
		{
			ValueT mag = x * x + y * y ;
			mag = 1 / std::sqrt(mag);
			x *= mag;
			y *= mag;
		}
		return *this;
	}

	Vector2T &  scale( const ValueT arg1 )
	{
		x *= arg1;
		y *= arg1;
		return *this;
	}

	Vector2T & clamp(const ValueT min, const ValueT max)
	{
		if(x < min) x = min; else if(x > max) x = max;
		if(y < min) y = min; else if(y > max) y = max;
		return *this;
	}


	Vector2T & clampLength( const ValueT length )
	{
		if(length > 0.0f)
		{
			ValueT sqrLength = squaredMagnitude();
			if(sqrLength > length * length)
			{
				ValueT coef = length / std::sqrt(sqrLength);
				x *= coef;
				y *= coef;
			}
		}
		return *this;
	}

	Vector2T & lerp( const Vector2T & a, const Vector2T & b, ValueT percent )
	{
		x = a.x + percent * (b.x - a.x);
		y = a.y + percent * (b.y - a.y);
		return *this;
	}

	ValueT squaredMagnitude()
	{
		return x * x + y * y ;
	}

	ValueT magnitude()
	{
		return std::sqrt(x*x + y*y);
	}

	ValueT    squaredDistance(const Vector2T & b)
	{
		return ((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) );
	}

	ValueT    distance(const Vector2T & b)
	{
		return std::sqrt((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) );
	}

	Vector2T & setLength(const ValueT length)
	{
		ValueT NewLength = length / magnitude();
		x *= NewLength;
		y *= NewLength;
		return *this;
	}

	// Operators
	Vector2T   operator - ()
	{
		x = -x; y= -y;
		return *this;
	}

	Vector2T   operator + ( const Vector2T & arg1) const  // Add add = this + arg1
	{
		return Vector2T(x + arg1.x, y + arg1.y);
	}

	Vector2T & operator +=( const Vector2T & arg1)  // Add this = this + arg1
	{
		x += arg1.x;
		y += arg1.y;
		return *this;
	}

	Vector2T   operator - ( const Vector2T & arg1) const
	{
		return Vector2T(x-arg1.x,y-arg1.y);
	}

	Vector2T & operator -=( const Vector2T & arg1)
	{
		x -= arg1.x;
		y -= arg1.y;
		return *this;
	}

	Vector2T   operator * ( const ValueT b ) const
	{
		return Vector2T(x * b, y * b);
	}

	Vector2T   operator / ( const ValueT b) const
	{
		ValueT c = 1/b;
		return Vector2T(x*c,y*c);
	}

	Vector2T & operator *= ( const ValueT b)
	{
		x *= b;
		y *= b;
		return *this;
	}
	Vector2T & operator /= ( const ValueT b)
	{
		ValueT c = 1.0f / b;
		x *= c;
		y *= c;
		return *this;
	}

	Vector2T   operator * ( const Vector2T & b) const
	{
		return Vector2T(x*b.x,y*b.y);
	}

	Vector2T   operator / ( const Vector2T & b) const
	{
		return Vector2T(x/b.x,y/b.y);
	}

	Vector2T & operator *= ( const Vector2T & b)
	{
		x *= b.x;
		y *= b.y;
		return *this;
	}

	Vector2T & operator /= ( const Vector2T & b)
	{
		x /= b.x;
		y /= b.y;
		return *this;
	}

	ValueT    operator | (const Vector2T & b) const
	{
		return (x * b.x + y * b.y );
	}

	bool	  operator == ( const Vector2T & b ) const
	{
		if(x==b.x && y==b.y )
			return true;
		return false;
	}

	bool	  operator != ( const Vector2T & b ) const
	{
		if(x!=b.x && y!=b.y )
			return true;
		return false;
	}
};


};