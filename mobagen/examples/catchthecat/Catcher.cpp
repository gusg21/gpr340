#include "Catcher.h"
#include "World.h"
#include "Cat.h"
#include <unordered_map>
#include <queue>
//
// Point2D Catcher::Move(World* world) {
//  auto side = world->getWorldSideSize();
//  auto sideOver2 = world->getWorldSideSize() / 2;
//  return Point2D(rand() % side - sideOver2, rand() % side - sideOver2);
//}

Point2D Catcher::Move(World* world) {
  auto goal = world->getCat();

  // GENERATE DISTANCE MAP
  float sideSizeOver2 = world->getWorldSideSize() / 2;
  const int _INFINITY = 10000;  // I'm a realist

  // distance grid
  auto distanceMap = std::unordered_map<Point2D, int>();

  for (int x = -sideSizeOver2; x <= sideSizeOver2; x++) {
    for (int y = -sideSizeOver2; y <= sideSizeOver2; y++) {
      auto point = Point2D(x, y);
      distanceMap[point] = _INFINITY;

      if (world->getContent(point)) distanceMap[point] = -1;
      if (point == goal) distanceMap[point] = 0;
    }
  }

  auto pointQueue = std::queue<Point2D>();
  pointQueue.push(goal);

  bool catHasEscape = false;

  while (!pointQueue.empty()) {
    auto head = pointQueue.front();
    auto myDistance = distanceMap[head];
    pointQueue.pop();

    auto neighbors = world->neighbors(head);
    for (Point2D neighbor : neighbors) {
      if (world->isValidPosition(neighbor)) {
        int currentNeighborDist = distanceMap[neighbor];
        if (currentNeighborDist == -1) {  // Wall
          continue;
        }

        int weight = 1;  // TODO: experiment!
        int potentialNeighborDist = myDistance + weight;
        if (potentialNeighborDist < currentNeighborDist) {
          distanceMap[neighbor] = potentialNeighborDist;
          pointQueue.push(neighbor);
        }
      } else {  // found an invalid point, i.e. the edge of the map
        catHasEscape = true;
      }
    }
  }

  if (!catHasEscape) {
    // Pick a random point in the bounds if the cat is trapped
    auto randomPoint = Point2D(-_INFINITY, -_INFINITY);
    int iterationCap = 20;
    while (!world->isValidPosition(randomPoint) || world->getContent(randomPoint)) {
      switch (rand() % 6) {
        case 0:
          randomPoint = world->NE(goal);
          break;
        case 1:
          randomPoint = world->E(goal);
          break;
        case 2:
          randomPoint = world->SE(goal);
          break;
        case 3:
          randomPoint = world->SW(goal);
          break;
        case 4:
          randomPoint = world->W(goal);
          break;
        case 5:
          randomPoint = world->NW(goal);
          break;
      }

      iterationCap--;
      if (iterationCap <= 0) break;
    }
    
    return randomPoint;
  }

  /*for (auto pair : distanceMap) {
    printf("%d,%d: %d\n", pair.first.x, pair.first.y, pair.second);
  }*/

  // FIND CLOSEST EDGE POINT
  Point2D INVALID_LOCATION = Point2D(-_INFINITY, -_INFINITY);
  int bestDistance = _INFINITY;
  auto bestLocation = INVALID_LOCATION;

  // Top + bottom edge
  for (int x = -sideSizeOver2; x <= sideSizeOver2; x++) {
    // Top
    auto point = Point2D(x, -sideSizeOver2);
    if (world->isValidPosition(point)) {
      int pointDistance = distanceMap[point];
      if (pointDistance == -1) continue;         // Wall
      if (pointDistance == _INFINITY) continue;  // Inaccessible

      if (pointDistance < bestDistance) {
        bestDistance = pointDistance;
        bestLocation = point;
      }
    }

    // Bottom
    point = Point2D(x, sideSizeOver2);
    if (world->isValidPosition(point)) {
      int pointDistance = distanceMap[point];
      if (pointDistance == -1) continue;         // Wall
      if (pointDistance == _INFINITY) continue;  // Inaccessible

      if (pointDistance < bestDistance) {
        bestDistance = pointDistance;
        bestLocation = point;
      }
    }
  }

  // Left + right edge
  for (int y = -sideSizeOver2; y <= sideSizeOver2; y++) {
    // Left
    auto point = Point2D(-sideSizeOver2, y);
    if (world->isValidPosition(point)) {
      int pointDistance = distanceMap[point];
      if (pointDistance == -1) continue;         // Wall
      if (pointDistance == _INFINITY) continue;  // Inaccessible

      if (pointDistance < bestDistance) {
        bestDistance = pointDistance;
        bestLocation = point;
      }
    }

    // Right
    point = Point2D(sideSizeOver2, y);
    if (world->isValidPosition(point)) {
      int pointDistance = distanceMap[point];
      if (pointDistance == -1) continue;         // Wall
      if (pointDistance == _INFINITY) continue;  // Inaccessible

      if (pointDistance < bestDistance) {
        bestDistance = pointDistance;
        bestLocation = point;
      }
    }
  }

  if (bestLocation == INVALID_LOCATION) {  // Nowhere to go!
    auto side = world->getWorldSideSize();
    auto sideOver2 = world->getWorldSideSize() / 2;
    return Point2D(rand() % side - sideOver2, rand() % side - sideOver2);
  }

  // DESCEND DISTANCE MAP
  printf("Descending distance map...\n");
  Point2D head = bestLocation;
  printf("Edge point: %d,%d\n", head.x, head.y);
  Point2D bestGoalNeighbor = INVALID_LOCATION;
  int iterationCap = 50;  // Max path to search
  while (head != goal && iterationCap > 0) {
    iterationCap--;
    int currentDistance = distanceMap[head];

    auto neighbors = world->neighbors(head);
    auto bestNeighborDistance = currentDistance;
    auto bestNeighbor = INVALID_LOCATION;
    printf("Current distance @ %d,%d: %d\n", head.x, head.y, currentDistance);
    for (auto neighbor : neighbors) {
      if (neighbor == goal) {  // We're about to go to the cat
        printf("Pathed to cat!\n");
        bestGoalNeighbor = head;
      }

      int neighborDistance = distanceMap[neighbor];

      if (world->isValidPosition(neighbor) && neighborDistance >= 0) {  // neighbor is valid and not a wall
        printf("Neighbor @ %d,%d: %d\n", neighbor.x, neighbor.y, neighborDistance);
        if (neighborDistance < bestNeighborDistance) {
          bestNeighborDistance = neighborDistance;
          bestNeighbor = neighbor;
        }
      }
    }

    head = bestNeighbor;

    printf("Head now @ %d,%d\n", head.x, head.y);
  }

  if (bestGoalNeighbor == INVALID_LOCATION) {  // Unable to path properly!
    auto side = world->getWorldSideSize();
    auto sideOver2 = world->getWorldSideSize() / 2;
    return Point2D(rand() % side - sideOver2, rand() % side - sideOver2);
  } else {
    // We found a good point!
    printf("Best goal neighbor @ %d,%d\n", bestGoalNeighbor.x, bestGoalNeighbor.y);
    return bestGoalNeighbor;
  }
}