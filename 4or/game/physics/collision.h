#pragma once
#include "shape.h"
#include "body.h"
#include "manifold.h"

struct Manifold;
struct Body;

typedef void(*collisionCallback)(Manifold* m, Body* a, Body* b);

extern collisionCallback dispatch[(int)Shape::Type::COUNT][(int)Shape::Type::COUNT];

void circletoCircle(Manifold* m, Body* a, Body* b);
void circletoPolygon(Manifold* m, Body* a, Body* b);
void polygontoCircle(Manifold* m, Body* a, Body* b);
void polygontoPolygon(Manifold* m, Body* a, Body* b);