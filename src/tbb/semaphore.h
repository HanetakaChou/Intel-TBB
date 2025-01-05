/*
    Copyright (c) 2005-2020 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef __TBB_tbb_semaphore_H
#define __TBB_tbb_semaphore_H

#include "tbb/tbb_stddef.h"

#if _WIN32 || _WIN64
#include "tbb/machine/windows_api.h"
#else
#include <pthread.h>
#endif

namespace tbb
{
    namespace internal
    {
#if _WIN32 || _WIN64
        //! Edsger Dijkstra's counting semaphore
        class semaphore : no_copy
        {
        public:
            //! ctor
            inline semaphore(int32_t start_count = 0)
            {
                InitializeCriticalSection(&this->my_mutex);
                InitializeConditionVariable(&this->my_cond);

                __TBB_ASSERT_EX(start_count >= 0, NULL);

                this->my_count = start_count;
            }

            //! dtor
            inline ~semaphore()
            {
                DeleteCriticalSection(&this->my_mutex);
                // DeleteConditionVariable(&this->my_cond);
            }

            //! wait/acquire
            inline void P()
            {
                EnterCriticalSection(&this->my_mutex);

                while (0 == this->my_count)
                {
                    SleepConditionVariableCS(&this->my_cond, &this->my_mutex, INFINITE);
                }

                __TBB_ASSERT(this->my_count > 0, NULL);

                --this->my_count;

                LeaveCriticalSection(&this->my_mutex);
            }

            //! post/release
            inline void V()
            {
                EnterCriticalSection(&this->my_mutex);

                ++this->my_count;

                WakeConditionVariable(&this->my_cond);

                // Thundering Herd
                // WakeAllConditionVariable(&this->my_cond);

                LeaveCriticalSection(&this->my_mutex);
            }

        private:
            CRITICAL_SECTION my_mutex;
            CONDITION_VARIABLE my_cond;
            int32_t my_count;
        };

#else  /* Linux/Unix */
        //! Edsger Dijkstra's counting semaphore
        class semaphore : no_copy
        {
        public:
            //! ctor
            inline semaphore(int32_t start_count = 0)
            {
                int ret_mutex_init = pthread_mutex_init(&this->my_mutex, NULL);
                __TBB_ASSERT_EX(0 == ret_mutex_init, NULL);
                int ret_cond_init = pthread_cond_init(&this->my_cond, NULL);
                __TBB_ASSERT_EX(0 == ret_cond_init, NULL);

                __TBB_ASSERT_EX(start_count >= 0, NULL);

                this->my_count = start_count;
            }

            //! dtor
            inline ~semaphore()
            {
                int ret_mutex_destroy = pthread_mutex_destroy(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_destroy, NULL);
                int ret_cond_destroy = pthread_cond_destroy(&this->my_cond);
                __TBB_ASSERT_EX(0 == ret_cond_destroy, NULL);
            }

            //! wait/acquire
            inline void P()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                while (0 == this->my_count)
                {
                    int ret_cond_wait = pthread_cond_wait(&this->my_cond, &this->my_mutex);
                    __TBB_ASSERT_EX(0 == ret_cond_wait, NULL);
                }

                __TBB_ASSERT(this->my_count > 0, NULL);

                --this->my_count;

                int ret_mutex_unlock = pthread_mutex_unlock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

            //! post/release
            inline void V()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                ++this->my_count;

                int ret_cond_signal = pthread_cond_signal(&this->my_cond);
                __TBB_ASSERT_EX(0 == ret_cond_signal, NULL);

                // Thundering Herd
                // int ret_cond_broadcast = pthread_cond_broadcast(&this->my_cond);
                // __TBB_ASSERT_EX(0 == ret_cond_broadcast, NULL);

                int ret_mutex_unlock = pthread_mutex_unlock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

        private:
            pthread_mutex_t my_mutex;
            pthread_cond_t my_cond;
            int32_t my_count;
        };
#endif /* _WIN32||_WIN64 */

//! for performance reasons, we want specialized binary_semaphore
#if _WIN32 || _WIN64
        //! binary_semaphore for concurrent_monitor
        class binary_semaphore : no_copy
        {
        public:
            //! ctor
            inline binary_semaphore()
            {
                InitializeCriticalSection(&this->my_mutex);
                InitializeConditionVariable(&this->my_cond);
                // initial unset
                this->my_set = false;
            }

            //! dtor
            inline ~binary_semaphore()
            {
                DeleteCriticalSection(&this->my_mutex);
                // DeleteConditionVariable(&this->my_cond);
            }

            //! wait/acquire
            inline void P()
            {
                EnterCriticalSection(&this->my_mutex);

                while (!this->my_set)
                {
                    SleepConditionVariableCS(&this->my_cond, &this->my_mutex, INFINITE);
                }

                __TBB_ASSERT(true == this->my_set, NULL);

                // auto reset
                this->my_set = false;

                LeaveCriticalSection(&this->my_mutex);
            }

            //! post/release
            inline void V()
            {
                EnterCriticalSection(&this->my_mutex);

                this->my_set = true;

                WakeConditionVariable(&this->my_cond);

                // Thundering Herd
                // WakeAllConditionVariable(&this->my_cond);

                LeaveCriticalSection(&this->my_mutex);
            }

        private:
            CRITICAL_SECTION my_mutex;
            CONDITION_VARIABLE my_cond;
            bool my_set;
        };
#else  /* Linux/Unix */
        //! binary_semaphore for concurrent monitor
        class binary_semaphore : no_copy
        {
        public:
            //! ctor
            inline binary_semaphore()
            {
                int ret_mutex_init = pthread_mutex_init(&this->my_mutex, NULL);
                __TBB_ASSERT_EX(0 == ret_mutex_init, NULL);
                int ret_cond_init = pthread_cond_init(&this->my_cond, NULL);
                __TBB_ASSERT_EX(0 == ret_cond_init, NULL);
                // initial unset
                this->my_set = false;
            }

            //! dtor
            inline ~binary_semaphore()
            {
                int ret_mutex_destroy = pthread_mutex_destroy(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_destroy, NULL);
                int ret_cond_destroy = pthread_cond_destroy(&this->my_cond);
                __TBB_ASSERT_EX(0 == ret_cond_destroy, NULL);
            }

            //! wait/acquire
            inline void P()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                while (!this->my_set)
                {
                    int ret_cond_wait = pthread_cond_wait(&this->my_cond, &this->my_mutex);
                    __TBB_ASSERT_EX(0 == ret_cond_wait, NULL);
                }

                __TBB_ASSERT(true == this->my_set, NULL);

                // auto reset
                this->my_set = false;

                int ret_mutex_unlock = pthread_mutex_unlock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

            //! post/release
            inline void V()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                this->my_set = true;

                int ret_cond_signal = pthread_cond_signal(&this->my_cond);
                __TBB_ASSERT_EX(0 == ret_cond_signal, NULL);

                // Thundering Herd
                // int ret_cond_broadcast = pthread_cond_broadcast(&this->my_cond);
                // __TBB_ASSERT_EX(0 == ret_cond_broadcast, NULL);

                int ret_mutex_unlock = pthread_mutex_unlock(&this->my_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

        private:
            pthread_mutex_t my_mutex;
            pthread_cond_t my_cond;
            bool my_set;
        };
#endif /* _WIN32||_WIN64 */

    } // namespace internal
} // namespace tbb

#endif /* __TBB_tbb_semaphore_H */
