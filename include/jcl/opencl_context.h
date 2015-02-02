//
//  opencl_context_data.h
//
//  Created by Jonathan Tompson on 5/12/13.
//
//  Struct to store the OpenCL context data and escentially hide the cl.h
//  header from the outside world.  This is an internal class and shouldn't
//  be used directly.
//

#pragma once

#include "jcl/cl_include.h"
#include "jcl/jcl.h"
#include "jcl/opencl_kernel.h"
#include "jcl/opencl_buffer_data.h"
#include "jcl/data_str/hash_map_managed.h"
#include "jcl/data_str/vector_managed.h"
#include "jcl/data_str/vector.h"
#include "jcl/math/math_types.h"  // for jcl::math::Int2 and Int3

#define OPENCL_KERNEL_STARTING_HASH_SIZE 11  // Make it a prime

namespace jcl {

  struct OpenCLProgram;

  struct OpenCLContext {
  public:
    OpenCLContext();
    ~OpenCLContext();

    cl::Context context;
    std::vector<cl::Device> devices;
    std::vector<cl::CommandQueue> queues;
    jcl::data_str::HashMapManaged<std::string, OpenCLProgram*>* programs;  // Stored by filename
    jcl::data_str::HashMapManaged<std::string, OpenCLKernel*>* kernels;  // Stored by (filename + kernel)
    jcl::data_str::VectorManaged<OpenCLBufferData*>* buffers;

    static void CheckError(const cl_int err_code);
    static std::string GetCLErrorString(const cl::Error& err);
    static std::string GetCLErrorEnumString(const cl_int err_code);
    
    static bool queryDeviceExists(const CLDevice device, const CLVendor vendor);

    void createContext(const CLDevice device, const CLVendor vendor);
    void InitDevices(const CLDevice device);
    void createCommandQueues();

    // Enumerate devices once the context is open
    uint32_t getNumDevices();
    std::string getDeviceName(const uint32_t device_index);
    CLDevice getDeviceType(const uint32_t device_index);
    uint32_t getMaxWorkgroupSize(const uint32_t device_index);
    void getMaxWorkitemSizes(const uint32_t device_index,
      jcl::math::Int3& max_device_item_sizes);

    // Memory management
    JCLBuffer allocateBuffer(const CLBufferType type, const uint32_t nelems);
    template <typename T>
    void writeToBuffer(const T* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);
    template <typename T>
    void readFromBuffer(T* data, const uint32_t device_index, 
      const JCLBuffer buffer, const bool blocking);

    // Kernel setup and run
    void useKernel(const char* filename, const char* kernel_name,
      const bool strict_float);
    void useKernelCStr(const char* kernel_c_str, const char* kernel_name,
      const bool strict_float);
    void setArg(const uint32_t index, const JCLBuffer& buf);
    template <typename T>
    void setArg(const uint32_t index, const T& val);
    void setArg(const uint32_t index, const uint32_t size, void* data);

    // Run commands to specify the local workgroup size
    void runKernel1D(const uint32_t device_index, const int global_work_size, 
      const int local_work_size, const bool blocking);
    void runKernel2D(const uint32_t device_index, 
      const jcl::math::Int2& global_work_size, 
      const jcl::math::Int2& local_work_size, const bool blocking);
    void runKernel3D(const uint32_t device_index, 
      const jcl::math::Int3& global_work_size, 
      const jcl::math::Int3& local_work_size, const bool blocking);

    // Run commands to let OpenCL choose the local workgroup size
    void runKernel1D(const uint32_t device_index, const int global_work_size, 
      const bool blocking);
    void runKernel2D(const uint32_t device_index, 
      const jcl::math::Int2& global_work_size, const bool blocking);
    void runKernel3D(const uint32_t device_index, 
      const jcl::math::Int3& global_work_size, const bool blocking);
    void sync(const uint32_t device_index);  // Blocking until queue is empty

    void getOptimalLocalWorkgroupSizes1D(const uint32_t device_index,
      const int global_workgroup, int& local_workgroup);

    // devices_max_workgroup_size is the max possible, each kernel might have
    // a specific maximum.  Use this to grab the max size for the compiled
    // kernel.
    int32_t queryMaxWorkgroupSizeForCurKernel(const uint32_t device_index);

  private:
    OpenCLProgram* cur_program_;
    OpenCLKernel* cur_kernel_;
    jcl::data_str::Vector<int> devices_max_workgroup_size_;
    jcl::data_str::Vector<jcl::math::Int3> devices_max_workitem_size_;

    static bool getPlatform(const CLDevice device, const CLVendor vendor, 
      cl::Platform& return_platform);
    static std::string CLVendor2String(const CLVendor vendor);
    static cl_device_type CLDevice2CLDeviceType(const CLDevice device);
    static CLDevice CLDeviceType2CLDevice(const cl_device_type device);
    static void getPlatformDeviceIDsOfType(cl::Platform& platform, 
      const cl_device_type, std::vector<cl_device_id>& devices);

    // Non-copyable, non-assignable.
    OpenCLContext(OpenCLContext&);
    OpenCLContext& operator=(const OpenCLContext&);
  };

  template <typename T>
  void OpenCLContext::setArg(const uint32_t index, const T& val) {
    if (!cur_kernel_) {
      throw std::runtime_error("OpenCLContext::setArg() - ERROR: You must "
        "call OpenCL::useKernel() first!");
    }
    cur_kernel_->setArg(index, val);
  }

  template <typename T>
  void OpenCLContext::writeToBuffer(const T* data, 
    const uint32_t device_index, const JCLBuffer buffer, 
    const bool blocking) {
#if defined(DEBUG) || defined(_DEBUG)
    if (device_index >= devices.size()) {
      throw std::runtime_error("runKernelxD() - ERROR: Invalid "
        "device_index");
    }
    if ((uint32_t)buffer >= buffers->size()) {
      throw std::runtime_error("runKernelxD() - ERROR: Invalid "
        "buffer");
    }
#endif
    OpenCLBufferData* buf = (*buffers)[(uint32_t)buffer];
    cl::Event cur_event;
    try {
      queues[device_index].enqueueWriteBuffer(buf->buffer, 
        blocking ? CL_TRUE : CL_FALSE, 0, 
        buf->nelems * sizeof(data[0]), data, 
        NULL, &cur_event);
    } catch (cl::Error err) {
      throw std::runtime_error(std::string("enqueueWriteBuffer failed: ") +
        GetCLErrorString(err));
    }
    if (blocking) {
      cur_event.wait();
    }
  }

  template <typename T>
  void OpenCLContext::readFromBuffer(T* data, 
    const uint32_t device_index, const JCLBuffer buffer, 
    const bool blocking) {
#if defined(DEBUG) || defined(_DEBUG)
    if (device_index >= devices.size()) {
      throw std::runtime_error("runKernelxD() - ERROR: Invalid "
        "device_index");
    }
    if ((uint32_t)buffer >= buffers->size()) {
      throw std::runtime_error("runKernelxD() - ERROR: Invalid "
        "buffer");
    }
#endif
    OpenCLBufferData* buf = (*buffers)[(uint32_t)buffer];
    cl::Event cur_event;
    try {
      queues[device_index].enqueueReadBuffer(buf->buffer, 
        blocking ? CL_TRUE : CL_FALSE, 0, 
        buf->nelems * sizeof(data[0]), data, 
        NULL, &cur_event);
    } catch (cl::Error err) {
      throw std::runtime_error(std::string("enqueueReadBuffer failed: ") +
        GetCLErrorString(err));
    }
    if (blocking) {
      cur_event.wait();
    }
  }

};  // namespace jcl
