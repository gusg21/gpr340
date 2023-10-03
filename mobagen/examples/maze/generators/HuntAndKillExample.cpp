#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>

#define POS_IN_BOUNDS(world, pos) \
  ((pos).x <= (w->GetSize() / 2) && (pos).x >= -(w->GetSize() / 2) && (pos).y <= (w->GetSize() / 2) && (pos).y >= -(w->GetSize() / 2))
#define VISITED_COLOR Color::Black
#define VISITABLE_COLOR Color::Pink
#define UNVISITED_COLOR Color::DarkGray

bool HuntAndKillExample::Step(World* w) {
  // Return true if world modified
  if (stack.empty() && !done) {
    auto start = randomStartPoint(w);
    stack.push_back(start);
    visited[start.y][start.x] = true;
    w->SetNodeColor(start, VISITED_COLOR);
  } else {
    auto head = stack[stack.size() - 1];
    // Go down path
    if (getVisitables(w, head).size() > 0) {
      std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
      Point2D delta;
      do {
        delta = deltas[rand() % 4];
      } while (!POS_IN_BOUNDS(w, head + delta));
      head += delta;
      w->SetNodeColor(head, VISITED_COLOR);
      visited[head.y][head.x] = true;
      stack.push_back(head);
    } else { // Find new head
      Point2D checkPoint = randomStartPoint(w);
      head = checkPoint;
      visited[checkPoint.y][checkPoint.x] = true;
      w->SetNodeColor(checkPoint, VISITED_COLOR);
    }
  }

  return true;
}
void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}
Point2D HuntAndKillExample::randomStartPoint(World* world) {
  // Todo: improve this if you want
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  if (POS_IN_BOUNDS(w, p + Point2D::UP) && !visited[p.x][p.y-1]) visitables.push_back(p + Point2D::UP);
  if (POS_IN_BOUNDS(w, p + Point2D::DOWN) && !visited[p.x][p.y + 1]) visitables.push_back(p + Point2D::DOWN);
  if (POS_IN_BOUNDS(w, p + Point2D::LEFT) && !visited[p.x-1][p.y]) visitables.push_back(p + Point2D::LEFT);
  if (POS_IN_BOUNDS(w, p + Point2D::RIGHT) && !visited[p.x+1][p.y]) visitables.push_back(p + Point2D::RIGHT);

  return visitables;
}

std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  return neighbors;
}
