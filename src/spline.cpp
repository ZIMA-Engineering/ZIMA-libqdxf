/****************************************************************************
**
** This file is based on rs_spline.cpp, a part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "spline.h"
#include <QPointF>
#include <QList>
#include <iostream>

Spline::Spline(const DRW_Spline *spline) : mSpline (spline)
{

}

/**
 * Updates the internal polygon of this spline. Called when the
 * spline or it's data, position, .. changes.
 */
void Spline::update(QList<QLineF> &lines) {

    //std::cout << "RS_Spline::update" << std::endl;

    //clear();

    /*if (isUndone()) {
        return;
    }*/
    mClosed = ((mSpline->flags&0x1)==0x1);

    if (mSpline->degree<1 || mSpline->degree>3) {
        std::cout << "RS_Spline::update: invalid degree: " << mSpline->degree << std::endl;
        return;
    }

    if (mSpline->controllist.size() < (unsigned int) mSpline->degree+1) {
        std::cout << "RS_Spline::update: not enough control points" << std::endl;
        return;
    }

    //resetBorders();

    QList<QPointF> tControlPoints;
    for(unsigned int i=0; i < mSpline->controllist.size(); i++)
    {
        tControlPoints.append( QPointF(mSpline->controllist.at(i)->x, mSpline->controllist.at(i)->y) );
    }
    //QList<QPointF> tControlPoints = mSpline.controllist;

    if (mClosed) {
        for (int i=0; i<mSpline->degree; ++i) {
            tControlPoints.append(QPointF(mSpline->controllist.at(i)->x, mSpline->controllist.at(i)->y));
        }
    }

    int i;
    int npts = tControlPoints.count();
    // order:
    int k = mSpline->degree+1;
    // resolution:
    //int p1 = getGraphicVariableInt("$SPLINESEGS", 8) * npts;
    int p1 = 8 * npts;

    double* b = new double[npts*3+1];
    double* h = new double[npts+1];
    double* p = new double[p1*3+1];

    i = 1;
    for (int it = 0; it < tControlPoints.size(); ++it) {
        b[i] = tControlPoints.at(it).x();
        b[i+1] = tControlPoints.at(it).y();
        b[i+2] = 0.0;

        //RS_DEBUG->print("RS_Spline::update: b[%d]: %f/%f", i, b[i], b[i+1]);
        i+=3;
    }

    // set all homogeneous weighting factors to 1.0
    for (i=1; i <= npts; i++) {
        h[i] = 1.0;
    }

    for (i = 1; i <= 3*p1; i++) {
        p[i] = 0.0;
    }

    if (mClosed) {
        rbsplinu(npts,k,p1,b,h,p);
    } else {
        rbspline(npts,k,p1,b,h,p);
    }

    QPointF prev;
    for (i = 1; i <= 3*p1; i=i+3) {
        if (!prev.isNull()) {
            /*RS_Line* line = new RS_Line(this,
                                        RS_LineData(prev, RS_Vector(p[i], p[i+1])));
            line->setLayer(NULL);
            line->setPen(RS_Pen(RS2::FlagInvalid));
            addEntity(line);*/
            //std::cout << "spline: draw line" << std::endl;

            lines.append( QLineF(prev, QPointF(p[i], p[i+1]) ) );
            //painter->drawLine(prev, QPointF(p[i], p[i+1]));
        }
        prev = QPointF(p[i], p[i+1]);

        //minV = RS_Vector::minimum(prev, minV);
        //maxV = RS_Vector::maximum(prev, maxV);
    }

    delete[] b;
    delete[] h;
    delete[] p;
}


/**
 * Generates a rational B-spline curve using a uniform open knot vector.
 */
void Spline::rbspline(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    //int x[30]; /* allows for 20 data points with basis function of order 5 */
    int nplusc;

    double step;
    double t;
    //double nbasis[20];
//    double temp;

    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    // zero and redimension the knot vector and the basis array

    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    // generate the uniform open knot vector
    knot(npts,k,x);

    icount = 0;

    // calculate the points on the rational B-spline curve
    t = 0;
    step = ((double)x[nplusc])/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        // generate the basis function for this value of t
        rbasis(k,t,npts,x,h,nbasis);

        // generate a point on the curve
        for (j = 1; j <= 3; j++) {
            jcount = j;
            p[icount+j] = 0.;

            // Do local matrix multiplication
            for (i = 1; i <= npts; i++) {
//                temp = nbasis[i]*b[jcount];
//                p[icount + j] = p[icount + j] + temp;
                p[icount + j] +=  nbasis[i]*b[jcount];
                jcount = jcount + 3;
            }
        }
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}


void Spline::knotu(int num, int order, int knotVector[]) {
    int nplusc,/*nplus2,*/i;

    nplusc = num + order;
//    nplus2 = num + 2;

    knotVector[1] = 0;
    for (i = 2; i <= nplusc; i++) {
        knotVector[i] = i-1;
    }
}



