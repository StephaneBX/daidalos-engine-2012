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
 
#ifndef __CTHREADSAFEQUEUE_HPP
#define __CTHREADSAFEQUEUE_HPP
 
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
 
namespace daidalosengine {
	/**
	* A thread safe queue implementation.
	*/
    template <typename T> class CThreadSafeQueue {
		
	private:
        std::atomic_bool m_valid{true};
        mutable std::mutex m_mutex;
        std::queue<T> m_queue;
        std::condition_variable m_condition;
		
    public:
        /**
         * The destructor.
         */
        ~CThreadSafeQueue() {
            Invalidate();
        }
 
		/**
         * Push a new value onto the queue.
		 * @param value The value
         */
        void Push(T value) {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            this->m_queue.push(std::move(value));
            this->m_condition.notify_one();
        }
		
		/**
         * Clear all items from the queue.
         */
        void Clear() {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            while(!this->m_queue.empty()) {
                this->m_queue.pop();
            }
            this->m_condition.notify_all();
        }
 
        /**
         * Invalidate the queue.
         * Used to ensure no conditions are being waited on in waitPop when
         * a thread or the application is trying to exit.
         * The queue is invalid after calling this method and it is an error
         * to continue using a queue after this method has been called.
         */
        void Invalidate() {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            this->m_valid = false;
            this->m_condition.notify_all();
        }
 
        /**
		 * Try to get the first value in the queue
		 * @param out The object to put the value into
         * @return true if a value was successfully written to the out parameter, false otherwise.
         */
        bool TryPop(T & out) {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            if(this->m_queue.empty() || !this->m_valid) {
                return false;
            }
            out = std::move(this->m_queue.front());
            this->m_queue.pop();
            return true;
        }
 
        /**
         * Try to get the first value in the queue
         * Will block until a value is available unless clear is called or the instance is destructed.
		 * @param out The object to put the value into
         * @return true if a value was successfully written to the out parameter, false otherwise.
         */
        bool WaitPop(T & out) {
			
			// Wait for a value or a invalidation
            std::unique_lock<std::mutex> lock{this->m_mutex};
            this->m_condition.wait(lock, [this]() {
                return !this->m_queue.empty() || !this->m_valid;
            });
            
			// If the queue became invalid during our wait
            if(!this->m_valid) {
                return false;
            }
			
            out = std::move(this->m_queue.front());
            this->m_queue.pop();
            return true;
        }

        /**
         * Check if the queue is empty.
		 * @return true if the queue is empty, false otherwise
         */
        bool IsEmpty() const {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            return this->m_queue.empty();
        }
 
        /**
         * Check if the queue is valid.
		 * @return true if the queue is valid, false otherwise
         */
        bool IsValid() const {
            std::lock_guard<std::mutex> lock{this->m_mutex};
            return this->m_valid;
        }
    };
}
 
#endif