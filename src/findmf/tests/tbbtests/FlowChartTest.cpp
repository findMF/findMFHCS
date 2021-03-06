
#include <cstring>
#include <cstdio>

const int num_image_buffers = 100;
int image_size = 10000000;

struct image {
  const int N;
  char *data;
  image();
  image( int image_number, bool a, bool b );
};

image::image() : N(image_size) {
  data = new char[N];
}

image::image( int image_number, bool a, bool b ) : N(image_size) {
  data = new char[N];
  memset( data, '\0', N );
  data[0] = (char)image_number - 32;
  if ( a ) data[N-2] = 'A';
  if ( b ) data[N-1] = 'B';
}


int img_number = 0;
int num_images = 64;
const int a_frequency = 11;
const int b_frequency = 13;

// trivial implementations that can be used as the bodies of the function nodes
image *get_next_image() {
  bool a = false, b = false;
  if ( img_number < num_images ) {
    if ( img_number%a_frequency == 0 ) a = true;
    if ( img_number%b_frequency == 0 ) b = true;
    return new image( img_number++, a, b );
  } else {
    return false;
  }
}


void preprocess_image( image *input_image, image *output_image ) {
  for ( int i = 0; i < input_image->N; ++i ) {
    output_image->data[i] = input_image->data[i] + 32;
  }
}

bool detect_with_A( image *input_image ) {
  for ( int i = 0; i < input_image->N; ++i ) {
    if ( input_image->data[i] == 'a' )
      return true;
  }
  return false;
}

bool detect_with_B( image *input_image ) {
  for ( int i = 0; i < input_image->N; ++i ) {
    if ( input_image->data[i] == 'b' )
      return true;
  }
  return false;
}

void output_image( image *input_image, bool found_a, bool found_b ) {
  bool a = false, b = false;
  int a_i = -1, b_i = -1;
  for ( int i = 0; i < input_image->N; ++i ) {
    if ( input_image->data[i] == 'a' ) { a = true; a_i = i; }
    if ( input_image->data[i] == 'b' ) { b = true; b_i = i; }
  }
  printf("Detected feature (a,b)=(%d,%d)=(%d,%d) at (%d,%d) for image %p:%d\n",
         a, b, found_a, found_b, a_i, b_i, input_image, input_image->data[0]);
}


int num_graph_buffers = 8;

#include "tbb/flow_graph.h"

using namespace tbb;
using namespace tbb::flow;

int main() {

  //First, a graph g is created. All of the nodes will belong to this single graph. A few typedefs are provided to make it easier to refer to the outputs of the join nodes:

  graph g;

  typedef std::tuple< image *, image * > resource_tuple;
  typedef std::pair< image *, bool > detection_pair;
  typedef std::tuple< detection_pair, detection_pair > detection_tuple;

  queue_node< image * > buffers( g );
  join_node< resource_tuple, reserving > resource_join( g );
  join_node< detection_tuple, tag_matching > detection_join( g,
                                                             [](const detection_pair &p) -> size_t { return (size_t)p.first; },
  [](const detection_pair &p) -> size_t { return (size_t)p.first; }  );


  source_node< image * > src( g, []( image* &next_image ) -> bool {
    next_image = get_next_image();
    if ( next_image ) return true;
    else return false;
  }
  );
  make_edge(src, input_port<0>(resource_join) );
  make_edge(buffers, input_port<1>(resource_join) );


  function_node< resource_tuple, image * >
      preprocess_function( g, unlimited,
                           []( const resource_tuple &in ) -> image * {
    image *input_image = std::get<0>(in);
    image *output_image = std::get<1>(in);
    preprocess_image( input_image, output_image );
    delete input_image;
    return output_image;
  }
  );

  make_edge(resource_join, preprocess_function );

  function_node< image *, detection_pair >
      detect_A( g, unlimited,
                []( image *input_image ) -> detection_pair {
    bool r = detect_with_A( input_image );
    return std::make_pair( input_image, r );
  }
  );

  function_node< image *, detection_pair >
      detect_B( g, unlimited,
                []( image *input_image ) -> detection_pair {
    bool r = detect_with_B( input_image );
    return std::make_pair( input_image, r );
  }
  );

  make_edge(preprocess_function, detect_A );
  make_edge(detect_A, input_port<0>(detection_join) );
  make_edge(preprocess_function, detect_B );
  make_edge(detect_B, input_port<1>(detection_join) );

  function_node< detection_tuple, image * >
      decide( g, serial,
              []( const detection_tuple &t ) -> image * {
    const detection_pair &a = std::get<0>(t);
    const detection_pair &b = std::get<1>(t);
    image *img = a.first;
    if ( a.second || b.second ) {
      output_image( img, a.second, b.second );
    }
    return img;
  }
  );

  make_edge(detection_join, decide);
  make_edge(decide, buffers);

  // Put image buffers into the buffer queue
  for ( int i = 0; i < num_graph_buffers; ++i ) {
    image *img = new image;
    buffers.try_put( img );
  }
  g.wait_for_all();

  for ( int i = 0; i < num_graph_buffers; ++i ) {
    image *img = NULL;
    if ( !buffers.try_get(img) )
      printf("ERROR: lost a buffer\n");
    else
      delete img;
  }
  return 0;
}
