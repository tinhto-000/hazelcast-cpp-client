//
// Created by sancar koyunlu on 6/21/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_ADD_ENTRY_LISTENER_REQUEST
#define HAZELCAST_ADD_ENTRY_LISTENER_REQUEST

#include "hazelcast/client/impl/ClientRequest.h"
#include "hazelcast/client/serialization/pimpl/Data.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace map {

            class AddEntryListenerRequest : public impl::ClientRequest {
            public:
                AddEntryListenerRequest(const std::string &name, bool includeValue);

                AddEntryListenerRequest(const std::string &name, bool includeValue, const serialization::pimpl::Data &key, const std::string &sql);

                AddEntryListenerRequest(const std::string &name, bool includeValue, const std::string &sql);

                AddEntryListenerRequest(const std::string &name, bool includeValue, const serialization::pimpl::Data &key);

                int getFactoryId() const;

                int getClassId() const;

                const serialization::pimpl::Data *getKey() const;

                void write(serialization::PortableWriter &writer) const;

            private:
                static int ALL_LISTENER_FLAGS;
                std::string name;
                bool includeValue;
                serialization::pimpl::Data key;
                std::string sql;
                bool hasKey;
                bool hasPredicate;
            };

        }
    }
}

#endif //HAZELCAST_ADD_ENTRY_LISTENER_REQUEST

