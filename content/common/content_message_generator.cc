// Get basic type definitions.
#define IPC_MESSAGE_IMPL

#include "presearch/content/common/content_message_generator.h"

// Generate constructors.
#include "ipc/struct_constructor_macros.h"
#include "presearch/content/common/content_message_generator.h"

// Generate param traits write methods.
#include "ipc/param_traits_write_macros.h"
namespace IPC {
#include "presearch/content/common/content_message_generator.h"
}  // namespace IPC

// Generate param traits read methods.
#include "ipc/param_traits_read_macros.h"
namespace IPC {
#include "presearch/content/common/content_message_generator.h"
}  // namespace IPC

// Generate param traits log methods.
#include "ipc/param_traits_log_macros.h"
namespace IPC {
#include "presearch/content/common/content_message_generator.h"
}  // namespace IPC
