#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f cohesionForce;

  if (neighborhood.empty()) return Vector2f::zero();

  Vector2f positionSum = Vector2f();

  for (const Boid* boid : neighborhood) {
    positionSum += boid->getPosition();
  }

  Vector2f averagePosition = positionSum / neighborhood.size();
  cohesionForce = (averagePosition - boid->getPosition()).normalized();

  return cohesionForce.normalized();
}