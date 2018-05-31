#pragma once
#include "headerSpaghetti.h"


struct Manifold;
struct Body;

typedef void(*collisionCallback)(Manifold* m, Body* a, Body* b);

extern collisionCallback dispatch[Shape::COUNT][Shape::COUNT];

void circletoCircle(Manifold* m, Body* a, Body* b);
void circletoPolygon(Manifold* m, Body* a, Body* b);
void polygontoCircle(Manifold* m, Body* a, Body* b);
void polygontoPolygon(Manifold* m, Body* a, Body* b);