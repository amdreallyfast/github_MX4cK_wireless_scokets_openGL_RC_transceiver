#include "shape_generator.h"

// for the definition of my custom "shape" object
#include "my_shape.h"

// for the definition of my custom "vertex" object
#include "my_vertex.h"

// for glm vectors and matrices
#include "my_glm_hpp_include.h"
using glm::vec4;
using glm::mat4;

// for glm transformation functions
#include <glm\glm\gtc\matrix_transform.hpp>
using glm::translate;
using glm::perspective;

int shape_generator::make_cube(my_shape **shape_ptr_to_assign_to)
{
   unsigned int array_size_bytes = 0;
   unsigned int array_entry_count = 0;
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == shape_ptr_to_assign_to)
   {
      this_ret_val = -1;
   }

   if (0 == this_ret_val)
   {
      // declare your vertices
      // Note: This is a cube, so we will have 8 vertices, one for each corner.
      // Also Note: Since my_vertex is a struct, I can initialize the array using
      // using vector objects, and since position comes before color in the 
      // structure, the addresses will all line up.  This is generally poor 
      // practice, but the openGL API is based on C, so my hands are a bit tied 
      // with initializing things.
      //
      // Also Also Note: The top right front corner is +1.0, +1.0, +1.0.
      my_vertex verts[] = 
      {
         vec4(+1.0f, +1.0f, +1.0f, 1.0f),    // position (right top front corner)
         vec4(1.0f, 0.0f, 0.0f, 1.0f),       // color (red)

         vec4(+1.0f, +1.0f, -1.0f, 1.0f),    // right top back corner
         vec4(1.0f, 1.0f, 0.0f, 1.0f),       // red + green

         vec4(+1.0f, -1.0f, +1.0f, 1.0f),    // right bottom front corner
         vec4(0.0f, 1.0f, 0.0f, 1.0f),       // green

         vec4(+1.0f, -1.0f, -1.0f, 1.0f),    // right bottom back corner
         vec4(0.0f, 1.0f, 1.0f, 1.0f),       // green + blue

         vec4(-1.0f, +1.0f, +1.0f, 1.0f),    // left top front corner
         vec4(0.0f, 0.0f, 1.0f, 1.0f),       // blue

         vec4(-1.0f, +1.0f, -1.0f, 1.0f),    // left top back corner
         vec4(1.0f, 0.0f, 1.0f, 1.0f),       // blue + red

         vec4(-1.0f, -1.0f, +1.0f, 1.0f),    // left bottom front corner
         vec4(0.5f, 0.5f, 0.5f, 1.0f),       // a little bit of everything

         vec4(-1.0f, -1.0f, -1.0f, 1.0f),    // left bottom back corner
         vec4(1.0f, 0.0f, 0.0f, 1.0f),       // back to red
      };

      // openGL will happily draw vertices and not double up on any of them, so I
      // need to specify the indices of the vertices to draw
      // Note: Each index value of "verts" is a "my vertex" object, not a "vec4".
      GLushort indices[] = 
      {
         0, 1, 5, 0, 4, 5,    // top face
         2, 3, 7, 2, 6, 7,    // bottom face
         0, 2, 6, 0, 4, 6,    // front face
         1, 3, 7, 1, 5, 7,    // back face
         0, 2, 3, 0, 1, 3,    // right face
         4, 6, 7, 4, 5, 7,    // left face
      };


      // now make a new shape and return it via the supplied pointer
      *shape_ptr_to_assign_to = new my_shape(
         verts,
         sizeof(verts) / sizeof(my_vertex),
         indices,
         sizeof(indices) / sizeof(GLushort)
         );
   }

   return this_ret_val;
}

