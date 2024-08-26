#pragma once

namespace ST {


enum Rect_Positions
{
	RP_Top,
	RP_Right,
	RP_Bottom,
	RP_Left,
	RP_Inside,
	RP_Outside
};


// Parameters
// value = Ideally it should be a C++ intrinsic value that has a sign
// point = a class that has a constructor defined as point(x,y) and can 
//			be used as point.x and point.y

template <class value, class point>
class RectT
{
protected:
	typedef RectT<value,point> rect;
public:
	union { value left;		value	x;	};
	union { value top;		value	y;	};
	union { value right;	value	x2;	};
	union { value bottom;	value	y2;	};
	
	RectT()																				{}
	RectT(value l, value t, value r, value b) : left(l), top(t), 
		right(r), bottom (b)															{}
	RectT(const point & center, value width, value height) : x(center.x - width /2),
		y(center.y - height / 2), x2(center.x + width /2), y2(center.y + height / 2)	{}
	RectT(const rect & r) : x(r.x), y(r.y), x2(r.x2), y2(r.y2)							{} 
	~RectT()																			{}

	RectT(RectT &&) = default;

	RectT & operator = (const RectT & r) = default;
	RectT & operator = (RectT && r) = default;

	rect & set(value l, value t, value r, value b)
	{
		x  = l; y  = t;
		x2 = r; y2 = b;
		return *this;
	}

	rect & set(const point & center, value width, value height)
	{
		x  = center.x - width  / 2;
		y  = center.y - height / 2;
		x2 = center.x + width  / 2;
		y2 = center.y + height / 2;
		return *this;
	}

	// Returns the width
	// width = this.x2 - this.x
	value width() const { return (x2 - x); }
	
	// Returns the height
	// height = this.y2 - this.y
	value height() const { return (y2 - y); }

	// The Z value will be set to point::DefaultZ
	point center()		const	{ return point( (x2	  - x)/2,(y2 - y)/2 );	}
	point topLeft()		const	{ return point( x,y	  );					}
	point bottomRight() const	{ return point( x2,y2 );					}

// Intersection Functions

	// Note: the Z value will be IGNORED!
	bool isInside(value xp, value yp, bool inclusive = false) const
	{
		if(inclusive)
		{
			if( xp >= x && xp <= x2 && yp >= y && yp <= y2)
				return true;
		}
		else
			if(xp > x && xp < x2 && yp > y && yp < y2)
				return true;
		return false;
	}
	
	bool isInside(const point & p, bool inclusive = false) const
	{
		if(inclusive)
		{
			if( p.x >= x && p.x <= x2 && p.y >= y && p.y <= y2)
				return true;
		}
		else
			if(p.x > x && p.x < x2 && p.y > y && p.y < y2)
				return true;
		return false;
	}

	// Checks if r is inside of this
	// this.x > r.x && r.x2 < this.x2
	// this.y > r.y && r.y2 < this.y2
	bool isInside(const rect & r)						const
	{
		// Todo: Fix this function
		if( isInside(r.topLeft()) && isInside(r.bottomRight())) 
			return true;
		return false;

	}

	// Note: to check if a point is Outside the rect call if(!pointInside(x,y))
	Rect_Positions isPointOnBorder(value xt, value yt) const
	{
#define onLine(a,b,c,d,e,f) if(a == b && (c <= d && c >= e)) return f
		// Check if it is on the left
		onLine(xt,x,yt,y,y2,RP_Left);
		onLine(xt,x2,yt,y,y2,RP_Right);
		onLine(yt,y,xt,x,x2,RP_Top);
		onLine(yt,y2,xt,x,x2,RP_Bottom);
		if(isInside(xt,yt))
			return RP_Inside;
		return RP_Outside;
	}

	Rect_Positions isPointOnBorder(const point & p) const
	{
		onLine(p.x,x,p.y,y,y2,RP_Left);
		onLine(p.x,x2,p.y,y,y2,RP_Right);
		onLine(p.y,y,p.x,x,x2,RP_Top);
		onLine(p.y,y2,p.x,x,x2,RP_Bottom);
		if(isInside(p.x,p.y))
			return RP_Inside;
		return RP_Outside;
	#undef onLine
	}

	// Checks to see if the two rects are intersecting
	bool isIntersect(const rect & r) const
	{
		if( x2 < r.x  ||
			x  > r.x2 ||
			y2 < r.y  ||
			y  > r.y2 ) return false;
		return true;
	}

		
	
	// Makes the rect into the two intersecting parts
	// Think of it as a boolean union
	// |-++/::}
	// |-++/::}
	// |---|
	// return:  ++/
	//			++/
	// IF they are not intersecting than return this
	/*rect & getIntersect(const rect & r1, const rect & r2)
	{
		if(r1.isIntersect(r2))
		{
			

		}
		return *this;
	}
	*/

