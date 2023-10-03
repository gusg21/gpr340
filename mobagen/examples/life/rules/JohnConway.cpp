#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  for (int x = 0; x < world.SideSize(); x++)
    for (int y = 0; y < world.SideSize(); y++) {
      Point2D me = Point2D(x, y);
      int alive = world.Get(Point2D(x, y));
      int neighborCount = CountNeighbors(world, Point2D(x, y));

      if (alive) {
        if (neighborCount < 2)
          world.SetNext(me, false);
        else if (neighborCount < 4)
          world.SetNext(me, true);
        else
          world.SetNext(me, false);
      } else {
        if (neighborCount == 3 || neighborCount == 4)
          world.SetNext(me, true);
        else
          world.SetNext(me, false);
      }
    }

  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  int count = 0;

  count += world.Get(point + Point2D(-1, -1)) ? 1 : 0;
  count += world.Get(point + Point2D(0, -1)) ? 1 : 0;
  count += world.Get(point + Point2D(1, -1)) ? 1 : 0;
  count += world.Get(point + Point2D(-1, 0)) ? 1 : 0;
  count += world.Get(point + Point2D(1, 0)) ? 1 : 0;
  count += world.Get(point + Point2D(-1, 1)) ? 1 : 0;
  count += world.Get(point + Point2D(0, 1)) ? 1 : 0;
  count += world.Get(point + Point2D(1, 1)) ? 1 : 0;

  return count;
}
