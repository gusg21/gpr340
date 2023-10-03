#include "BoundedAreaRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f BoundedAreaRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Return a force proportional to the proximity of the boids with the bounds, and opposed to it
  Vector2f force = Vector2f::zero();  // zero

  auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
  auto windowSize = this->world->engine->window->size();
  auto myPos = boid->getPosition();

  // Turn around if close to edge
  if (myPos.x < desiredDistance) {
    force.x = abs(force.x);
  } 
  if (myPos.x > windowSize.x - desiredDistance) {
    force.x = -abs(force.x);
  }
  if (myPos.y < desiredDistance) {
    force.y = abs(force.y);
  }
  if (myPos.y > windowSize.y - desiredDistance) {
    force.y = -abs(force.y);
  }

  // Recenter if out of bounds
  if (myPos.x < 0 || myPos.y < 0 || myPos.x > windowSize.x || myPos.y > windowSize.y) {
    auto center = Vector2f(windowSize.x / 2.f, windowSize.y / 2.f);
    force = (center - myPos).normalized();
  }

  return force.normalized();
}

bool BoundedAreaRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  auto size = this->world->engine->window->size();
  auto widthWindows = size.x;
  auto heightWindows = size.y;
  bool valueHasChanged = false;

  // We cap the max separation as the third of the min of the width.height
  auto minHeightWidth = std::min(widthWindows, heightWindows);

  if (ImGui::SliderInt("Desired Distance From Borders", &desiredDistance, 0.0f, (int)(minHeightWidth / 3), "%i")) {
    valueHasChanged = true;
  }

  return valueHasChanged;
}

void BoundedAreaRule::draw(const Boid& boid, SDL_Renderer* renderer) const {
  FlockingRule::draw(boid, renderer);
  auto size = this->world->engine->window->size();
  auto dist = (float)desiredDistance;

  // Draw a rectangle on the map
  Polygon::DrawLine(renderer, Vector2f(dist, dist), Vector2f(size.x - dist, dist), Color::Gray);                    // TOP
  Polygon::DrawLine(renderer, Vector2f(size.x - dist, dist), Vector2f(size.x - dist, size.y - dist), Color::Gray);  // RIGHT
  Polygon::DrawLine(renderer, Vector2f(size.x - dist, size.y - dist), Vector2f(dist, size.y - dist), Color::Gray);  // Bottom
  Polygon::DrawLine(renderer, Vector2f(dist, size.y - dist), Vector2f(dist, dist), Color::Gray);                    // LEFT
}