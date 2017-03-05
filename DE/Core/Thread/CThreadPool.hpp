/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/
#pragma once
 
#ifndef CTHREADPOOL_HPP
#define CTHREADPOOL_HPP
 
#include "../../Core/Thread/CThreadSafeQueue.hpp"
#include "../../Core/Thread/CThreadTask.hpp"
#include "../../Core/Thread/CTaskFuture.hpp"
 
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
 
namespace DE {
	/**
	* A Thread pool.
	*/
    class CThreadPool {
	private:
        std::atomic_bool m_done;
        CThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
        std::vector<std::thread> m_threads;
	
    public:
        /**
        * Constructor.
		* Create n-1 threads depending on the hardware capability
        */
        CThreadPool() : CThreadPool{std::max(std::thread::hardware_concurrency(), 2u) - 1u} {}
 
        /**
        * Constructor.
		* @param numThreads The number of threads
        */
        explicit CThreadPool(const unsigned int numThreads) {
			this->m_done = false;
            try {
                for(unsigned int i = 0; i < numThreads; ++i) {
                    this->m_threads.emplace_back(&CThreadPool::Worker, this);
                }
            }
            catch(...) {
                Destroy();
                throw;
            }
        }
 
        /**
        * Non-copyable.
        */
        CThreadPool(const CThreadPool& rhs) = delete;
 
        /**
        * Non-assignable.
        */
        CThreadPool& operator=(const CThreadPool& rhs) = delete;
 
        /**
        * Destructor.
        */
        ~CThreadPool() {
            Destroy();
        }
 
        /**
        * Submit a job to be run by the thread pool.
		* @param func The job
		* @param args The arguments
        */
        template <typename Func, typename... Args> auto Submit(Func&& func, Args&&... args)
        {
            auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            using ResultType = std::result_of_t<decltype(boundTask)()>;
            using PackagedTask = std::packaged_task<ResultType()>;
            using TaskType = CThreadTask<PackagedTask>;
             
            PackagedTask task{std::move(boundTask)};
            CTaskFuture<ResultType> result{task.get_future()};
            this->m_workQueue.Push(std::make_unique<TaskType>(std::move(task)));
            return result;
        }
 
    private:
        /**
         * Main function for the threads that process jobs and wait for other jobs to do.
         */
        void Worker() {
            while(!this->m_done) {
                std::unique_ptr<IThreadTask> pTask{nullptr};
                if(this->m_workQueue.WaitPop(pTask)) {
                    pTask->Execute();
                }
            }
        }
 
        /**
         * Invalidates the queue and joins all running threads.
         */
        void Destroy() {
            this->m_done = true;
            this->m_workQueue.Invalidate();
			
            for(auto& thread : this->m_threads) {
                if(thread.joinable()) {
                    thread.join();
                }
            }
        }
    };
 
    namespace DefaultThreadPool
    {
        /**
        * Submit a job to the default thread pool.
        */
        template <typename Func, typename... Args> inline auto SubmitJob(Func&& func, Args&&... args)
        {
            return GetThreadPool().Submit(std::forward<Func>(func), std::forward<Args>(args)...);
        }
 
        /**
        * Get the default thread pool.
		* @return The default thread pool
        */
        inline CThreadPool& GetThreadPool()
        {
            static CThreadPool defaultPool;
            return defaultPool;
        }
    }
}
 
#endif