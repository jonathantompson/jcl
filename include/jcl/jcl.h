//
//  opencl.h
//
//  Created by Jonathan Tompson on 5/12/13.
//
//  Top level header for my OpenCL interface.  Contains functions for
//  creating an OpenCL context, loading kernels, allocating memory, etc.
//  ALL OPENCL CALLS ARE MADE THROUGH THIS CLASS!
//
//  Note that to simplify the memory interface, all OpenCL memory is allocated
//  by openCL (so no flags are used in the opencl buffer call, as opposed to 
//  using user mapped memory).  This means that you will be responsible for 
//  calling writeBuffer and readBuffer to transfer memory manually.  According
//  to the documentation, letting openCL manage memory is faster anyway.
//  

#pragma once

#include <string>
#include <mutex>
#include "jtil/math/math_types.h"

namespace jcl {

  struct OpenCLContext;  // Internal helper class
  typedef uint32_t JCLBuffer;  // Unique id into OpenCLContext class array

  typedef enum {
    CLDeviceDefault,
    CLDeviceAll,
    CLDeviceCPU,
    CLDeviceGPU,
    CLDeviceAccelerator
  } CLDevice;

  typedef enum {
    CLVendorAny,
    CLDeviceNVidia,
    CLDeviceAMD,
    CLDeviceIntel,
  } CLVendor;

  typedef enum {
    CLBufferTypeReadWrite,
    CLBufferTypeWrite,
    CLBufferTypeRead
  } CLBufferType;

  class JCL {
  public:
    // Initialization  and Shutdown
    JCL(const CLDevice device = CLDeviceCPU, 
      const CLVendor vendor = CLVendorAny, 
      const bool strict_float = true);  // Set to false for more speed
    ~JCL();  // Call destructor to shutdown the context
    
    // Query if a device of a certain type exists (otherwise constructor throws
    // exceptions!)
    static bool queryDeviceExists(const CLDevice device, const CLVendor vendor);

    uint32_t getNumDevices();
    std::string getDeviceName(const uint32_t device_index);
    CLDevice getDeviceType(const uint32_t device_index);
    uint32_t getMaxWorkgroupSize(const uint32_t device_index);
    void getMaxWorkitemSizes(const uint32_t device_index,
      jtil::math::Int3& max_device_item_sizes);

    // allocateBuffer - Maps a new OpenCL buffer to the user memory at data
    JCLBuffer allocateBuffer(const CLBufferType type, const uint32_t w, 
      const uint32_t h = 1, const uint32_t d = 1);

    // Read and Write a buffer to the specific device
    // --> Unfortunately, this cannot be templatized since it would expose 
    //  OpenCLContext to the outside world (templates need to be in the header)
    void writeToBuffer(const float* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void readFromBuffer(float* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void writeToBuffer(const int* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void readFromBuffer(int* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void writeToBuffer(const int16_t* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void readFromBuffer(int16_t* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void writeToBuffer(const uint8_t* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    void readFromBuffer(uint8_t* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);

    // Set the current Kernel
    void useKernel(const char* filename, const char* kernel_name);  // USE THIS VERSION
    void useKernelCStr(const char* kernel_c_str, const char* kernel_name);  // SLOWER WHEN SWITCHING

    // Set an argument for the current kernel.
    // --> Unfortunately, this cannot be templatized since it would expose 
    //  OpenCLContext to the outside world (templates need to be in the header)
    void setArg(const uint32_t index, JCLBuffer buf);
    void setArg(const uint32_t index, int val);
    void setArg(const uint32_t index, float val);
    void setArg(const uint32_t index, const uint32_t size, void* data);

    // Enqueue the current Kernel
    // global_work_size - Size of the array to work on (width * height)
    // local_work_size - Local work group size (a good number is 32, but try many)
    // Run commands to specify the local workgroup size:
    void runKernel1D(const uint32_t device_index, const int global_work_size, 
      const int local_work_size, const bool blocking);
    void runKernel2D(const uint32_t device_index, 
      const jtil::math::Int2& global_work_size, 
      const jtil::math::Int2& local_work_size, const bool blocking);
    void runKernel3D(const uint32_t device_index, 
      const jtil::math::Int3& global_work_size, 
      const jtil::math::Int3& local_work_size, const bool blocking);
    // Run commands to let OpenCL choose the local workgroup size:
    void runKernel1D(const uint32_t device_index, const int global_work_size, 
      const bool blocking);
    void runKernel2D(const uint32_t device_index, 
      const jtil::math::Int2& global_work_size, const bool blocking);
    void runKernel3D(const uint32_t device_index, 
      const jtil::math::Int3& global_work_size, const bool blocking);
    void sync(const uint32_t device_index);  // Blocking until queue is empty

    void getOptimalLocalWorkgroupSizes1D(const uint32_t device_index,
      const int global_workgroup, int& local_workgroup);

    // devices_max_workgroup_size is the max possible, each kernel might have
    // a specific maximum.  Use this to grab the max size for the compiled
    // kernel.
    int32_t queryMaxWorkgroupSizeForCurKernel(const uint32_t device_index);

  private:
    bool strict_float_;
    CLDevice device_;
    CLVendor vendor_;
    OpenCLContext* context_;
    static std::mutex context_lock_;  // For creating contexts

    // Non-copyable, non-assignable.
    JCL(JCL&);
    JCL& operator=(const JCL&);
  };

};  // namespace jcl
