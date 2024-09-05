#pragma once

#include "concepts.hpp"

namespace math {

enum class Rect_Positions {
  RP_Top,
  RP_Right,
  RP_Bottom,
  RP_Left,
  RP_Inside,
  RP_Outside
};

// Parameters
// ValueT = Ideally it should be a C++ intrinsic ValueT that has a sign
// PointT = a class that has a constructor defined as PointT(x,y) and can
//			be used as PointT.x and PointT.y

template <concepts::IntegeralOrFloat ValueT, concepts::Point<ValueT> PointT>
class RectT {

public:
  union {
    ValueT left;
    ValueT x;
  };
  union {
    ValueT top;
    ValueT y;
  };
  union {
    ValueT right;
    ValueT x2;
  };
  union {
    ValueT bottom;
    ValueT y2;
  };

  RectT() {}
  RectT(ValueT l, ValueT t, ValueT r, ValueT b)
      : left(l), top(t), right(r), bottom(b) {}
  RectT(const PointT &center, ValueT width, ValueT height)
      : x(center.x - width / 2), y(center.y - height / 2),
        x2(center.x + width / 2), y2(center.y + height / 2) {}
  RectT(const RectT &r) : x(r.x), y(r.y), x2(r.x2), y2(r.y2) {}
  ~RectT() {}

  RectT(RectT &&) = default;

  RectT &operator=(const RectT &r) = default;
  RectT &operator=(RectT &&r) = default;

  RectT &set(ValueT l, ValueT t, ValueT r, ValueT b) {
    x = l;
    y = t;
    x2 = r;
    y2 = b;
    return *this;
  }

  RectT &set(const PointT &center, ValueT width, ValueT height) {
    x = center.x - width / 2;
    y = center.y - height / 2;
    x2 = center.x + width / 2;
    y2 = center.y + height / 2;
    return *this;
  }

  // Returns the width
  // width = this.x2 - this.x
  ValueT width() const { return (x2 - x); }

  // Returns the height
  // height = this.y2 - this.y
  ValueT height() const { return (y2 - y); }

  // The Z ValueT will be set to PointT::DefaultZ
  PointT center() const { return PointT((x2 - x) / 2, (y2 - y) / 2); }
  PointT topLeft() const { return PointT(x, y); }
  PointT bottomRight() const { return PointT(x2, y2); }

  // Intersection Functions

  // Note: the Z ValueT will be IGNORED!
  bool isInside(ValueT xp, ValueT yp, bool inclusive = false) const {
    if (inclusive) {
      if (xp >= x && xp <= x2 && yp >= y && yp <= y2)
        return true;
    } else if (xp > x && xp < x2 && yp > y && yp < y2)
      return true;
    return false;
  }

  bool isInside(const PointT &p, bool inclusive = false) const {
    if (inclusive) {
      if (p.x >= x && p.x <= x2 && p.y >= y && p.y <= y2)
        return true;
    } else if (p.x > x && p.x < x2 && p.y > y && p.y < y2)
      return true;
    return false;
  }

  // Checks if r is inside of this
  // this.x > r.x && r.x2 < this.x2
  // this.y > r.y && r.y2 < this.y2
  bool isInside(const RectT &r) const {
    // Todo: Fix this function
    if (isInside(r.topLeft()) && isInside(r.bottomRight()))
      return true;
    return false;
  }

  // Note: to check if a PointT is Outside the rect call if(!pointInside(x,y))
  Rect_Positions isPointOnBorder(ValueT xt, ValueT yt) const {
#define onLine(a, b, c, d, e, f)                                               \
  if (a == b && (c <= d && c >= e))                                            \
  return f
    // Check if it is on the left
    onLine(xt, x, yt, y, y2, Rect_Positions::RP_Left);
    onLine(xt, x2, yt, y, y2, Rect_Positions::RP_Right);
    onLine(yt, y, xt, x, x2, Rect_Positions::RP_Top);
    onLine(yt, y2, xt, x, x2, Rect_Positions::RP_Bottom);
    if (isInside(xt, yt))
      return Rect_Positions::RP_Inside;
    return Rect_Positions::RP_Outside;
  }

  Rect_Positions isPointOnBorder(const PointT &p) const {
    onLine(p.x, x, p.y, y, y2, Rect_Positions::RP_Left);
    onLine(p.x, x2, p.y, y, y2, Rect_Positions::RP_Right);
    onLine(p.y, y, p.x, x, x2, Rect_Positions::RP_Top);
    onLine(p.y, y2, p.x, x, x2, Rect_Positions::RP_Bottom);
    if (isInside(p.x, p.y))
      return Rect_Positions::RP_Inside;
    return Rect_Positions::RP_Outside;
#undef onLine
  }

