#include "shape_generator.h"

// for the definition of my custom "shape" object
#include "my_shape.h"

// for the definition of my custom "vertex" object
#include "my_vertex.h"

// for glm vectors and matrices
#include <glm\glm\glm.hpp>
using glm::vec4;
using glm::mat4;

// for glm transformation functions
#include <glm\glm\gtc\matrix_transform.hpp>
using glm::translate;
using glm::perspective;

int shape_generator::make_cube(my_shape *shape_data_to_fill_ptr)
{
   unsigned int array_size_bytes = 0;
   unsigned int array_entry_count = 0;
   int this_ret_val = 0;

   // check for input shenanigans
   if (0 == shape_data_to_fill_ptr)
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

      // fill the shape data with the default vertex data
      array_size_bytes = sizeof(verts);
      array_entry_count = array_size_bytes / sizeof(*verts);
      shape_data_to_fill_ptr->m_num_vertices = array_entry_count;
      shape_data_to_fill_ptr->m_vertices_arr = new my_vertex[array_entry_count];
      memcpy(shape_data_to_fill_ptr->m_vertices_arr, verts, array_size_bytes);

      // fill the shape data with the default index data
      array_size_bytes = sizeof(indices);
      array_entry_count = array_size_bytes / sizeof(*indices);
      shape_data_to_fill_ptr->m_num_indices = array_entry_count;
      shape_data_to_fill_ptr->m_indices_arr = new GLushort[array_entry_count];
      memcpy(shape_data_to_fill_ptr->m_indices_arr, indices, array_size_bytes);
   }

   return this_ret_val;
}
