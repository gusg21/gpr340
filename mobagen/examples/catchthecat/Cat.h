#ifndef CAT_H
#define CAT_H

#include "Agent.h"

enum CatDirection {
  NE, NW, W, SW, SE, E
};

class Cat : public Agent {
public:
  explicit Cat() : Agent(){};
  Point2D Move(World*) override;

  static Point2D GetPointFromDirection(World* world, Point2D pos, CatDirection direction);
  static CatDirection RotateLeft(CatDirection direction);
  static CatDirection RotateRight(CatDirection direction);

private:
  CatDirection m_direction = E;
};

#endif  // CAT_H
