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

#include "../include/rml_tbb.h"
#include "library_assert.h"
#include <assert.h>

namespace tbb
{
    namespace internal
    {
        namespace rml
        {

#define MAKE_SERVER(x) ((x) = __TBB_make_rml_server)
#define GET_INFO(x) ((x) = __TBB_call_with_my_server_info)
#define SERVER tbb_server
#define CLIENT tbb_client
#define FACTORY tbb_factory

            extern "C"
            {
                ::rml::factory::status_type __TBB_make_rml_server(tbb::internal::rml::tbb_factory &f, tbb::internal::rml::tbb_server *&server, tbb::internal::rml::tbb_client &client);
                void __TBB_call_with_my_server_info(::rml::server_info_callback_t cb, void *arg);
            }

#include "rml_factory.h"

        } // rml
    } // internal
} // tbb
