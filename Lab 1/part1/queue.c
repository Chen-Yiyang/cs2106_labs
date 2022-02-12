#include "queue.h"
#include <stdio.h>

static double  _queue[MAX_Q_SIZE];
static int _front = 0, _rear = 0;

void enq(double  data) {
	if ((_front + 1) % MAX_Q_SIZE == _rear){
		printf("Error: Queue is full. Item value %3.2f is not added.\n", data);
	}
	else {
		_queue[_front] = data;
		_front = (_front + 1) % MAX_Q_SIZE;
	}
}

double deq() {
	double val = -1;
	if (_rear == _front){
		printf("Error: Queue is empty. Nothing to return\n");
	}
	else {
		val = _queue[_rear];
		_rear = (_rear + 1) % MAX_Q_SIZE;
	}

	return val;
}

/* This section is for the function pointers exercise */

static double _res;

void sum(double x) {
    _res += x;
}

void prod(double x) {
    _res *= x;
}

void clear_sum() {
    _res = 0;
}

void clear_prod() {
    _res = 1.0;
}

double reduce() {
    int ndx = _rear;

    clear_sum();
    while(ndx != _front) {
        sum(_queue[ndx]);
        ndx = (ndx + 1) % MAX_Q_SIZE;
    }
    return _res;
}

/* Implement flex_reduce here:

double flex_reduce(clear, op){
    // clear(); // Clear _res to either 0 or 1
	// for every element in queue:
		//Call op with element.

	return _res;

}

*/

double flex_reduce(void (*clear) (), void (*op) (double)) {
    int ndx = _rear;
	
    (*clear)();

    while (ndx != _front) {
        (*op)(_queue[ndx]);
	ndx = (ndx + 1) % MAX_Q_SIZE;
    }
    return _res;
}


