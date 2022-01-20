#ifndef __BEZIER_H__
#define __BEZIER_H__

#include <math.h>
#include "linkedlist.h"
#include <QPointF>

#define TIMES 1000
#define DOTS 6

int factorial(int);
double dist (QPointF, QPointF);
double bezierPoly (double, int, int); 
void bezierCurve (QPointF *, int, QPointF *, int);
void triangleDist (QPointF *, double *);

#endif
