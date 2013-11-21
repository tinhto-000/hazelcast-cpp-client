//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/queue/PollRequest.h"
#include "hazelcast/client/queue/QueuePortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"


namespace hazelcast {
    namespace client {
        namespace queue {
            PollRequest::PollRequest(const std::string& name, long timeout)
            :name(name)
            , timeoutInMillis(timeout) {

            };

            PollRequest::PollRequest(const std::string& name)
            :name(name)
            , timeoutInMillis(0) {

            };

            int PollRequest::getFactoryId() const {
                return queue::QueuePortableHook::F_ID;
            }

            int PollRequest::getClassId() const {
                return queue::QueuePortableHook::POLL;
            };


            void PollRequest::writePortable(serialization::PortableWriter& writer) const {
                writer.writeUTF("n", name);
                writer.writeLong("t", timeoutInMillis);
            };
        }
    }
}


