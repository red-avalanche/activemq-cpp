/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <CMS_Destination.h>

#include <Config.h>
#include <types/CMS_Types.h>

#include <activemq/core/ActiveMQConnection.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <memory>

////////////////////////////////////////////////////////////////////////////////
cms_status createDestination(CMS_Session* session, DESTINATION_TYPE type,
                             const char* name, CMS_Destination** destination) {

    cms_status result = CMS_SUCCESS;
    std::auto_ptr<CMS_Destination> wrapper( new CMS_Destination );

    try{

        if (session == NULL) {
            result = CMS_ERROR;
        } else {

            switch(type) {
                case CMS_TOPIC:
                    wrapper->destination = session->session->createTopic(name);
                    break;
                case CMS_TEMPORARY_TOPIC:
                    wrapper->destination = session->session->createTemporaryTopic();
                    break;
                case CMS_TEMPORARY_QUEUE:
                    wrapper->destination = session->session->createTemporaryQueue();
                    break;
                default:
                    wrapper->destination = session->session->createQueue(name);
                    break;
            }

            wrapper->type = type;
            *destination = wrapper.release();
        }

    } catch(...) {
        result = CMS_ERROR;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
cms_status destroyDestination(CMS_Destination* destination) {

    cms_status result = CMS_SUCCESS;

    if(destination != NULL) {

        try{
            delete destination->destination;
            delete destination;
        } catch(...) {
            result = CMS_ERROR;
        }
    }

    return result;
}
