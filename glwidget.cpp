#include "glwidget.h"
#include "objects.h" // draw platonic solids
#include "vector.h"
#include <QDebug>

GLfloat light_position[4] = {0.0, 100.0, 100.0, 0.0};

// Some colours defined in rgb alpha space
GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat cyan[] = {0.0f, 1.0f, 1.0f, 1.0f};
GLfloat magenta[] = {1.0f, 0.0f, 1.0f, 1.0f};
GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  arcBall = new ArcBall();
}

void GLWidget::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  if (aspectRatio > 1.0) {
    glOrtho(-zoom, zoom, -zoom / aspectRatio, zoom / aspectRatio, -10, 10);
  } else {
    glOrtho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom, -10, 10);
  }
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::initializeGL() {
  // Set up the view
  setProjection();

  // Turn on the lights
  // start with a unit light vector from upper right front
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
}

void GLWidget::paintGL() {

  // Set up the visualization pipeline
  // set the colour of the window to light gray
  glClearColor(0.8, 0.8, 0.8, 1.0);
  // and clear the window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Apply the arcball state to the gl matrix
  GLfloat mvMatrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mvMatrix);
  arcBall->getMatrix(mvMatrix);
  glLoadMatrixf(mvMatrix);

  // Test your 3D models here
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glLineWidth(8);
  switch (modelIndex) {
  case 0:
    glRotatef(65, 0.0, 1.0, 1.0);
    glScalef(0.38, 0.38, 0.38);
    // give object a colour and fill
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    drawTetrahedron();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawTetrahedron();
    break;
  case 1:
    glRotatef(45, -1.0, -1.0, -1.0);
    glScalef(0.38, 0.38, 0.38);
    // give object a colour
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    drawCube();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawCube();
    break;
  case 2:
    glRotatef(65, 1.0, 1.0, 1.0);
    glScalef(0.68, 0.68, 0.68);
    // give object a colour
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    drawOctahedron();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawOctahedron();
    break;
  case 3:
    glRotatef(45, 1.0, 1.0, 1.0);
    // give object a colour
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
    drawCylinder(0.5, 1, 15);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawCylinder(0.5, 1, 15);
    break;
  case 4:
    glRotatef(45, -1.0, -1.0, -1.0);
    // give object a colour
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, magenta);
    drawSphere(0.68, 15, 15);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawSphere(0.68, 15, 15);
    break;
  }

  glPopMatrix();
  glFlush();
}

void GLWidget::resizeGL(int w, int h) {
  width = w;
  height = h;
  glViewport(0, 0, width, height); // set the viewport
  setProjection();
  this->updateGL();
}

// select model to be displayed
void GLWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Q:
    qApp->exit();
    break;
  case Qt::Key_Plus:
    modelIndex = (modelIndex + 1) % 5;
    this->updateGL();
    break;
  case Qt::Key_Minus:
    modelIndex = (modelIndex + 4) % 5;
    this->updateGL();
    break;
  default:
    event->ignore();
    break;
  }
}

// Use mouse wheel to zoom in or out
void GLWidget::wheelEvent(QWheelEvent *event) {
  float numDegrees = (static_cast<float>(event->delta()) / 8.0);
  float numSteps = numDegrees / (180.0 * (1.0 / zoom));
  zoom -= numSteps;
  if (zoom <= 0.0) {
    zoom = 0.0001;
  }
  setProjection();
  this->updateGL();
}

// Use mouse drag to rotate view
void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    arcBall->startBall(event->x() * devicePixelRatio(),
                       event->y() * devicePixelRatio(), width, height);
    isDragging = true;
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    if (isDragging == true) {
      arcBall->updateBall(event->x() * devicePixelRatio(),
                          event->y() * devicePixelRatio(), width, height);
      this->updateGL();
    }
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isDragging = false;
    this->updateGL();
  }
}

void GLWidget::resetArcballAndZoom() {
  zoom = 1.0;
  arcBall->reset();
  setProjection();
  this->updateGL();
}

GLWidget::~GLWidget() {
  // Auto-generated destructor stub
}
