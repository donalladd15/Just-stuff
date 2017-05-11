#include "glwidget.h"
#include "arcBall.h"
#include "point.h"
#include "vector.h"
#include <QDebug>

const float GLWidget::PI = 3.14159265359;

// the viewing area dimensions
int viewWidth;
int viewHeight;

// the zoom factor
double zoomFactor = 1.0;

float ortho = 1.0;
float orthoNear = -100.0;
float orthoFar = 100.0;

static GLfloat light_position[] = {0.0f, 100.0f, 100.0f, 0.0f};

// basic colours
static GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat grey[] = {0.5f, 0.5f, 0.5f, 1.0f};

// primary colours
static GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
static GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
static GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};

// secondary colours
static GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
static GLfloat magenta[] = {1.0f, 0.0f, 1.0f, 1.0f};
static GLfloat cyan[] = {0.0f, 1.0f, 1.0f, 1.0f};

// other colours
static GLfloat orange[] = {1.0f, 0.5f, 0.0f, 1.0f};
static GLfloat brown[] = {0.5f, 0.25f, 0.0f, 1.0f};
static GLfloat dkgreen[] = {0.0f, 0.5f, 0.0f, 1.0f};
static GLfloat pink[] = {1.0f, 0.6f, 0.6f, 1.0f};

bool drag;
bool pull;
float pullX = 0.0f;
float pullY = 0.0f;
float moveX = 0.0f;
float moveY = 0.0f;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  resetArcBall = false;
  axes = true;
  grid = true;
  wframe = false;
  animation = false;
  nGridlines = 3;
  arc = new arcBall();
  objects = new Objects(this);
  viewWidth = this->width();
  viewHeight = this->height();

  time = 0;
  nframes = 50;

  posn_x = 0.0f, posn_y = 1.5f, posn_z = 0.0f;
  rotationAngle = 0.0f;
  circleRadius = 2.0f;

  this->animate(time);

  QTimer *timer = new QTimer(this);
  timer->start(0);
  connect(timer, SIGNAL(timeout()), this, SLOT(idle()));
}

void GLWidget::loadTextures() {
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  // load a texture
  glGenTextures(4, textures);
  if (textures[0] == 0)
    qDebug() << "invalid texture names";

  QImage deathstar;
  deathstar.load(QDir::homePath() + "/Desktop/texture.jpg");
  QImage glFormattedImage = QGLWidget::convertToGLFormat(deathstar);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 1 would not load";
  }

  QImage comic;
  comic.load(QDir::homePath() + "/Desktop/comics.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(comic);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 2 would not load";
  }

  QImage eyeball;
  eyeball.load(QDir::homePath() + "/Desktop/eye.jpg");
  glFormattedImage = QGLWidget::convertToGLFormat(eyeball);
  if (!glFormattedImage.isNull()) {
    qDebug() << "Texture read correctly...";
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    // tell OpenGL which texture you want to use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glFormattedImage.width(),
                 glFormattedImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 glFormattedImage.bits());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  } else {
    qDebug() << "Image 3 would not load";
  }

  //       QImage something;
  //       something.load("something.jpg");
  //       glFormattedImage = QGLWidget::convertToGLFormat(something);
  //       if(!glFormattedImage.isNull()){
  //           qDebug() << "Texture read correctly...";
  //           glBindTexture(GL_TEXTURE_2D, textures[0]);
  //           // tell OpenGL which texture you want to use
  //           glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
  //           glFormattedImage.width(),glFormattedImage.height(), 0, GL_RGBA,
  //           GL_UNSIGNED_BYTE, glFormattedImage.bits());
  //           glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  //           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
  //           GL_LINEAR);
  //           glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
  //           GL_LINEAR);
  //           glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  //       }else{
  //           qDebug() << "Image 4 would not load";
  //       }

  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::setOrtho() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspectRatio =
      static_cast<float>(viewWidth) / static_cast<float>(viewHeight);
  if (aspectRatio > 1.0) {
    glOrtho(-ortho, ortho, -ortho / aspectRatio, ortho / aspectRatio, -100,
            100);
  } else {
    glOrtho(-ortho * aspectRatio, ortho * aspectRatio, -ortho, ortho, -100,
            100);
  }
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::initializeGL() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  setOrtho();
  glShadeModel(GL_SMOOTH); // use flat lighting
  glLightfv(GL_LIGHT0, GL_POSITION,
            light_position); // specify the position of the light
  glEnable(GL_LIGHT0);       // switch light #0 on
  glEnable(GL_LIGHTING);     // switch lighting on
  glEnable(GL_DEPTH_TEST);   // make sure depth buffer is switched on
  glEnable(GL_NORMALIZE);    // normalize normal vectors for safety
  loadTextures();
}

