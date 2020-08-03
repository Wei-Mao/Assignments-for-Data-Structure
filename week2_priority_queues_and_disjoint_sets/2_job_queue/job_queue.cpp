#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>   // std:priority_queue

using namespace std;
class Thread 
{ 
  public:
    // One thread has two data members: ID and the time when it becomes free again.
    int ID;
    long long next_free_time;

    // Constructor
    Thread(int _ID)
      :ID(_ID)
    {
      next_free_time = 0;
    }
};


class Compare_Thread
{
  public:
    // determine who is of lower priority
    bool operator () (Thread & thread1, Thread &thread2)
    {
      if(thread1.next_free_time == thread2.next_free_time)
      {
	// For the two threads who become free simultaneously, 
	// the tread with smaller ID is given precedence over the one with larger ID.
	return thread1.ID >  thread2.ID;
      }

      return thread1.next_free_time > thread2.next_free_time;
    }
};

class JobQueue {
 private:
  int num_workers_;
  vector<int> jobs_;

  vector<int> assigned_workers_;
  vector<long long> start_times_;

  void WriteResponse() const {
    for (int i = 0; i < jobs_.size(); ++i) {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData() {
    int m;
    cin >> num_workers_ >> m;
    jobs_.resize(m);
    for(int i = 0; i < m; ++i)
      cin >> jobs_[i];
  }

  void AssignJobs() {
    // TODO: replace this code with a faster algorithm.
    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());
    vector<long long> next_free_time(num_workers_, 0);
    for (int i = 0; i < jobs_.size(); ++i) {
      int duration = jobs_[i];
      int next_worker = 0;
      for (int j = 0; j < num_workers_; ++j) {
        if (next_free_time[j] < next_free_time[next_worker])
          next_worker = j;
      }
      assigned_workers_[i] = next_worker;
      start_times_[i] = next_free_time[next_worker];
      next_free_time[next_worker] += duration;
    }
  }

  void assign_jobs_fast()
  {
    /*
       In contrast with the naive implementation, we replace vector of threads with priority_queue of threads to perform the search for suitable worker in logarithmic time.
       Hence, the total running time falls/drops from O(n^2) to  O(nlogn)
    */ 
    // Info to be stored
    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());

    priority_queue<Thread, vector<Thread>, Compare_Thread> pq_threads;
    for(int i(0); i < num_workers_; i++)
    {
      pq_threads.push(Thread(i));
    }

    for(int i(0); i < jobs_.size(); i ++)
    {
      int duration = jobs_[i];
      Thread thread_free = pq_threads.top();
      pq_threads.pop();
      assigned_workers_[i] = thread_free.ID;
      start_times_[i] = thread_free.next_free_time;
      thread_free.next_free_time += duration;
      pq_threads.push(thread_free);
    }
  }

 public:
  void Solve() {
    ReadData();
    /* AssignJobs(); */
    assign_jobs_fast();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  JobQueue job_queue;
  job_queue.Solve();
  return 0;
}
