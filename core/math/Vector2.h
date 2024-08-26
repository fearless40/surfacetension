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
class Vector2T 
{
public:
	// The following constructor is to allow stRectf to work 
	Vector2T(value xt, value yt) : x(xt), y(yt)		{}
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

	Vector2T &  set(const value xt,const value yt) {x = xt; y=yt;  return *this;} 
	Vector2T &  operator()(const value xt, const value yt) {x = xt; y=yt;  return *this;}

	value dot( const Vector2T &arg2  ) const
	{
		return (x * arg2.x + y * arg2.y );
	}

	Vector2T &	normalize()
	{
		if (x != y != 0)
		{
			value mag = x * x + y * y ;
			mag = 1 / std::sqrt(mag);
			x *= mag;
			y *= mag;
		}
		return *this;
	}

	Vector2T &  scale( const value arg1 )
	{
		x *= arg1;
		y *= arg1;
		return *this;
	}

	Vector2T & clamp(const value min, const value max)
	{
		if(x < min) x = min; else if(x > max) x = max;
		if(y < min) y = min; else if(y > max) y = max;
		return *this;
	}


	Vector2T & clampLength( const value length )
	{
		if(length > 0.0f)
		{
			value sqrLength = squaredMagnitude();
			if(sqrLength > length * length)
			{
				value coef = length / std::sqrt(sqrLength);
				x *= coef;
				y *= coef;
			}
		}
		return *this;
	}

	Vector2T & lerp( const Vector2T & a, const Vector2T & b, value percent )
	{
		x = a.x + percent * (b.x - a.x);
		y = a.y + percent * (b.y - a.y);
		return *this;
	}

	value squaredMagnitude()
	{
		return x * x + y * y ;
	}

	value magnitude()
	{
		return std::sqrt(x*x + y*y);
	}

	value    squaredDistance(const Vector2T & b)
	{
		return ((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) );
	}

	value    distance(const Vector2T & b)
	{
		return std::sqrt((x-b.x)*(x-b.x) + (y-b.y) * (y-b.y) );
	}

	Vector2T & setLength(const value length)
	{
		value NewLength = length / magnitude();
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

	Vector2T   operator * ( const value b ) const
	{
		return Vector2T(x * b, y * b);
	}

	Vector2T   operator / ( const value b) const
	{
		value c = 1/b;
		return Vector2T(x*c,y*c);
	}

	Vector2T & operator *= ( const value b)
	{
		x *= b;
		y *= b;
		return *this;
	}
	Vector2T & operator /= ( const value b)
	{
		value c = 1.0f / b;
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

	value    operator | (const Vector2T & b) const
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