void GLWidget::paintGL() {
  // clear all pixels
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat mNow[16];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glPushMatrix();
  glTranslatef(pullX, pullY, 0.0);
  glGetFloatv(GL_MODELVIEW_MATRIX, mNow);
  arc->getMatrix(mNow);
  glLoadMatrixf(mNow);

  if (axes) {
    drawAxes();
  }
  if (grid) {
    drawGrid(magenta, nGridlines);
  }

  // draw a sphere to display a deadly moon
  glPushMatrix();
  glTranslatef(-5.0f, 5.0f, 0.0f);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(3.0f, 64, 64);
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // draw a quad to display a comic book
  glPushMatrix();
  glTranslatef(0.0f, 2.0f, -2.0f);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  // draw the all seeing eye
  glPushMatrix();
  glTranslatef(2.0f, 5.0f, 0.0f);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glEnable(GL_TEXTURE_2D);
  objects->drawSphere(0.5f, 64, 64);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  if (wframe) {             // switch for wireframe, this is a hack
    glDisable(GL_LIGHTING); // disable lighting
    glColor4fv(red);        // set a consistant colour for the model
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // switch to line mode
    glPushMatrix();
    drawHuman(); // draw the model
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,
                  GL_FILL); // fill triangles for the rest of rendering
    glEnable(GL_LIGHTING);  // enable lighting for the rest of the rendering
  } else {
    // now do the drawing
    glPushMatrix();
    drawHuman();
    glPopMatrix();
  }
}

void GLWidget::resizeGL(int w, int h) {
  viewWidth = w;
  viewHeight = h;
  glViewport(0, 0, viewWidth, viewHeight);
  setOrtho();
  this->updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {

  case Qt::Key_1:
    axes = !axes;
    this->updateGL();
    break;
  case Qt::Key_2:
    grid = !grid;
    this->updateGL();
    break;
  case Qt::Key_3:
    wframe = !wframe;
    this->updateGL();
    break;
  case Qt::Key_4:
    animation = !animation;
    this->updateGL();
    break;
  case '[':
    if (nGridlines > 1)
      nGridlines--;
    this->updateGL();
    break;
  case ']':
    nGridlines++;
    this->updateGL();
    break;
  // case Qt::Key_T: this->dumpTexture(); break;

  case Qt::Key_Q:
    qApp->exit();
    break;
  default:
    event->ignore();
    break;
  }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  float numDegrees = (static_cast<float>(event->delta()) / 8.0);
  float numSteps = numDegrees / (180.0 * (1.0 / ortho));
  ortho -= numSteps;
  if (ortho <= 0.0) {
    ortho = 0.0001;
  }
  viewWidth = this->width();
  viewHeight = this->height();
  setOrtho();
  this->updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    arc->startBall(event->x(), event->y(), this->width(), this->height());
    drag = true;
  }
  if (event->button() == Qt::RightButton) {
    moveX = static_cast<float>(event->x());
    moveY = static_cast<float>(event->y());
    pull = true;
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    if (drag == true) {
      arc->updateBall(event->x(), event->y(), this->width(), this->height());
      this->updateGL();
    }
  }
  if (pull == true) {
    viewHeight = this->height();
    viewWidth = this->width();
    float aspectRatio =
        static_cast<float>(viewWidth) / static_cast<float>(viewHeight);
    if (aspectRatio < 1.0) {
      pullX += (static_cast<float>(event->x()) - moveX) /
               (static_cast<float>(this->width()) /
                ((ortho * 2.0) * aspectRatio)); //	set the vector
      pullY -= (static_cast<float>(event->y()) - moveY) /
               (static_cast<float>(this->height()) / (ortho * 2.0));
    } else {
      pullX += (static_cast<float>(event->x()) - moveX) /
               (static_cast<float>(this->width()) /
                (ortho * 2.0)); //	set the vector
      pullY -=
          (static_cast<float>(event->y()) - moveY) /
          (static_cast<float>(this->height()) / ((ortho * 2.0) / aspectRatio));
    }
    moveX = static_cast<float>(event->x());
    moveY = static_cast<float>(event->y());
    this->updateGL();
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    drag = false;
    this->updateGL();
  }
  if (event->button() == Qt::RightButton) {
    pull = false;
    this->updateGL();
  }
}

