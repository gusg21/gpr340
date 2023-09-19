#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to match the heading of neighbors = Average velocity
  Vector2f averageVelocity = Vector2f::zero();

  for (const Boid* boid : neighborhood)
  {
    averageVelocity += boid->getVelocity();
  }

  return Vector2f::normalized(averageVelocity);
}