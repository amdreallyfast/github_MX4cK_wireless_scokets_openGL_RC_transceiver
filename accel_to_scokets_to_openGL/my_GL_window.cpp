#include "my_GL_window.h"

#include "my_shape.h"
#include "shape_generator.h"
#include "my_shader_handler.h"

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

// for vector and matrix types
#include "my_glm_hpp_include.h"

// for matrix transformation stuff
#include <glm\glm\gtc\matrix_transform.hpp>

// for mouse stuff
#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>


// include the moc-generated source file for the the GL window's meta data, which 
// will allow the compiler to link up the things labeled as SIGNAL and SLOT
#include "my_GL_window_moc.cpp"


my_GL_window::~my_GL_window()
{
   // destroy all shapes that we kept around
   m_shape_ptrs_vector_it = m_shape_ptrs_vector.begin();
   while (m_shape_ptrs_vector_it != m_shape_ptrs_vector.end())
   {
      delete (*m_shape_ptrs_vector_it);

      m_shape_ptrs_vector_it += 1;
   }
}


void my_GL_window::initializeGL()
{
   int ret_val = 0;
   my_shape *new_shape_ptr = 0;

   // sets up all the openGL pointers
   glewInit();
   glEnable(GL_DEPTH_TEST);

   // only use if you are NOT tracking accelerometer data
   //setMouseTracking(true);

   // make a new shape, which will automatically have it's data sent to openGL
   // during the initialization process
   ret_val = shape_generator::make_pyramid(&new_shape_ptr);
   if (0 == ret_val)
   {
      m_shape_ptrs_vector.push_back(new_shape_ptr);
   }

   my_shader_handler& shader_thingy = my_shader_handler::get_instance();
   ret_val = shader_thingy.install_shaders();
   if (ret_val < 0)
   {
      // something didn't compile or link correctly (??do something??)
      cout << "something bad happened during shader initialization" << endl;
   }

   // make the timer go as fast as it can
   bool b = this->connect(&m_qt_timer, SIGNAL(timeout()), this, SLOT(timer_update()));
   cout << b << endl;
   m_qt_timer.start(0);
}


// do nothing
void my_GL_window::paintEvent()
{
}


void my_GL_window::paintGL()
{
   render_frame();
}


void my_GL_window::render_frame()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, this->width(), this->height());

   // field of view is 90 degrees
   float fov_radians = (1.0f / 2.0f) * 3.14159f;

   // aspect ratio is determined by window dimensions 
   // Note: The return values of width() and height() are integers, so they need to
   // be cast to floats so that there are no implicit conversions that result in 
   // lost data, such as an integer dividing by an integer, resulting in an integer,
   // which is then shoved into the float "aspect ratio".  That would be bad.
   float aspect_ratio = (float)this->width() / (float)this->height();

   // other things
   float near_plan_dist = 0.1f;
   float far_plane_dist = 10.0f;

   // create the projection matrix
   glm::mat4 projection_matrix = glm::perspective(
      fov_radians,
      aspect_ratio,
      near_plan_dist,
      far_plane_dist);

   glm::mat4 camera_world_to_view_matrix = m_camera.get_world_to_view_matrix();

   //m_shape_ptrs_vector[0]->draw_thineself(&projection_matrix, &camera_world_to_view_matrix);
   //m_shape_ptrs_vector[0]->draw_thineself();

   m_shape_ptrs_vector_it = m_shape_ptrs_vector.begin();
   while (m_shape_ptrs_vector_it != m_shape_ptrs_vector.end())
   {
      (*m_shape_ptrs_vector_it)->draw_thineself(
         &projection_matrix,
         &camera_world_to_view_matrix);
      m_shape_ptrs_vector_it += 1;
   }
}


static bool g_mouse_is_pressed;

void my_GL_window::mouseMoveEvent(QMouseEvent *e)
{
   // tell the shape to point itself at the mouse
   if (g_mouse_is_pressed)
   {
      m_shape_ptrs_vector[0]->point_thineself_mouse_update(glm::vec2(e->x(), e->y()), this->width(), this->height());
   }
   else
   {
      m_camera.mouse_update(glm::vec2(e->x(), e->y()));
   }
   

   //this->repaint();
}

void my_GL_window::mousePressEvent(QMouseEvent * e)
{
   g_mouse_is_pressed = true;

   m_shape_ptrs_vector[0]->reset_thineself();
   //this->repaint();
   cout << "mouse clicked" << endl;
}


void my_GL_window::mouseReleaseEvent(QMouseEvent * e)
{
   g_mouse_is_pressed = false;

   cout << "mouse released" << endl;
}

void my_GL_window::keyPressEvent(QKeyEvent* e)
{
   switch (e->key())
   {
   case Qt::Key::Key_W:
      m_camera.move_forward();
      break;
   case Qt::Key::Key_A:
      m_camera.strafe_left();
      break;
   case Qt::Key::Key_S:
      m_camera.move_back();
      break;
   case Qt::Key::Key_D:
      m_camera.strafe_right();
      break;
   case Qt::Key::Key_R:
      m_camera.move_up();
      break;
   case Qt::Key::Key_F:
      m_camera.move_down();
      break;
   case Qt::Key::Key_Up:
      m_shape_ptrs_vector[0]->rotate_thineself(+0.1f, glm::vec3(1.0f, 1.0f, 1.0f));
      break;
   case Qt::Key::Key_Down:
      m_shape_ptrs_vector[0]->rotate_thineself(-0.1f, glm::vec3(1.0f, -1.0f, 1.0f));
      break;
   case Qt::Key::Key_Left:
      m_shape_ptrs_vector[0]->rotate_thineself(+0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
      break;
   case Qt::Key::Key_Right:
      m_shape_ptrs_vector[0]->rotate_thineself(-0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
      break;

   default:
      break;
   }

   //this->repaint();
}


// this is a QT SLOT function
void my_GL_window::timer_update()
{
   //this->render_frame();
   glDraw();
}


// this is a QT SLOT function
void my_GL_window::receive_serial_data(float X, float Y, float Z)
{
   //cout << "X = '" << X << "', Y = '" << Y << "', Z = '" << Z << "'" << endl;

   // hijack the shape's ability to point itself at a 2D viewport point
   // Note: The "negative X" is because of accelerometer-openGL axis directions.
   m_shape_ptrs_vector[0]->point_thineself_at_relative_point(glm::vec3(-X, Y, Z));

   //this->repaint();
}