void GLWidget::resetArc() {
  resetArcBall = !resetArcBall;
  arc->reset();
  this->updateGL();
}

void GLWidget::resetPos() {
  pullX = 0.0;
  pullY = 0.0;
  this->updateGL();
}

void GLWidget::resetZoom() {
  viewWidth = this->width();
  viewHeight = this->height();
  ortho = zoomFactor;
  setOrtho();
  this->updateGL();
}

void GLWidget::drawAxes() {
  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPushMatrix();             // push a matrix on the stack

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey); // set colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grey); // but they do emit

  glBegin(GL_LINES); // start drawing lines
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0); // x-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0); // y-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0); // z-axis
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
               black); // set reflective colour

  glPushMatrix();                 // push a matrix on the stack
  glTranslatef(0.0, 0.0, 1.0);    // move to end of z axis
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the z-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in z=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "Z");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(0.0, 1.0, 0.0); // move to end of y axis
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the y-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in y=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "Y");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(1.0, 0.0, 0.0); // move to end of x axis
  glRotatef(90.0, 0.0, 1.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the x-axis
  glRotatef(90.0, 0.0, 1.0, 0.0); // rotate to draw in x=0 plane
  glTranslatef(0.0, 0.1, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  renderText(0, 0, 0, "X");
  glPopMatrix(); // pop matrix off stack

  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPopMatrix();              // pop matrix off stack
} // drawAxes()

// function to draw grid on z = 0 plane
void GLWidget::drawGrid(GLfloat *colour, int nGridlines) {
  int x, z;
  // edges don't reflect
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colour); // but they do emit
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  for (x = -nGridlines; x <= nGridlines; x++) { // for each x
    if (x % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(x, 0, -nGridlines);
    glVertex3i(x, 0, +nGridlines);
    glEnd();
  }                                             // for each x
  for (z = -nGridlines; z <= nGridlines; z++) { // for each y
    if (z % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(-nGridlines, 0, z);
    glVertex3i(+nGridlines, 0, z);
    glEnd();
  } // for each y
  glLineWidth(1.0);
  glPopMatrix();
  // stop emitting, otherwise other objects will emit the same colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
} // drawGrid()

void GLWidget::solidCone(GLdouble base, GLdouble height, GLint slices,
                         GLint stacks) {
  glBegin(GL_LINE_LOOP);
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluCylinder(quadric, base, 0, height, slices, stacks);

  gluDeleteQuadric(quadric);
  glEnd();
}

void GLWidget::idle() {
  if (animation == true) {
    if (time < nframes)
      time++;
    else
      time = 0;
    animate(time);
  }
  this->updateGL();
}

void GLWidget::drawHuman() { // drawHuman()

  /**
      TO DO: draw an articultated human character, you can use this
      member function or model the human using as a class.

      Criteria:

      The human model you create must have :

      1. two arms with elbow joints
      2. two legs with knee joints
      3. a body comprised of at least a pelvis and torso
      4. a neck

      You may also create as many other characters for your scene as you wish
  */

  /* Example code to get you started */

  // read more about the OpenGL function calles in the red book

  glPushMatrix(); //origin
  glTranslatef(posn_x, posn_y, posn_z);
  glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

  // pelvis
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  glPushMatrix(); // pelvis
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.15f, 0.35f, 32);
  glPopMatrix();  // pelvis

  // chest
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // pelvis
  glTranslatef(0.0f, 0.5f, 0.0f);
  glPushMatrix(); // chest
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.15f, 0.5f, 32);
  glPopMatrix();  // chest

  // neck
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // chest
  glTranslatef(0.0f, 0.1f, 0.0f);
  glPushMatrix(); // neck
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.075f, 0.1f, 32);
  glPopMatrix();  // neck

  // head
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // neck
  glTranslatef(0.0f, 0.15f, 0.0f);
  objects->drawSphere(0.2f, 32, 32);

  glPopMatrix(); // neck

  glPopMatrix(); // chest

  // left shoulder
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // chest
  glTranslatef(-0.25f, 0.0f, 0.0f);
  objects->drawSphere(0.12f, 32, 32);

  // left upper arm
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // left shoulder
  glRotatef(90.0f + left_shoulder_angle, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // left shoulder

  // left elbow
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // left shoulder
  glRotatef(left_shoulder_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  // left forearm
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // left elbow
  glRotatef(left_elbow_angle, 1.0f, 0.0f, 0.0f);
  glPushMatrix(); // left forearm
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // left forearm

  // left hand
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  glPopMatrix(); // left elbow

  glPopMatrix(); // left shoulder

  glPopMatrix(); // chest

  // right shoulder
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // chest
  glTranslatef(0.25f, 0.0f, 0.0f);
  objects->drawSphere(0.12f, 32, 32);

  // right upper arm
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // right shoulder
  glRotatef(90.0f + right_shoulder_angle, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // right shoulder

  // right elbow
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // right shoulder
  glRotatef(right_shoulder_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  // right forearm
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // right elbow
  glRotatef(right_elbow_angle, 1.0f, 0.0f, 0.0f);
  glPushMatrix(); // right forearm
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // right forearm

  // right hand
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  glPopMatrix(); // right elbow

  glPopMatrix(); // right shoulder

  glPopMatrix(); // chest

  glPopMatrix(); // pelvis

  // pelvis joint
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  glPushMatrix(); // pelvis
  glTranslatef(0.0f, -0.35f, 0.0f);
  objects->drawSphere(0.15f, 32, 32);

  // left thigh
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // pelvis joint
  glTranslatef(-0.08f, 0.0f, 0.0f);
  glPushMatrix(); // left thigh
  glRotatef(90.0f + left_thigh_angle, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // left thigh

  // left knee
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // left thigh
  glRotatef(left_thigh_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  // left shin
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // left knee
  glRotatef(left_knee_angle, 1.0f, 0.0f, 0.0f);
  glPushMatrix(); // left shin
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix(); // left shin

  // left foot
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glTranslatef(0.0f, -0.4f, -0.06f);
  objects->drawCylinder(0.06f, 0.3f, 32);

  glPopMatrix();  // left knee

  glPopMatrix();  // left thigh

  glPopMatrix();  // pelvis joint

  // right thigh
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // pelvis joint
  glTranslatef(0.08f, 0.0f, 0.0f);
  glPushMatrix(); // right thigh
  glRotatef(90.0f + right_thigh_angle, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix();  // right thigh

  // right knee
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glPushMatrix(); // right thigh
  glRotatef(right_thigh_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -0.4f, 0.0f);
  objects->drawSphere(0.08f, 32, 32);

  // right shin
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
  glPushMatrix(); // right knee
  glRotatef(right_knee_angle, 1.0f, 0.0f, 0.0f);
  glPushMatrix(); // right shin
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  objects->drawCylinder(0.06f, 0.4f, 32);
  glPopMatrix(); // right shin

  // right foot
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
  glTranslatef(0.0f, -0.4f, -0.06f);
  objects->drawCylinder(0.06f, 0.3f, 32);

  glPopMatrix();  // right knee

  glPopMatrix();  // right thigh

  glPopMatrix();  // pelvis joint

  glPopMatrix();  // pelvis

  glPopMatrix();  // origin

} // drawHuman()

void GLWidget::animate(int time) {
  /* animate() */
  /**
      TO DO : implement animation
  */

  static int direction = -1;
  if (time == 0) direction *= - 1;

  left_shoulder_angle = -direction * (45.0f - 90.0f * ((float)time / nframes));
  right_shoulder_angle = direction * (45.0f - 90.0f * ((float)time / nframes));

  left_elbow_angle = -90.0f;
  right_elbow_angle = -90.0f;

  left_thigh_angle = direction * (60.0f - 120.0f * ((float)time / nframes));
  right_thigh_angle = -direction * (60.0f - 120.0f * ((float)time / nframes));

  left_knee_angle = 45.0f;
  right_knee_angle = 45.0f;

  rotationAngle++;
  if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

  posn_x = -cos(rotationAngle * (PI / 180.0f)) * circleRadius;
  posn_z = sin(rotationAngle * (PI / 180.0f)) * circleRadius;

} /* animate() */

void GLWidget::saveImage() {
  QImage image = this->grabFrameBuffer();
  image.save("your_image.jpeg", "jpeg");
  if (!image.isNull()) {
    qDebug() << "Image saved successfully";
  }
}

GLWidget::~GLWidget() {
  delete objects;
  delete arc;
}
