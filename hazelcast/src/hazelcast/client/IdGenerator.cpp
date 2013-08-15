#include "hazelcast/client/IdGenerator.h"
#include "hazelcast/client/spi/DistributedObjectListenerService.h"

namespace hazelcast {
    namespace client {

        IdGenerator::IdGenerator()
        : local(new boost::atomic<int>(-1))
        , residue(new boost::atomic<int>(BLOCK_SIZE))
        , localLock(new boost::mutex) {

        };

        void IdGenerator::init(const std::string& instanceName, spi::ClientContext *clientContext) {
            this->context = clientContext;
            this->instanceName = instanceName;
            atomicLong.init(instanceName, clientContext);
        };


        bool IdGenerator::init(long id) {
            if (id <= 0) {
                return false;
            }
            long step = (id / BLOCK_SIZE);

            boost::lock_guard<boost::mutex> lg(*localLock);
            bool init = atomicLong.compareAndSet(0, step + 1);
            if (init) {
                *local = step;
                *residue = (id % BLOCK_SIZE) + 1;
            }
            return init;
        };

        long IdGenerator::newId() {
            int value = (*residue)++;
            if (value >= BLOCK_SIZE) {
                boost::lock_guard<boost::mutex> lg(*localLock);
                value = *residue;
                if (value >= BLOCK_SIZE) {
                    *local = atomicLong.getAndIncrement();
                    *residue = 0;
                }
                return newId();

            }
            return int(*local) * BLOCK_SIZE + value;
        };

        void IdGenerator::destroy() {
            atomicLong.destroy();
            context->getDistributedObjectListenerService().removeDistributedObject(instanceName);
        };

    }
}