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
                InitializeSRWLock(&this->m_mutex);
                InitializeConditionVariable(&this->m_cond);

                __TBB_ASSERT_EX(start_count >= 0, NULL);

                this->m_count = start_count;
            }

            //! dtor
            inline ~semaphore()
            {
                // DeleteSRWLock(&this->m_mutex);
                // DeleteConditionVariable(&this->m_cond);
            }

            //! wait/acquire
            inline void P()
            {
                AcquireSRWLockExclusive(&this->m_mutex);

                while (this->m_count <= 0)
                {
                    BOOL ret_cond_wait = SleepConditionVariableSRW(&this->m_cond, &this->m_mutex, INFINITE, 0U);
                    __TBB_ASSERT_EX(FALSE != ret_cond_wait, NULL);
                }

                __TBB_ASSERT(this->m_count > 0, NULL);

                --this->m_count;

                ReleaseSRWLockExclusive(&this->m_mutex);
            }

            //! post/release
            inline void V()
            {
                AcquireSRWLockExclusive(&this->m_mutex);

                __TBB_ASSERT_EX(this->m_count < INT32_MAX, NULL);
                ++this->m_count;

                WakeConditionVariable(&this->m_cond);

                // Thundering Herd
                // WakeAllConditionVariable(&this->m_cond);

                ReleaseSRWLockExclusive(&this->m_mutex);
            }

        private:
            SRWLOCK m_mutex;
            CONDITION_VARIABLE m_cond;
            int32_t m_count;
        };

#else  /* Linux/Unix */
        //! Edsger Dijkstra's counting semaphore
        class semaphore : no_copy
        {
        public:
            //! ctor
            inline semaphore(int32_t start_count = 0)
            {
                int ret_mutex_init = pthread_mutex_init(&this->m_mutex, NULL);
                __TBB_ASSERT_EX(0 == ret_mutex_init, NULL);
                int ret_cond_init = pthread_cond_init(&this->m_cond, NULL);
                __TBB_ASSERT_EX(0 == ret_cond_init, NULL);

                __TBB_ASSERT_EX(start_count >= 0, NULL);

                this->m_count = start_count;
            }

            //! dtor
            inline ~semaphore()
            {
                int ret_mutex_destroy = pthread_mutex_destroy(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_destroy, NULL);
                int ret_cond_destroy = pthread_cond_destroy(&this->m_cond);
                __TBB_ASSERT_EX(0 == ret_cond_destroy, NULL);
            }

            //! wait/acquire
            inline void P()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                while (this->m_count <= 0)
                {
                    int ret_cond_wait = pthread_cond_wait(&this->m_cond, &this->m_mutex);
                    __TBB_ASSERT_EX(0 == ret_cond_wait, NULL);
                }

                __TBB_ASSERT(this->m_count > 0, NULL);

                --this->m_count;

                int ret_mutex_unlock = pthread_mutex_unlock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

            //! post/release
            inline void V()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                __TBB_ASSERT_EX(this->m_count < INT32_MAX, NULL);
                ++this->m_count;

                int ret_cond_signal = pthread_cond_signal(&this->m_cond);
                __TBB_ASSERT_EX(0 == ret_cond_signal, NULL);

                // Thundering Herd
                // int ret_cond_broadcast = pthread_cond_broadcast(&this->m_cond);
                // __TBB_ASSERT_EX(0 == ret_cond_broadcast, NULL);

                int ret_mutex_unlock = pthread_mutex_unlock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

        private:
            pthread_mutex_t m_mutex;
            pthread_cond_t m_cond;
            int32_t m_count;
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
                InitializeSRWLock(&this->m_mutex);
                InitializeConditionVariable(&this->m_cond);
                // initial unset
                this->m_set = false;
            }

            //! dtor
            inline ~binary_semaphore()
            {
                // DeleteSRWLock(&this->m_mutex);
                // DeleteConditionVariable(&this->m_cond);
            }

            //! wait/acquire
            inline void P()
            {
                AcquireSRWLockExclusive(&this->m_mutex);

                while (!this->m_set)
                {
                    BOOL ret_cond_wait = SleepConditionVariableSRW(&this->m_cond, &this->m_mutex, INFINITE, 0U);
                    __TBB_ASSERT_EX(FALSE != ret_cond_wait, NULL);
                }

                __TBB_ASSERT(true == this->m_set, NULL);

                // auto reset
                this->m_set = false;

                ReleaseSRWLockExclusive(&this->m_mutex);
            }

            //! post/release
            inline void V()
            {
                AcquireSRWLockExclusive(&this->m_mutex);

                this->m_set = true;

                WakeConditionVariable(&this->m_cond);

                // Thundering Herd
                // WakeAllConditionVariable(&this->m_cond);

                ReleaseSRWLockExclusive(&this->m_mutex);
            }

        private:
            SRWLOCK m_mutex;
            CONDITION_VARIABLE m_cond;
            bool m_set;
        };
#else  /* Linux/Unix */
        //! binary_semaphore for concurrent monitor
        class binary_semaphore : no_copy
        {
        public:
            //! ctor
            inline binary_semaphore()
            {
                int ret_mutex_init = pthread_mutex_init(&this->m_mutex, NULL);
                __TBB_ASSERT_EX(0 == ret_mutex_init, NULL);
                int ret_cond_init = pthread_cond_init(&this->m_cond, NULL);
                __TBB_ASSERT_EX(0 == ret_cond_init, NULL);
                // initial unset
                this->m_set = false;
            }

            //! dtor
            inline ~binary_semaphore()
            {
                int ret_mutex_destroy = pthread_mutex_destroy(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_destroy, NULL);
                int ret_cond_destroy = pthread_cond_destroy(&this->m_cond);
                __TBB_ASSERT_EX(0 == ret_cond_destroy, NULL);
            }

            //! wait/acquire
            inline void P()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                while (!this->m_set)
                {
                    int ret_cond_wait = pthread_cond_wait(&this->m_cond, &this->m_mutex);
                    __TBB_ASSERT_EX(0 == ret_cond_wait, NULL);
                }

                __TBB_ASSERT(true == this->m_set, NULL);

                // auto reset
                this->m_set = false;

                int ret_mutex_unlock = pthread_mutex_unlock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

            //! post/release
            inline void V()
            {
                int ret_mutex_lock = pthread_mutex_lock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_lock, NULL);

                this->m_set = true;

                int ret_cond_signal = pthread_cond_signal(&this->m_cond);
                __TBB_ASSERT_EX(0 == ret_cond_signal, NULL);

                // Thundering Herd
                // int ret_cond_broadcast = pthread_cond_broadcast(&this->m_cond);
                // __TBB_ASSERT_EX(0 == ret_cond_broadcast, NULL);

                int ret_mutex_unlock = pthread_mutex_unlock(&this->m_mutex);
                __TBB_ASSERT_EX(0 == ret_mutex_unlock, NULL);
            }

        private:
            pthread_mutex_t m_mutex;
            pthread_cond_t m_cond;
            bool m_set;
        };
#endif /* _WIN32||_WIN64 */

    } // namespace internal
} // namespace tbb

#endif /* __TBB_tbb_semaphore_H */
