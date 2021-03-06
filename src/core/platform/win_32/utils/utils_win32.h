#ifndef __UTILS_WIN32_H__
#define __UTILS_WIN32_H__

namespace core {

CORE_API UhRC_t platform_set_thread_affinity( uint64_t logical_cpu );

uint64_t platform_get_process_affinity_mask();
void platform_set_process_affinity_mask( uint64_t mask );
uint64_t platform_get_system_affinity_mask();
void* platform_get_current_process();

} //end namespace core

#endif //__UTILS_WIN32_H__