	// This functions inflates the rect by the following algorythim
	// left -= xt; top  -= yt
	// right += x2t; bottom += y2t;
 	rect & inflate(value xt, value yt, value x2t, value y2t)
	{
		x -= xt; y -= yt;
		x2 += x2t; y2 += y2t; 
		return *this;
	}

	rect & inflate(const rect & r)
	{
		x -= r.x; y -= r.y;
		x2 += r.x2; y2 += r.y2;
		return *this;
	}
	
	// Same algorythm as above except all by the same value
	rect & inflate(value value)
	{
		x -= value; y -= value;
		x2 += value; y2 += value;
		return *this;
	}

	// Note the following functions are equal to (inflate(-xt,-yt,-x2t,-y2t);
	rect & deflate(value xt, value yt, value x2t, value y2t)
	{
		x += xt; y += yt;
		x2 -= x2t; y2 -= y2t;
		return *this;
	}
	
	rect & deflate(value value)
	{
		x += value; y += value;
		x2 -= value; y2 -= value;
		return *this;
	}

	rect & deflate(const rect & r)
	{
		x += r.x; y += r.y;
		x2 -= r.x2; y2 -= r.y2;
		return *this;
	}

	// Scale it equally. Keeping the CENTER the same  
	rect & scale(value amount)
	{
		set(center(), width() * amount, height() * amount);
		return *this;
	}

	// Scale it unequally keeping the CENTER the same
	rect & scale(value xAmount, value yAmount)					
	{
		set(center(), width() * xAmount / 2, height() * yAmount / 2);
		return *this;
	}

	// Add the two rects together
	// It effectively MOVES the rect over by r
	// rect.x  = this.x  + r.x;  rect.y  = this.y  + r.y; 
	// rect.x2 = this.x2 + r.x2; rect.y2 = this.y2 + r.y2;
	rect operator + (const rect & r)							const
	{
		return rect( x + r.x,
					 y + r.y,
					 x2 + r.x2,
					 y2 + r.y2 );
	}

	// It moves this over by r
	// this.x  += r.x;  this.y  += r.y;
	// this.x2 += r.x2; this.y2 += t.y2;
	rect & operator += (const rect & r)
	{
		x += r.x;
		y += r.y;
		x2 += r.x2;
		y2 += r.y2;
		return *this;
	}

	// Same as the + function just replace the + with a -
	// it moves the rect over by -r
	rect operator - (const rect & r)	const
	{
		return rect( x - r.x,
					 y - r.y,
					 x2 - r.x2,
					 y2 - r.y2 );
	}

	
	// It moves this over by -r
	rect & operator -= (const rect & r)
	{
		x -= r.x;
		y -= r.y;
		x2 -= r.x2;
		y2 -= r.y2;
		return *this;
	}

	// rect.x = this.x; rect.y = this.y;
	// rect.x2 = this.x2 * scaleAmount; rect.y2 = this.y2 * scaleAmount;
	rect operator * (const value scaleAmount)					const
	{
		point c = center();
		value xdiff = width() * scaleAmount / 4;
		value ydiff = height() * scaleAmount / 4;
		return rect ( c.x - xdiff,
					  c.y - ydiff,
					  c.x + xdiff,
					  c.y + ydiff );
	}

	// this.right *= scaleAmount; this.bottom *= scaleAmount
	rect & operator *= ( const value scaleAmount)
	{
		return scale(scaleAmount);
	}

	// Inflate by 1
	rect & operator ++()
	{
		return inflate(1);
	}

	// Return the preInflated rect and then inflate by 1
	rect operator ++(int)
	{
		rect r(*this);
		inflate(1);
		return r;
	}

	// Deflate the rect by 1
	rect & operator --()
	{
		return deflate(1);
	}

	// Return the pre-deflated rect and then deflate by 1
	rect operator --(int)
	{
		rect r(*this);
		deflate(1);
		return r;
	}

	// Comparison operators
	bool operator == (const rect & r)						const
	{
		if( x == r.x && y == r.y && x2 == r.x2 && y2 == r.y2 )
			return true;
		return false;
	}

	bool operator != (const rect & r)						const
	{
		if( x!= r.x || y != r.y || x2 != r.x2 || y2 != r.y2)
			return true;
		return false;
	}

	// Return true if the two rects intersect
	bool operator | (const rect &r)						const
	{ 
		return isIntersect(r);
	}

	// Return true if r is inside of this
	// this ^ r is DIFFERENT than r ^ this
	bool operator ^ (const rect & r)						const
	{
		return isInside(r);
	}
};

};
