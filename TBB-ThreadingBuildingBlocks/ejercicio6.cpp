
#include <stdio.h>
#include <iostream>
#include <tbb/pipeline.h>
using namespace std;
using namespace tbb;

#define BUFFER_SIZE 100

class MyBuffer{
    static const size_t buffer_size = 10000;
    char* my_end;
    char storage[1+buffer_size];
  public:
    char* begin() {return storage+1;}
    const char* begin() const {return storage+1;}
    char* end() const{ return my_end;}
    void set_end(char* new_ptr){my_end = new_ptr;}
    size_t max_size() const{return buffer_size;}
    size_t size() const {return my_end-begin();}
  };

  class MyOutputFilter: public tbb::filter{
    FILE* my_output_file;
  public:
    MyOutputFilter(FILE* output_file);
    void* operator()(void* item);
  };

  MyOutputFilter::MyOutputFilter(FILE* output_file):  tbb::filter(true),
    my_output_file(output_file){}

  void* MyOutputFilter::operator()(void* item){
    MyBuffer& b = *static_cast<MyBuffer*>(item);
    fwrite(b.begin(), 1, b.size(), my_output_file);
    return NULL;
  }

  class MyTransformFilter: public tbb::filter{
  public:
    MyTransformFilter( );
    void* operator()( void* item);
  };

  MyTransformFilter::MyTransformFilter(): tbb::filter(false){}

  void* MyTransformFilter::operator()(void* item){
    MyBuffer& b = *static_cast<MyBuffer*>(item);
    bool prev_char_is_space = b.begin()[-1]== ' ';
    for(char* s=b.begin(); s!=b.end(); ++s){
      if(prev_char_is_space && islower(*s))
        *s =  toupper(*s);
      prev_char_is_space = isspace(*s);
    }
    return &b;
  }

  class MyInputFilter: public tbb::filter{
  public:
    static const size_t n_buffer = 4;
    MyInputFilter(FILE* input_file_);
  private:
    FILE* input_file;
    size_t next_buffer;
    char last_char_of_previous_buffer;
    MyBuffer buffer[n_buffer];
    void* operator()(void*);
  };

  MyInputFilter::MyInputFilter(FILE* input_file_) :
    filter(true),
    next_buffer(0),
    input_file(input_file_),
    last_char_of_previous_buffer(' ')
  {}

  void* MyInputFilter::operator()(void*){
    MyBuffer& b = buffer[next_buffer];
    next_buffer = (next_buffer+1) % n_buffer;
    size_t n = fread(b.begin(), 1, b.max_size(), input_file);
    if(!n){
      return NULL;
    }else {
      b.begin()[-1] = last_char_of_previous_buffer;
      last_char_of_previous_buffer = b.begin()[n-1];
      b.set_end(b.begin()+n);
      return &b;
    }
  }


int main( )  {

  FILE* input_file;
  FILE* output_file;
  char fichero_entrada[100];
  char fichero_salida[100];

  
  cout << "Introduce el fichero de entrada: " ;
  cin >> fichero_entrada;
  if( ( input_file = fopen(fichero_entrada,"r") ) == NULL ) {
    cout << "Fichero no existente." << endl;
    return 0;
  }
  cout << "Introduce el fichero de salida: " ;
  cin >> fichero_salida;
  output_file = fopen(fichero_salida,"w");

  /*************************/
  /* Resolucion paralela */
  tbb::pipeline pipeline;
  MyInputFilter input_filter(input_file);
  pipeline.add_filter(input_filter);
  MyTransformFilter transformFilter;
  pipeline.add_filter(transformFilter);
  MyOutputFilter output_filter(output_file);
  pipeline.add_filter(output_filter);
  pipeline.run(MyInputFilter::n_buffer);
  pipeline.clear();
  /*************************/
  
  /*************************/
  /* Resolucion secuencial *//*
  char buffer[BUFFER_SIZE];
  size_t n;
  bool primero = false;
  do {
    n = fread( buffer, 1, BUFFER_SIZE, input_file );
    if( primero ) buffer[0] = toupper(buffer[0]);
    primero = false;
    for( int i=1; i<n; i++ ) {
      char *c = &(buffer[i-1]);
      if( *c == ' ' || *c == '\r' || *c == '\n' ) {
        *c = toupper(*(++c));
      }
    }
    fwrite( buffer, 1, n, output_file );
    if( buffer[n-1] == ' ' ) primero = true;
  } while ( n );
  /*************************/
  
  fclose(input_file);
  fclose(output_file);
}



