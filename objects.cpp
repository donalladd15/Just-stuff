#include "objects.h"
#include "point.h"
#include "vector.h"
#include <cmath>

const double pi = 3.1416;

void drawTetrahedron() {
  /*
   * TODO
   * Draw a tetrahedron centered at the origin using GL_TRIANGLES
   * Compute and set normal vectors for each triangle to ensure proper shading
   */
    Point vertices[4] = {Point(-1.0, -1.0, -1.0), Point(-1.0, 1.0, 1.0),
                         Point(1.0, -1.0, 1.0), Point(1.0, 1.0, -1.0)};
    int faces[4][3] = {{0, 2, 1}, {1, 3, 0}, {2, 3, 1}, {3, 2, 0}};
    glBegin(GL_TRIANGLES);
    for(int face = 0; face < 4; face++){
        Vector u = vertices[faces[face][1]] - vertices[faces[face][0]];
        Vector v = vertices[faces[face][2]] - vertices[faces[face][0]];
        Vector n = u.cross(v);
        n = n.normalised();

        glNormal3f(n.dx, n.dy, n.dz);
        glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y, vertices[faces[face][0]].z);
        glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y, vertices[faces[face][1]].z);
        glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y, vertices[faces[face][2]].z);
    }
    glEnd();
}

void drawCube() {
  /*
   * TODO
   * Draw a cube centered at the origin using GL_QUADS
   * Compute and set normal vectors for each rectangle to ensure proper shading
   * Set texture coordinates
   */
    Point vertices[8] = {Point(-1.0, -1.0, -1.0), Point(-1.0, -1.0, 1.0),
                          Point(-1.0, 1.0, -1.0),  Point(-1.0, 1.0, 1.0),
                          Point(1.0, -1.0, -1.0),  Point(1.0, -1.0, 1.0),
                          Point(1.0, 1.0, -1.0),   Point(1.0, 1.0, 1.0)};

    int faces[6][4] = {{0, 4, 5, 1}, {0, 2, 6, 4}, {0, 1, 3, 2},
                        {4, 6, 7, 5}, {1, 5, 7, 3}, {2, 3, 7, 6}};
    glBegin(GL_QUADS);
    for(int face = 0; face < 6; face++){
        Vector u = vertices[faces[face][1]] - vertices[faces[face][0]];
        Vector v = vertices[faces[face][2]] - vertices[faces[face][0]];
        Vector n = u.cross(v);
        n = n.normalised();

        glNormal3f(n.dx, n.dy, n.dz);
        glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y, vertices[faces[face][0]].z);
        glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y, vertices[faces[face][1]].z);
        glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y, vertices[faces[face][2]].z);
        glVertex3f(vertices[faces[face][3]].x, vertices[faces[face][3]].y, vertices[faces[face][3]].z);
    }
    glEnd();
}

void drawOctahedron() {
  /*
   * TODO
   * Draw an octahedron centered at the origin using GL_TRIANGLES
   * Compute and set normal vectors for each triangle to ensure proper shading
   */
    Point vertices[6] = {Point(-1.0, 0.0, 0.0), Point(0.0, -1.0, 0.0),
                           Point(0.0, 0.0, -1.0), Point(1.0, 0.0, 0.0),
                           Point(0.0, 1.0, 0.0),  Point(0.0, 0.0, 1.0)};

    int faces[8][3] = {{1, 5, 0}, {1, 3, 5}, {3, 4, 5}, {4, 0, 5},
                         {2, 0, 4}, {3, 2, 4}, {1, 2, 3}, {1, 0, 2}};
    glBegin(GL_TRIANGLES);

    for(int face = 0; face < 8; face++){
        Vector u = vertices[faces[face][1]] - vertices[faces[face][0]];
        Vector v = vertices[faces[face][2]] - vertices[faces[face][0]];
        Vector n = u.cross(v);
        n = n.normalised();

        glNormal3f(n.dx, n.dy, n.dz);
        glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y, vertices[faces[face][0]].z);
        glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y, vertices[faces[face][1]].z);
        glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y, vertices[faces[face][2]].z);
    }
    glEnd();
}

