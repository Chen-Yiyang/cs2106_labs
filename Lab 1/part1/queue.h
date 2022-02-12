
// Maximum number of elements in a queue
//
#define MAX_Q_SIZE	10

// Place function prototypes here.
void enq(double);
double deq();

void sum(double);
void prod(double);
void clear_sum();
void clear_prod();
double reduce();

double flex_reduce( void (*clear)(), void (*op)(double) );