void Spline::rbsplinu(int npts, int k, int p1,
                         double b[], double h[], double p[]) {

    int i,j,icount,jcount;
    int i1;
    //int x[30];		/* allows for 20 data points with basis function of order 5 */
    int nplusc;

    double step;
    double t;
    //double nbasis[20];
//    double temp;


    nplusc = npts + k;

    int* x = new int[nplusc+1];
    double* nbasis = new double[npts+1];

    /*  zero and redimension the knot vector and the basis array */

    for(i = 0; i <= npts; i++) {
        nbasis[i] = 0.0;
    }

    for(i = 0; i <= nplusc; i++) {
        x[i] = 0;
    }

    /* generate the uniform periodic knot vector */

    knotu(npts,k,x);

    /*
        printf("The knot vector is ");
        for (i = 1; i <= nplusc; i++){
                printf(" %d ", x[i]);
        }
        printf("\n");

        printf("The usable parameter range is ");
        for (i = k; i <= npts+1; i++){
                printf(" %d ", x[i]);
        }
        printf("\n");
    */

    icount = 0;

    /*    calculate the points on the rational B-spline curve */

    t = k-1;
    step = ((double)((npts)-(k-1)))/((double)(p1-1));

    for (i1 = 1; i1<= p1; i1++) {

        if ((double)x[nplusc] - t < 5e-6) {
            t = (double)x[nplusc];
        }

        rbasis(k,t,npts,x,h,nbasis);      /* generate the basis function for this value of t */
        /*
                        printf("t = %f \n",t);
                        printf("nbasis = ");
                        for (i = 1; i <= npts; i++){
                                printf("%f  ",nbasis[i]);
                        }
                        printf("\n");
        */
        for (j = 1; j <= 3; j++) {      /* generate a point on the curve */
            jcount = j;
            p[icount+j] = 0.;

            for (i = 1; i <= npts; i++) { /* Do local matrix multiplication */
//                temp = nbasis[i]*b[jcount];
//                p[icount + j] = p[icount + j] + temp;
                p[icount + j] += nbasis[i]*b[jcount];
                /*
                                                printf("jcount,nbasis,b,nbasis*b,p = %d %f %f %f %f\n",jcount,nbasis[i],b[jcount],temp,p[icount+j]);
                */
                jcount = jcount + 3;
            }
        }
        /*
                        printf("icount, p %d %f %f %f \n",icount,p[icount+1],p[icount+2],p[icount+3]);
        */
        icount = icount + 3;
        t = t + step;
    }

    delete[] x;
    delete[] nbasis;
}

/**
 * Generates rational B-spline basis functions for an open knot vector.
 */
void Spline::rbasis(int c, double t, int npts,
                       int x[], double h[], double r[]) {

    int nplusc;
    int i,k;
    double d,e;
    double sum;
    //double temp[36];

    nplusc = npts + c;

    double* temp = new double[nplusc+1];

    // calculate the first order nonrational basis functions n[i]
    for (i = 1; i<= nplusc-1; i++) {
        if (( t >= x[i]) && (t < x[i+1]))
            temp[i] = 1;
        else
            temp[i] = 0;
    }

    /* calculate the higher order nonrational basis functions */

    for (k = 2; k <= c; k++) {
        for (i = 1; i <= nplusc-k; i++) {
            // if the lower order basis function is zero skip the calculation
            if (temp[i] != 0)
                d = ((t-x[i])*temp[i])/(x[i+k-1]-x[i]);
            else
                d = 0;
            // if the lower order basis function is zero skip the calculation
            if (temp[i+1] != 0)
                e = ((x[i+k]-t)*temp[i+1])/(x[i+k]-x[i+1]);
            else
                e = 0;

            temp[i] = d + e;
        }
    }

    // pick up last point
    if (t == (double)x[nplusc]) {
        temp[npts] = 1;
    }

    // calculate sum for denominator of rational basis functions
    sum = 0.;
    for (i = 1; i <= npts; i++) {
        sum = sum + temp[i]*h[i];
    }

    // form rational basis functions and put in r vector
    for (i = 1; i <= npts; i++) {
        if (sum != 0) {
            r[i] = (temp[i]*h[i])/(sum);
        } else
            r[i] = 0;
    }

    delete[] temp;
}

/**
 * Generates B-Spline open knot vector with multiplicity
 * equal to the order at the ends.
 */
void Spline::knot(int num, int order, int knotVector[]) {
    knotVector[1] = 0;
    for (int i = 2; i <= num + order; i++) {
        if ( (i > order) && (i < num + 2) ) {
            knotVector[i] = knotVector[i-1] + 1;
        } else {
            knotVector[i] = knotVector[i-1];
        }
    }
}
