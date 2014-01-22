#ifndef SPLINE_H
#define SPLINE_H
#include "../libdxfrw/src/drw_entities.h"
#include <QPainter>

class Spline
{
public:
    Spline(const DRW_Spline *spline);
    void update(QList<QLineF> &lines);

private:
    void rbspline(int npts, int k, int p1, double b[], double h[], double p[]);
    void knotu(int num, int order, int knotVector[]);
    void rbsplinu(int npts, int k, int p1, double b[], double h[], double p[]);
    void rbasis(int c, double t, int npts, int x[], double h[], double r[]);
    void knot(int num, int order, int knotVector[]);

    const DRW_Spline *mSpline;
    bool mClosed;
};

#endif // SPLINE_H
