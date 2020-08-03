# python3

from collections import namedtuple
import heapq as pq

AssignedJob = namedtuple("AssignedJob", ["worker", "started_at"])
class Thread:
    def __init__(self, ID):
        self.ID = ID
        self.next_free_time = 0

    # Refer to stack overflow thread at:
    # https://stackoverflow.com/questions/8875706/heapq-with-custom-compare-predicate
    def __lt__(self, other):
        # If two threads becomes free concurrently, then the thread with smaller ID
        # takes precedence over the one with larger ID

        # determine who is of higher priority, which is different from C++
        if self.next_free_time == other.next_free_time:
            return self.ID < other.ID   # lager ID lower priority

        return self.next_free_time < other.next_free_time  # large next_free_time lower priority

class Job_Queue:
    def __init__(self):
        self.result = []
        self.n_jobs = 0
        self.n_workers = 0
        self.jobs = []

    def read_data(self):
        self.n_workers, self.n_jobs = map(int, input().split())
        self.jobs = list(map(int, input().split()))
        assert len(self.jobs) == self.n_jobs

    def assign_jobs_fast(self):
        # namedtuple is also immutable just like tuple
        # for i in range(self.n_jobs):
        #     self.result.append(AssignedJob(0, 0))

        self.pq_threads = []
        for i in range(self.n_workers):
            pq.heappush(self.pq_threads, Thread(i))

        for i in range(self.n_jobs):
            thread_free = pq.heappop(self.pq_threads)  # return and remove ele with highest priority
            self.result.append(AssignedJob(thread_free.ID, thread_free.next_free_time))
            thread_free.next_free_time += self.jobs[i]
            pq.heappush(self.pq_threads, thread_free)

    def write_response(self):
        for job in self.result:
            print(job.worker, job.started_at)

    def Solve(self):
        self.read_data()
        self.assign_jobs_fast()
        self.write_response()


# def assign_jobs(n_workers, jobs):
#     # TODO: replace this code with a faster algorithm.
#     result = []
#     next_free_time = [0] * n_workers
#     for job in jobs:
#         next_worker = min(range(n_workers), key=lambda w: next_free_time[w])
#         result.append(AssignedJob(next_worker, next_free_time[next_worker]))
#         next_free_time[next_worker] += job

#     return result


def main():
    job_queue = Job_Queue()
    job_queue.Solve()
    # n_workers, n_jobs = map(int, input().split())
    # jobs = list(map(int, input().split()))
    # assert len(jobs) == n_jobs

    # assigned_jobs = assign_jobs(n_workers, jobs)

    # for job in assigned_jobs:
    #     print(job.worker, job.started_at)


if __name__ == "__main__":
    main()
