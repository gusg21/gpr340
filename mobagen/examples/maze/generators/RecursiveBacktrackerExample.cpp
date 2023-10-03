#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

#define POS_IN_BOUNDS(world, pos) \
  (pos).x <= (w->GetSize() / 2) && (pos).x >= -(w->GetSize() / 2) && (pos).y <= (w->GetSize() / 2) && (pos).y >= -(w->GetSize() / 2)
#define VISITED_COLOR Color::Black
#define VISITABLE_COLOR Color::Pink
#define UNVISITED_COLOR Color::DarkGray

bool RecursiveBacktrackerExample::Step(World* w) {
  // Return true if world is modified
  if (stack.empty() && !done) {
    Point2D start = randomStartPoint(w);
    stack.push_back(start);
    w->SetNodeColor(start, UNVISITED_COLOR);
  } else if (!done) {
    Point2D head = stack[stack.size() - 1];

    std::vector<Point2D> visitables = getVisitables(w, head);
    Point2D chosen;
    if (visitables.size() == 0) {
      do {
        chosen = stack[stack.size() - 1];
        w->SetNodeColor(chosen, VISITED_COLOR);
        stack.pop_back();
        if (stack.empty()) {
          done = true; 
          auto sideSize = w->GetSize();
          w->SetNodeColor(Point2D(-sideSize / 2, -sideSize / 2), VISITED_COLOR);
        } 
      } while (getVisitables(w, chosen).size() == 0 && stack.size() > 0);
    } else {
      chosen = visitables[rand() % visitables.size()];
    }
    
    if (POS_IN_BOUNDS(w, chosen)) {
      w->SetNodeColor(chosen, VISITABLE_COLOR);

      Point2D diff = chosen - head;
      if (diff == Point2D::LEFT) w->SetEast(chosen, false);
      if (diff == Point2D::RIGHT) w->SetWest(chosen, false);
      if (diff == Point2D::UP) w->SetSouth(chosen, false);
      if (diff == Point2D::DOWN) w->SetNorth(chosen, false);
      stack.push_back(chosen);
    }
  }

  return true;
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}
Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  if (POS_IN_BOUNDS(w, p + Point2D::UP) && w->GetNodeColor(p + Point2D::UP) == UNVISITED_COLOR) visitables.push_back(p + Point2D::UP);
  if (POS_IN_BOUNDS(w, p + Point2D::DOWN) && w->GetNodeColor(p + Point2D::DOWN) == UNVISITED_COLOR) visitables.push_back(p + Point2D::DOWN);
  if (POS_IN_BOUNDS(w, p + Point2D::LEFT) && w->GetNodeColor(p + Point2D::LEFT) == UNVISITED_COLOR) visitables.push_back(p + Point2D::LEFT);
  if (POS_IN_BOUNDS(w, p + Point2D::RIGHT) && w->GetNodeColor(p + Point2D::RIGHT) == UNVISITED_COLOR) visitables.push_back(p + Point2D::RIGHT);

  return visitables;
}
