#include "base/command_line.h"
#include "presearch/common/presearch_switches.h"

// switches::kDisableMachineId
const char kDisableMachineId[] = "disable-machine-id";

namespace {
bool IsMachineIdDisabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kDisableMachineId);
}

}  // namespace
#include "../../../../components/metrics/machine_id_provider_win.cc"