int shape_generator::make_double_triangle(my_shape **shape_ptr_to_assign_to)
{
   unsigned int array_size_bytes = 0;
   unsigned int array_entry_count = 0;
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == shape_ptr_to_assign_to)
   {
      this_ret_val = -1;
   }

   if (0 == this_ret_val)
   {
      // declare your vertices
      const float RED_TRIANGLE_Z = +0.5f;
      const float BLUE_TRIANGLE_Z = -0.5f;
      my_vertex verts[] =
      {
         // first triangle
         glm::vec4(-1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // left bottom corner
         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),          // all red
         glm::vec4(+1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // right bottom corner
         glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),          // red + green (apparently this makes yellow)
         glm::vec4(+0.0f, +1.0f, -1.0f, 1.0f),    // center top
         glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),          // red + blue (apparently this makes pink

         // second triangle
         glm::vec4(-1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // left top corner
         glm::vec4(0.0f, 0.0f, 1.0f, 0.1f),          // all blue
         glm::vec4(+1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // right top corner
         glm::vec4(1.0f, 0.0f, 1.0f, 0.5f),          // blue + red
         glm::vec4(+0.0f, -1.0f, BLUE_TRIANGLE_Z, 1.0f),   // center bottom
         glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),          // blue + green
      };

      GLushort indices[] = { 0, 1, 2, 3, 4, 5 };

      // now make a new shape and return it via the supplied pointer
      *shape_ptr_to_assign_to = new my_shape(
         verts,
         sizeof(verts) / sizeof(my_vertex),
         indices,
         sizeof(indices) / sizeof(GLushort)
         );
   }

   return this_ret_val;
}

int shape_generator::make_square(my_shape **shape_ptr_to_assign_to)
{
   unsigned int array_size_bytes = 0;
   unsigned int array_entry_count = 0;
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == shape_ptr_to_assign_to)
   {
      this_ret_val = -1;
   }

   if (0 == this_ret_val)
   {
      // declare your vertices
      my_vertex verts[] =
      {
         glm::vec4(+0.5f, +0.5f, +0.0f, 1.0f),     // top right corner
         glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),        // red

         glm::vec4(+0.5f, -0.5f, +0.0f, 1.0f),     // bottom right corner
         glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),        // green

         glm::vec4(-0.5f, -0.5f, +0.0f, 1.0f),     // bottom left corner
         glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),        // blue

         glm::vec4(-0.5f, +0.5f, +0.0f, 1.0f),     // top left corner
         glm::vec4(0.5f, 0.5f, 0.5f, 0.0f),        // a little bit of everything
      };

      GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

      // now make a new shape and return it via the supplied pointer
      *shape_ptr_to_assign_to = new my_shape(
         verts,
         sizeof(verts) / sizeof(my_vertex),
         indices,
         sizeof(indices) / sizeof(GLushort)
         );
   }

   return this_ret_val;
}


int shape_generator::make_pyramid(my_shape **shape_ptr_to_assign_to)
{
   unsigned int array_size_bytes = 0;
   unsigned int array_entry_count = 0;
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == shape_ptr_to_assign_to)
   {
      this_ret_val = -1;
   }

   if (0 == this_ret_val)
   {
      // declare your vertices
      my_vertex verts[] =
      {
         glm::vec4(+0.0f, +0.5f, +0.0f, 1.0f),     // top center
         glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),        // red

         glm::vec4(+0.5f, -0.5f, +0.5f, 1.0f),     // right bottom front corner
         glm::vec4(1.0f, 1.0f, 0.0f, 0.0f),        // red + green = ??

         glm::vec4(+0.5f, -0.5f, -0.5f, 1.0f),     // right bottom back corner
         glm::vec4(0.0f, 1.0f, 1.0f, 0.0f),        // green + blue = ??

         glm::vec4(-0.5f, -0.5f, +0.5f, 1.0f),     // left bottom front corner
         glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),        // red + blue = ??

         glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),     // left bottom back corner
         glm::vec4(0.5f, 0.5f, 0.5f, 0.0f),        // a little bit of everything
      };

      GLushort indices[] = { 
         0, 1, 2,       // right face
         0, 2, 4,       // back face
         0, 4, 3,       // left face
         0, 3, 1,       // front face
         1, 2, 4, 1, 4, 3  // bottom face
      };

      // now make a new shape and return it via the supplied pointer
      *shape_ptr_to_assign_to = new my_shape(
         verts,
         sizeof(verts) / sizeof(my_vertex),
         indices,
         sizeof(indices) / sizeof(GLushort)
         );
   }

   return this_ret_val;
}
