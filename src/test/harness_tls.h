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

class LimitTLSKeysTo
{
#if _WIN32 || _WIN64
    typedef DWORD handle;
#else // _WIN32 || _WIN64
    typedef pthread_key_t handle;
#endif
    // for platforms that not limit number of TLS keys, set artificial limit
    static const int LIMIT = 16 * 1024;
    handle handles[LIMIT];
    int lastUsedIdx;

public:
    LimitTLSKeysTo(int keep_keys)
    {
        for (lastUsedIdx = 0; lastUsedIdx < LIMIT; lastUsedIdx++)
        {
#if _WIN32 || _WIN64
            handle h = FlsAlloc(NULL);
            if (h == FLS_OUT_OF_INDEXES)
#else
            int setspecific_dummy = 10;
            if (pthread_key_create(&handles[lastUsedIdx], NULL) != 0)
#endif
            {
                break;
            }
#if _WIN32 || _WIN64
            handles[lastUsedIdx] = h;
#else
            pthread_setspecific(handles[lastUsedIdx], &setspecific_dummy);
#endif
        }
        lastUsedIdx--;
        ASSERT(lastUsedIdx >= keep_keys - 1, "Less TLS keys are available than requested");
        for (; keep_keys > 0; keep_keys--, lastUsedIdx--)
        {
#if _WIN32 || _WIN64
            BOOL ret = !FlsFree(handles[lastUsedIdx]); // fail is zero
#else
            int ret = pthread_key_delete(handles[lastUsedIdx]);
            ASSERT(!ret, "Can't delete a key");
#endif
        }
        REMARK("%d thread local objects allocated in advance\n", lastUsedIdx + 1);
    }
    ~LimitTLSKeysTo()
    {
        for (int i = 0; i <= lastUsedIdx; i++)
        {
#if _WIN32 || _WIN64
            BOOL ret = !FlsFree(handles[i]); // fail is zero
#else
            int ret = pthread_key_delete(handles[i]);
#endif
            ASSERT(!ret, "Can't delete a key");
        }
        lastUsedIdx = 0;
    }
};
