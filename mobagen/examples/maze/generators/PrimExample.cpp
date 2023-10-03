#include "PrimExample.h"
#include "../World.h"
#include "Random.h"

#define POS_IN_BOUNDS(world, pos) \
  (pos).x <= (w->GetSize() / 2) && (pos).x >= -(w->GetSize() / 2) && (pos).y <= (w->GetSize() / 2) && (pos).y >= -(w->GetSize() / 2)
#define VISITED_COLOR Color::Black
#define VISITABLE_COLOR Color::Pink
#define UNVISITED_COLOR Color::DarkGray

bool PrimExample::Step(World* w) {
  int sideOver2 = w->GetSize() / 2;

  if (toBeVisited.empty()) {
    auto initialPosition = Point2D(0, 0);

    for (Point2D p : getVisitables(w, initialPosition)) {
      toBeVisited.push_back(p);
    }
  } else {
    int index = rand() % toBeVisited.size();
    Point2D random = toBeVisited[index];
    toBeVisited[index] = toBeVisited[toBeVisited.size() - 1];  // Move last into new place
    toBeVisited.pop_back();
    if (POS_IN_BOUNDS(w, random)) {
      w->SetNodeColor(random, VISITED_COLOR);
      std::vector<Point2D> walls{};
      std::vector<Point2D> directions = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};
      for (Point2D dir : directions) {
        if (POS_IN_BOUNDS(w, random + dir)) {
          if (w->GetNodeColor(random + dir) == VISITED_COLOR) {
            walls.push_back(dir);
          }
        }
      }
      if (!walls.empty()) {
        Point2D chosenDir = walls[rand() % walls.size()];
        if (chosenDir == Point2D::LEFT) w->SetWest(random, false);
        if (chosenDir == Point2D::RIGHT) w->SetEast(random, false);
        if (chosenDir == Point2D::UP) w->SetNorth(random, false);
        if (chosenDir == Point2D::DOWN) w->SetSouth(random, false);
      }
    }

    for (Point2D p : getVisitables(w, random)) {
      toBeVisited.push_back(p);
    }
  }

  for (const Point2D point : toBeVisited) {
    w->SetNodeColor(point, VISITABLE_COLOR);
  }

  return true;
}
void PrimExample::Clear(World* world) {
  toBeVisited.clear();
  initialized = false;
}

std::vector<Point2D> PrimExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  if (POS_IN_BOUNDS(w, p + Point2D::UP) && w->GetNodeColor(p + Point2D::UP) == UNVISITED_COLOR) visitables.push_back(p + Point2D::UP);
  if (POS_IN_BOUNDS(w, p + Point2D::DOWN) && w->GetNodeColor(p + Point2D::DOWN) == UNVISITED_COLOR) visitables.push_back(p + Point2D::DOWN);
  if (POS_IN_BOUNDS(w, p + Point2D::LEFT) && w->GetNodeColor(p + Point2D::LEFT) == UNVISITED_COLOR) visitables.push_back(p + Point2D::LEFT);
  if (POS_IN_BOUNDS(w, p + Point2D::RIGHT) && w->GetNodeColor(p + Point2D::RIGHT) == UNVISITED_COLOR) visitables.push_back(p + Point2D::RIGHT);

  return visitables;
}

std::vector<Point2D> PrimExample::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  return neighbors;
}
