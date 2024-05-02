#include "Triangulation.h"

void Triangulation::addPoint(float x, float y, float z) {
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.i = vertices.size();
    vertices.push_back(v);
}

int vec3Compare(const void* vector1, const void* vector2) {
    vec3* p1 = (vec3*)vector1;
    vec3* p2 = (vec3*)vector2;
    if (p1->x < p2->x)
        return(-1);
    else if (p1->x > p2->x)
        return(1);
    else
        return(0);
}

void Triangulation::triangule() {

    if (vertices.size() < 3) {
        return;
    }

    int nv = vertices.size();

    vector<vec3> verticesTemp = vertices;
    qsort(&verticesTemp[0], verticesTemp.size(), sizeof(vec3), vec3Compare);

    vector<vec3> verticesvec3;

    for (int i = 0; i < nv; i++) {
        vec3 v;
        v.x = verticesTemp.at(i).x;
        v.y = verticesTemp.at(i).y;
        v.z = verticesTemp.at(i).z;
        verticesvec3.push_back(v);
    }

    verticesvec3.push_back(vec3());
    verticesvec3.push_back(vec3());
    verticesvec3.push_back(vec3());

    int nbTri = 3 * nv;
    triangles.resize(nbTri);

    triangulate(nv, &verticesvec3[0], &triangles[0], nbTri);

    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    for (int i = 0; i < nv; i++) {
        mesh.addVertex(ofVec3f(vertices[i].x, vertices[i].y, vertices[i].z));
    }

    for (int i = 0; i < nbTri; i++) {
        mesh.addIndex(verticesTemp.at(triangles[i].v1).i);
        mesh.addIndex(verticesTemp.at(triangles[i].v2).i);
        mesh.addIndex(verticesTemp.at(triangles[i].v3).i);
    }
}

int Triangulation::triangulate(int countPoints, vec3 tab[], triangle v[], int& count)
{
    int* complete = NULL;
    arete* aretes = NULL;
    arete* areteTmp;
    int nedge = 0;
    int trimax, emax = 200;
    int status = 0;

    int inside;
    int i, j, k;
    double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
    double xmin, xmax, ymin, ymax, xmid, ymid;
    double dx, dy, dmax;

    trimax = 4 * countPoints;
    complete = new int[trimax];
    aretes = new arete[emax];

    xmin = tab[0].x;
    ymin = tab[0].y;
    xmax = xmin;
    ymax = ymin;
    for (i = 1; i < countPoints; i++) {
        if (tab[i].x < xmin) xmin = tab[i].x;
        if (tab[i].x > xmax) xmax = tab[i].x;
        if (tab[i].y < ymin) ymin = tab[i].y;
        if (tab[i].y > ymax) ymax = tab[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;
    xmid = (xmax + xmin) / 2.0;
    ymid = (ymax + ymin) / 2.0;

    tab[countPoints + 0].x = xmid - 20 * dmax;
    tab[countPoints + 0].y = ymid - dmax;
    tab[countPoints + 0].z = 0.0;
    tab[countPoints + 1].x = xmid;
    tab[countPoints + 1].y = ymid + 20 * dmax;
    tab[countPoints + 1].z = 0.0;
    tab[countPoints + 2].x = xmid + 20 * dmax;
    tab[countPoints + 2].y = ymid - dmax;
    tab[countPoints + 0].z = 0.0;
    v[0].v1 = countPoints;
    v[0].v2 = countPoints + 1;
    v[0].v3 = countPoints + 2;
    complete[0] = false;
    count = 1;


    for (i = 0; i < countPoints; i++) {
        xp = tab[i].x;
        yp = tab[i].y;
        nedge = 0;
        for (j = 0; j < count; j++) {
            if (complete[j])
                continue;
            x1 = tab[v[j].v1].x;
            y1 = tab[v[j].v1].y;
            x2 = tab[v[j].v2].x;
            y2 = tab[v[j].v2].y;
            x3 = tab[v[j].v3].x;
            y3 = tab[v[j].v3].y;
            inside = calculCircumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);

            if (xc < xp && ((xp - xc) * (xp - xc)) > r)
                complete[j] = true;

            if (inside) {
                if (nedge + 3 >= emax) {
                    emax += 100;
                    areteTmp = new arete[emax];
                    for (int i = 0; i < nedge; i++) {
                        areteTmp[i] = aretes[i];
                    }
                    delete[]aretes;
                    aretes = areteTmp;
                }

                aretes[nedge + 0].v1 = v[j].v1;
                aretes[nedge + 0].v2 = v[j].v2;
                aretes[nedge + 1].v1 = v[j].v2;
                aretes[nedge + 1].v2 = v[j].v3;
                aretes[nedge + 2].v1 = v[j].v3;
                aretes[nedge + 2].v2 = v[j].v1;
                nedge += 3;
                v[j] = v[count - 1];
                complete[j] = complete[count - 1];
                count--;
                j--;
            }
        }

        for (j = 0; j < nedge - 1; j++) {
            for (k = j + 1; k < nedge; k++) {
                if ((aretes[j].v1 == aretes[k].v2) && (aretes[j].v2 == aretes[k].v1)) {
                    aretes[j].v1 = -1;
                    aretes[j].v2 = -1;
                    aretes[k].v1 = -1;
                    aretes[k].v2 = -1;
                }
                if ((aretes[j].v1 == aretes[k].v1) && (aretes[j].v2 == aretes[k].v2)) {
                    aretes[j].v1 = -1;
                    aretes[j].v2 = -1;
                    aretes[k].v1 = -1;
                    aretes[k].v2 = -1;
                }
            }
        }

        for (j = 0; j < nedge; j++) {
            if (aretes[j].v1 < 0 || aretes[j].v2 < 0)
                continue;

            if (count >= trimax) {
                break;
            }

            v[count].v1 = aretes[j].v1;
            v[count].v2 = aretes[j].v2;
            v[count].v3 = i;
            complete[count] = false;
            count++;
        }

        if (count >= trimax) {
            break;
        }
    }

    for (i = 0; i < count; i++) {
        if (v[i].v1 >= countPoints || v[i].v2 >= countPoints || v[i].v3 >= countPoints) {
            v[i] = v[count - 1];
            count--;
            i--;
        }
    }
    delete[] aretes;
    delete[] complete;
    return 0;
}

int Triangulation::calculCircumCircle(double xp, double yp, double x1, double y1, double x2,
    double y2, double x3, double y3, double& centerX, double& centerY, double& rayon) {
    double m1, m2, mx1, mx2, my1, my2;
    double dx, dy, rsqr, drsqr;

    if (abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON)
        return(false);

    if (abs(y2 - y1) < EPSILON) {
        m2 = -(x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        centerX = (x2 + x1) / 2.0;
        centerY = m2 * (centerX - mx2) + my2;
    }
    else if (abs(y3 - y2) < EPSILON) {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        centerX = (x3 + x2) / 2.0;
        centerY = m1 * (centerX - mx1) + my1;
    }
    else {
        m1 = -(x2 - x1) / (y2 - y1);
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        centerX = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        centerY = m1 * (centerX - mx1) + my1;
    }

    dx = x2 - centerX;
    dy = y2 - centerY;
    rsqr = dx * dx + dy * dy;
    rayon = sqrt(rsqr);
    dx = xp - centerX;
    dy = yp - centerY;
    drsqr = dx * dx + dy * dy;

    return((drsqr <= rsqr) ? true : false);
}