void drawCylinder(float radius, float height, int nSegments) {
  /*
   * TODO
   * Draw cylinder centered at the origin using GL_TRIANGLES
   * Compute and set normal vectors for each vertex to ensure smooth shading
   */
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < nSegments; i++){
        float angle = pi * i * 2 / nSegments;
        float nextAngle = pi * (i + 1) * 2 / nSegments;
        float y1 = radius * sin(angle);
        float x1 = radius * cos(angle);
        float y2 = radius * sin(nextAngle);
        float x2 = radius * cos(nextAngle);

        glNormal3f(x1, y1, 0.0f);glVertex3f(x1, y1, -(height/2.0f));
        glNormal3f(x1, y1, 1.0f);glVertex3f(x1, y1, (height/2.0f));
        glNormal3f(x2, y2, 1.0f);glVertex3f(x2, y2, (height/2.0f));//triangles in front

        glNormal3f(x1, y1, 0.0f);glVertex3f(x1, y1, -(height/2.0f));
        glNormal3f(x2, y2, 0.0f);glVertex3f(x2, y2, -(height/2.0f));
        glNormal3f(x2, y2, 1.0f);glVertex3f(x2, y2, (height/2.0f));//triangles at the back

        glNormal3f(0.0f, 0.0f, -1.0f);glVertex3f(x1, y1, (height/2.0f));
        glNormal3f(0.0f, 0.0f, -1.0f);glVertex3f(x2, y2, (height/2.0f));
        glNormal3f(0.0f, 0.0f, -1.0f);glVertex3f(0.0f, 0.0f, (height/2.0f));//circle in front

        glNormal3f(1.0f, 1.0f, 1.0f);glVertex3f(x1, y1, -(height/2.0f));
        glNormal3f(1.0f, 1.0f, 1.0f);glVertex3f(x2, y2, -(height/2.0f));
        glNormal3f(1.0f, 1.0f, 1.0f);glVertex3f(0.0f, 0.0f, -(height/2.0f));//circle at the back

    }
    glEnd();

}

void drawSphere(float radius, int nSegments, int nSlices) {
  /*
   * TODO
   * Draw sphere centered at the origin using GL_QUADS
   * Compute and set normal vectors for each vertex to ensure proper shading
   * Set texture coordinates
   */
    float x, y, z;
    float s, t; // texture coordinates

    float inctheta = (2.0 * pi) / float(nSlices);
    float incphi = pi / float(nSegments);

    glBegin(GL_QUADS);

    // loop from -pi to +pi where step would be inctheta
    // loop from -pi/2 to +pi/2 step is incphi
    for(float theta = -pi; theta < pi; theta+=inctheta){
        for(float phi = (-pi/2.0f); phi < (pi/2.0f); phi+=incphi){
            //calculate below for where a could be phi or phi + incphi and b could be theta
            //and (theta + inctheta)
            float a, b;

            a = phi;
            b = theta;

            x = cos(a) * cos(b) * radius;
            y = cos(a) * sin(b) * radius;
            z = sin(a) * radius;
            t = (a / pi) + 0.5f;
            s = (b / (pi * 2.0f)) + 0.5f;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);

            a = phi + incphi;
            b = theta;

            x = cos(a) * cos(b) * radius;
            y = cos(a) * sin(b) * radius;
            z = sin(a) * radius;
            t = (a / pi) + 0.5f;
            s = (b / (pi * 2.0f)) + 0.5f;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);

            a = phi + incphi;
            b = theta + inctheta;

            x = cos(a) * cos(b) * radius;
            y = cos(a) * sin(b) * radius;
            z = sin(a) * radius;
            t = (a / pi) + 0.5f;
            s = (b / (pi * 2.0f)) + 0.5f;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);

            a = phi;
            b = theta + inctheta;

            x = cos(a) * cos(b) * radius;
            y = cos(a) * sin(b) * radius;
            z = sin(a) * radius;
            t = (a / pi) + 0.5f;
            s = (b / (pi * 2.0f)) + 0.5f;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}
