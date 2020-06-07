#include <iostream>

#include <iceoryx_posh/runtime/posh_runtime.hpp>
#include <iceoryx_utils/posix_wrapper/semaphore.hpp>

#include "iceoryx_dds/gateway/dds_to_iox.hpp"

class ShutdownManager
{
  public:
    static void scheduleShutdown(int num)
    {
        char reason;
        psignal(num, &reason);
        s_semaphore.post();
    }
    static void waitUntilShutdown()
    {
        s_semaphore.wait();
    }

  private:
    static iox::posix::Semaphore s_semaphore;
    ShutdownManager() = default;
};
iox::posix::Semaphore ShutdownManager::s_semaphore = iox::posix::Semaphore::create(0u).get_value();

int main(int argc, char* argv[])
{
    // Set OS signal handlers
    signal(SIGINT, ShutdownManager::scheduleShutdown);
    signal(SIGTERM, ShutdownManager::scheduleShutdown);

    // Start application
    iox::runtime::PoshRuntime::getInstance("/gateway_dds2iceoryx");

    iox::dds::DDS2IceoryxGateway<> gw;
    gw.runMultithreaded();

    // Run until SIGINT or SIGTERM
    ShutdownManager::waitUntilShutdown();

    return 0;
}
