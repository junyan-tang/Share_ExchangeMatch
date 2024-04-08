#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>


using namespace std;

class threadpool{
private: 
    vector <thread> threads;
    queue <function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;

public:
    threadpool(size_t size){
        stop = false;
        for(size_t i = 0; i < size; i++){
            threads.push_back(thread([this]{
                while(true){
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queue_mutex);
                        condition.wait(lock, [this]{return stop || !tasks.empty();});
                        if(stop && tasks.empty()){
                            return;
                        }
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            }));
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type>{
        using return_type = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );

        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(queue_mutex);

            if(stop){
                throw runtime_error("enqueue on stopped threadpool");
            }

            tasks.emplace([task](){(*task)();});
        }
        condition.notify_one();
        return res;

    }

    ~threadpool(){
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for(thread &t: threads){
            t.join();
        }
    }
    

};