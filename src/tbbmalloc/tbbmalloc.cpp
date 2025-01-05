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

#include "TypeDefinitions.h" // Customize.h and proxy.h get included
#include "tbbmalloc_internal_api.h"

#undef UNICODE

#if USE_PTHREAD
#include <dlfcn.h> // dlopen
#elif USE_WINTHREAD
#include "tbb/machine/windows_api.h"
#endif

namespace rml
{
    namespace internal
    {
        void init_tbbmalloc()
        {
#if DO_ITT_NOTIFY
            MallocInitializeITT();
#endif
        }

#if !__TBB_SOURCE_DIRECTLY_INCLUDED
        struct RegisterProcessShutdownNotification
        {
            RegisterProcessShutdownNotification()
            {
            }
            ~RegisterProcessShutdownNotification()
            {
                __TBB_mallocProcessShutdownNotification(false);
            }
        };

        static RegisterProcessShutdownNotification reg;
#endif /* !__TBB_SOURCE_DIRECTLY_INCLUDED */
    }
} // namespaces

#if __TBB_ipf
/* It was found that on IA-64 architecture inlining of __TBB_machine_lockbyte leads
   to serious performance regression with ICC. So keep it out-of-line.

   This code is copy-pasted from tbb_misc.cpp.
 */
extern "C" intptr_t __TBB_machine_lockbyte(volatile unsigned char &flag)
{
    tbb::internal::atomic_backoff backoff;
    while (!__TBB_TryLockByte(flag))
        backoff.pause();
    return 0;
}
#endif