  // Checks to see if the two rects are intersecting
  bool isIntersect(const RectT &r) const {
    if (x2 < r.x || x > r.x2 || y2 < r.y || y > r.y2)
      return false;
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
  RectT &inflate(ValueT xt, ValueT yt, ValueT x2t, ValueT y2t) {
    x -= xt;
    y -= yt;
    x2 += x2t;
    y2 += y2t;
    return *this;
  }

  RectT &inflate(const RectT &r) {
    x -= r.x;
    y -= r.y;
    x2 += r.x2;
    y2 += r.y2;
    return *this;
  }

  // Same algorythm as above except all by the same ValueT
  RectT &inflate(ValueT ValueT) {
    x -= ValueT;
    y -= ValueT;
    x2 += ValueT;
    y2 += ValueT;
    return *this;
  }

  // Note the following functions are equal to (inflate(-xt,-yt,-x2t,-y2t);
  RectT &deflate(ValueT xt, ValueT yt, ValueT x2t, ValueT y2t) {
    x += xt;
    y += yt;
    x2 -= x2t;
    y2 -= y2t;
    return *this;
  }

  RectT &deflate(ValueT ValueT) {
    x += ValueT;
    y += ValueT;
    x2 -= ValueT;
    y2 -= ValueT;
    return *this;
  }

  RectT &deflate(const RectT &r) {
    x += r.x;
    y += r.y;
    x2 -= r.x2;
    y2 -= r.y2;
    return *this;
  }

  // Scale it equally. Keeping the CENTER the same
  RectT &scale(ValueT amount) {
    set(center(), width() * amount, height() * amount);
    return *this;
  }

  // Scale it unequally keeping the CENTER the same
  RectT &scale(ValueT xAmount, ValueT yAmount) {
    set(center(), width() * xAmount / 2, height() * yAmount / 2);
    return *this;
  }

  // Add the two rects together
  // It effectively MOVES the rect over by r
  // rect.x  = this.x  + r.x;  rect.y  = this.y  + r.y;
  // rect.x2 = this.x2 + r.x2; rect.y2 = this.y2 + r.y2;
  RectT operator+(const RectT &r) const {
    return RectT(x + r.x, y + r.y, x2 + r.x2, y2 + r.y2);
  }

  // It moves this over by r
  // this.x  += r.x;  this.y  += r.y;
  // this.x2 += r.x2; this.y2 += t.y2;
  RectT &operator+=(const RectT &r) {
    x += r.x;
    y += r.y;
    x2 += r.x2;
    y2 += r.y2;
    return *this;
  }

  // Same as the + function just replace the + with a -
  // it moves the rect over by -r
  RectT operator-(const RectT &r) const {
    return RectT(x - r.x, y - r.y, x2 - r.x2, y2 - r.y2);
  }

  // It moves this over by -r
  RectT &operator-=(const RectT &r) {
    x -= r.x;
    y -= r.y;
    x2 -= r.x2;
    y2 -= r.y2;
    return *this;
  }

  // rect.x = this.x; rect.y = this.y;
  // rect.x2 = this.x2 * scaleAmount; rect.y2 = this.y2 * scaleAmount;
  RectT operator*(const ValueT scaleAmount) const {
    auto c = center();
    ValueT xdiff = width() * scaleAmount / 4;
    ValueT ydiff = height() * scaleAmount / 4;
    return RectT(c.x - xdiff, c.y - ydiff, c.x + xdiff, c.y + ydiff);
  }

  // this.right *= scaleAmount; this.bottom *= scaleAmount
  RectT &operator*=(const ValueT scaleAmount) { return scale(scaleAmount); }

  // Inflate by 1
  RectT &operator++() { return inflate(1); }

  // Return the preInflated rect and then inflate by 1
  RectT operator++(int) {
    RectT r(*this);
    inflate(1);
    return r;
  }

  // Deflate the rect by 1
  RectT &operator--() { return deflate(1); }

  // Return the pre-deflated rect and then deflate by 1
  RectT operator--(int) {
    RectT r(*this);
    deflate(1);
    return r;
  }

  // Comparison operators
  bool operator==(const RectT &r) const {
    if (x == r.x && y == r.y && x2 == r.x2 && y2 == r.y2)
      return true;
    return false;
  }

  bool operator!=(const RectT &r) const {
    if (x != r.x || y != r.y || x2 != r.x2 || y2 != r.y2)
      return true;
    return false;
  }

  // Return true if the two rects intersect
  bool operator|(const RectT &r) const { return isIntersect(r); }

  // Return true if r is inside of this
  // this ^ r is DIFFERENT than r ^ this
  bool operator^(const RectT &r) const { return isInside(r); }
};

}; // namespace math
