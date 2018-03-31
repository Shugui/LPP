#include <tbb/task_scheduler_init.h>
#include <iostream>
	int main() {
	tbb::task_scheduler_init init(8);
	return 0;
}