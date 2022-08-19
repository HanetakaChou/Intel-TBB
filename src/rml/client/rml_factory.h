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

// No ifndef guard because this file is not a normal include file.

const ::rml::versioned_object::version_type CLIENT_VERSION = 2;

extern "C"
{
    ::rml::factory::status_type __RML_open_factory(::rml::factory &, ::rml::versioned_object::version_type &, ::rml::versioned_object::version_type);
    void __RML_close_factory(::rml::factory &f);
}

::rml::factory::status_type FACTORY::open()
{
    status_type (*open_factory_routine)(factory &, version_type &, version_type) = __RML_open_factory;
    my_wait_to_close_routine = __RML_close_factory;
    MAKE_SERVER(my_make_server_routine);
    GET_INFO(my_call_with_server_info_routine);

    version_type server_version;
    status_type result = (*open_factory_routine)(*this, server_version, CLIENT_VERSION);
    // server_version can be checked here for incompatibility if necessary.

    return result;
}

void FACTORY::close()
{
    if (my_wait_to_close_routine)
    {
        (*my_wait_to_close_routine)(*this);
    }
}

#include "library_assert.h"

::rml::factory::status_type FACTORY::make_server( SERVER*& s, CLIENT& c) {
    // Failure of following assertion means that factory was not successfully opened.
    LIBRARY_ASSERT( my_make_server_routine, NULL );
    return (*my_make_server_routine)(*this,s,c);
}

void FACTORY::call_with_server_info( ::rml::server_info_callback_t cb, void* arg ) const {
    // Failure of following assertion means that factory was not successfully opened.
    LIBRARY_ASSERT( my_call_with_server_info_routine, NULL );
    (*my_call_with_server_info_routine)( cb, arg );
}
