//
// Created by goujs on 19-6-13.
//

#include <assert.h>
#include "vec2d.h"


namespace bz_robot
{
Vec2d Vec2d::CreateUnitVec2d(const float angle) {
  return Vec2d(cos(angle), sin(angle));
}

float Vec2d::Length() const { return hypot(x_, y_); }

float Vec2d::LengthSquare() const { return x_ * x_ + y_ * y_; }

float Vec2d::Angle() const { return std::atan2(y_, x_); }

void Vec2d::Normalize() {
  const float l = Length();
  if (l > kMathEpsilon) {
    x_ /= l;
    y_ /= l;
  }
}

float Vec2d::DistanceTo(const Vec2d &other) const {
  return hypot(x_ - other.x_, y_ - other.y_);
}

float Vec2d::DistanceSquareTo(const Vec2d &other) const {
  const float dx = x_ - other.x_;
  const float dy = y_ - other.y_;
  return dx * dx + dy * dy;
}

float Vec2d::CrossProd(const Vec2d &other) const {
  return x_ * other.y() - y_ * other.x();
}

float Vec2d::InnerProd(const Vec2d &other) const {
  return x_ * other.x() + y_ * other.y();
}

Vec2d Vec2d::rotate(const float angle) const {
  return Vec2d(x_ * cos(angle) - y_ * sin(angle),
               x_ * sin(angle) + y_ * cos(angle));
}

void Vec2d::SelfRotate(const float angle) {
  float tmp_x = x_;
  x_ = x_ * cos(angle) - y_ * sin(angle);
  y_ = tmp_x * sin(angle) + y_ * cos(angle);
}

Vec2d Vec2d::operator+(const Vec2d &other) const {
  return Vec2d(x_ + other.x(), y_ + other.y());
}

Vec2d Vec2d::operator-(const Vec2d &other) const {
  return Vec2d(x_ - other.x(), y_ - other.y());
}

Vec2d Vec2d::operator - () const {
  return Vec2d(-x_, -y_);
}

Vec2d Vec2d::operator*(const float ratio) const {
  return Vec2d(x_ * ratio, y_ * ratio);
}

Vec2d Vec2d::operator/(float ratio) const {
  //assert(std::abs(ratio) > kMathEpsilon);
  //assert(ratio != 0);
    if(ratio < kMathEpsilon && ratio > -kMathEpsilon)
    {
        ratio = kMathEpsilon;
    }
  return Vec2d(x_ / ratio, y_ / ratio);
}

Vec2d &Vec2d::operator+=(const Vec2d &other) {
  x_ += other.x();
  y_ += other.y();
  return *this;
}

Vec2d &Vec2d::operator-=(const Vec2d &other) {
  x_ -= other.x();
  y_ -= other.y();
  return *this;
}

Vec2d &Vec2d::operator*=(const float ratio) {
  x_ *= ratio;
  y_ *= ratio;
  return *this;
}

Vec2d &Vec2d::operator/=(const float ratio) {
  assert(std::abs(ratio) > kMathEpsilon);
  x_ /= ratio;
  y_ /= ratio;
  return *this;
}

bool Vec2d::operator==(const Vec2d &other) const {
  return (std::abs(x_ - other.x()) < kMathEpsilon &&
          std::abs(y_ - other.y()) < kMathEpsilon);
}

Vec2d operator*(const float ratio, const Vec2d &vec) {
  return vec * ratio;
}

Vec2d Vec2d::ort(Vec2d b) const {
  Vec2d a(this->x_, this->y_);
  Vec2d c;
  // multiply b by the dot product of this and b then divide it by b's length
  c = a - b * a.InnerProd(b) / b.LengthSquare();
  return c;
}
}
