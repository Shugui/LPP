
#include <iostream>
#include <tbb/parallel_while.h>
using namespace std;
using namespace tbb;

void Foo( double& f ) { 
  f = f*f; 
}

struct s_item {
  double data;
  struct s_item *next;
};

typedef struct s_item Item;
class ItemStream {
  Item* my_ptr;
public:
  bool pop_if_present(Item*& item){
    if (my_ptr){
      item = my_ptr;
      my_ptr = my_ptr->next;
      return true;
    }else{
      return false;
    }
  }
  ItemStream( Item* root ) : my_ptr(root) {}
};

class ApplyFoo{
public:
  void operator()(Item* item)const{
    Foo(item->data);
  }
  typedef Item* argument_type;
};

void ParallelApplyFooToList(Item* root){
  parallel_while<ApplyFoo> w;
  ItemStream stream(root);
  ApplyFoo body;
  w.run( stream, body);
}

void SerialApplyFooToList( Item *root ) { 
  for( Item* ptr=root; ptr!=NULL; ptr=ptr->next )
    Foo(ptr->data);
}

int main( )  {

  long n = 10;
  Item *root = NULL;
  root = new( Item );
  root->data = 0.0;
  Item *p; 
  size_t i;
  for( i=1, p = root; i<n; i++, p = p->next ) {
    p->next = new( Item );
    p->next->data = (double) i;
    p->next->next = NULL;
  }
  //SerialApplyFooToList( root );
  ParallelApplyFooToList( root );
  cout << "Valores = " << endl;
  for( p = root; p!=NULL; p=p->next ) {
    cout << "Dato = " << p->data << endl;
  }

}

