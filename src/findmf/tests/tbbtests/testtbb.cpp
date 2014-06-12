// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/task_scheduler_init.h>
#include <iostream>
#include <vector>

struct mytask {
  mytask(size_t n)
    :_n(n)
  {}
  void operator()() {
    for (int i=0;i<1000000;++i) {}  // Deliberately run slow
    //std::cerr << "[" << _n << "]";
  }
  size_t _n;
};

struct executor
{
  executor(std::vector<mytask>& t)
    :_tasks(t)
  {}
  executor(executor& e,tbb::split)
    :_tasks(e._tasks)
  {}

  void operator()(const tbb::blocked_range<size_t>& r) const {
    for (size_t i=r.begin();i!=r.end();++i)
      _tasks[i]();
  }

  std::vector<mytask>& _tasks;
};


void test_parrallel_for(){
  std::vector<mytask> tasks;
  for (int i=0;i<1000;++i)
    tasks.push_back(mytask(i));

  executor exec(tasks);
  tbb::parallel_for(tbb::blocked_range<size_t>(0,tasks.size()),exec);
  std::cerr << std::endl;

}

template <typename T> struct invoker {
  void operator()(T& it) const {it();}
};

void test_parrallel_foreach(){
  std::vector<mytask> tasks;
    for (int i=0;i<1000;++i)
      tasks.push_back(mytask(i));

    tbb::parallel_for_each(tasks.begin(),tasks.end(),invoker<mytask>());
    std::cerr << std::endl;

}

int main(int,char**) {

  tbb::task_scheduler_init init;  // Automatic number of threads
  // tbb::task_scheduler_init init(2);  // Explicit number of threads
  test_parrallel_for();
  test_parrallel_foreach();

  return 0;
}
