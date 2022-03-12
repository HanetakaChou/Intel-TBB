/*
    Copyright (c) 2005-2019 Intel Corporation

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

#include "rml_omp.h"
#include "omp_dynamic_link.h"
#include <assert.h>

namespace __kmp {
namespace rml {

#define MAKE_SERVER(x) (x = __KMP_make_rml_server)
#define GET_INFO(x) (x = __KMP_call_with_my_server_info)
#define SERVER omp_server 
#define CLIENT omp_client
#define FACTORY omp_factory

extern "C"
{
    omp_factory::status_type __KMP_make_rml_server(omp_factory &f, omp_server *&server, omp_client &client);
    void __KMP_call_with_my_server_info(::rml::server_info_callback_t cb, void *arg);
}

#include "rml_factory.h"

} // rml
} // __kmp
