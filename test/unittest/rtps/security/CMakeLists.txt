# Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(NOT ((MSVC OR MSVC_IDE) AND EPROSIMA_INSTALLER))
    include(${PROJECT_SOURCE_DIR}/cmake/dev/gtest.cmake)
    check_gtest()
    check_gmock()

    if(GTEST_FOUND AND GMOCK_FOUND)
        if(WIN32)
            add_definitions(-D_WIN32_WINNT=0x0601)
        endif()

        include_directories(${ASIO_INCLUDE_DIR})

        set(SOURCES_SECURITY_TEST_SOURCE
            ${PROJECT_SOURCE_DIR}/src/cpp/log/Log.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/log/StdoutConsumer.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/attributes/PropertyPolicy.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/flowcontrol/ThroughputControllerDescriptor.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/TimedEvent.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/TimedEventImpl.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/resources/ResourceEvent.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/common/Token.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/exceptions/Exception.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/security/SecurityManager.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/security/exceptions/SecurityException.cpp
            ${PROJECT_SOURCE_DIR}/src/cpp/rtps/security/timedevent/HandshakeMessageTokenResent.cpp
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/SecurityPluginFactory/rtps/security/SecurityPluginFactory.cpp
            )

        add_executable(SecurityAuthentication ${SOURCES_SECURITY_TEST_SOURCE}
            ${CMAKE_CURRENT_SOURCE_DIR}/SecurityAuthenticationTests.cpp)
        add_gtest(SecurityAuthentication 
            ${CMAKE_CURRENT_SOURCE_DIR}/SecurityAuthenticationTests.cpp)
        target_compile_definitions(SecurityAuthentication PRIVATE FASTRTPS_NO_LIB)
        target_include_directories(SecurityAuthentication PRIVATE
            ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/SecurityPluginFactory
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/Endpoint
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSParticipantImpl
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSWriter
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/StatelessWriter
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/StatefulWriter
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/WriterHistory
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/RTPSReader
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/StatelessReader
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/StatefulReader
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/ReaderHistory
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/PDPSimple
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/EDP
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/ParticipantProxyData
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/ReaderProxyData
            ${PROJECT_SOURCE_DIR}/test/mock/rtps/WriterProxyData
            ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}
            ${PROJECT_SOURCE_DIR}/src/cpp
            )
        target_link_libraries(SecurityAuthentication ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES})
    endif()
endif()
