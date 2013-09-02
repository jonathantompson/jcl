#include <iostream>
#include "jcl/cl_include.h"
#include "jcl/jcl.h"
#include "jcl/opencl_context.h"
#include "jtil/exceptions/wruntime_error.h"

#define SAFE_DELETE(x) if (x != NULL) { delete x; x = NULL; }
#define SAFE_DELETE_ARR(x) if (x != NULL) { delete[] x; x = NULL; }

using std::wruntime_error;
using std::string;
using std::cout;
using std::endl;
using namespace jtil::math;

namespace jcl {

  std::mutex JCL::context_lock_;

  JCL::JCL(const CLDevice device, const CLVendor vendor, 
    const bool strict_float) {
    context_ = NULL;
    device_ = device;
    vendor_ = vendor;
    strict_float_ = strict_float;

    std::cout << "Creating OpenCL Context..." << std::endl;

    // Aquire lock to prevent multiple initilizations:
    std::lock_guard<std::mutex> lock(context_lock_);

    context_ = new OpenCLContext();
    context_->createContext(device, vendor);
    context_->InitDevices(device);
    context_->createCommandQueues();  // For each device
  }

  JCL::~JCL() {
    std::cout << "Shutting down OpenCL Context..." << std::endl;
    SAFE_DELETE(context_);
  }
  
  bool JCL::queryDeviceExists(const CLDevice device, const CLVendor vendor) {
    // Aquire lock to prevent multiple initilizations:
    std::lock_guard<std::mutex> lock(context_lock_);
    return OpenCLContext::queryDeviceExists(device, vendor);
  }

  uint32_t JCL::getNumDevices() {
    return context_->getNumDevices();
  }

  std::string JCL::getDeviceName(const uint32_t device_index) {
    return context_->getDeviceName(device_index);
  }

  uint32_t JCL::getMaxWorkgroupSize(const uint32_t device_index) {
    return context_->getMaxWorkgroupSize(device_index);
  }

  void JCL::getMaxWorkitemSizes(const uint32_t device_index, 
    jtil::math::Int3& max_device_item_sizes) {
    context_->getMaxWorkitemSizes(device_index, max_device_item_sizes);
  }

  CLDevice JCL::getDeviceType(const uint32_t device_index) {
    return context_->getDeviceType(device_index);
  }

  JCLBuffer JCL::allocateBuffer(const CLBufferType type,
    const uint32_t w, const uint32_t h, const uint32_t d) {
    return context_->allocateBuffer(type, w, h, d);
  }

  void JCL::writeToBuffer(const float* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->writeToBuffer<float>(data, device_index, buffer, blocking);
  }

  void JCL::readFromBuffer(float* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->readFromBuffer<float>(data, device_index, buffer, blocking);
  }

  void JCL::writeToBuffer(const int* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->writeToBuffer<int>(data, device_index, buffer, blocking);
  }

  void JCL::readFromBuffer(int* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->readFromBuffer<int>(data, device_index, buffer, blocking);
  }

  void JCL::writeToBuffer(const int16_t* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->writeToBuffer<int16_t>(data, device_index, buffer, blocking);
  }

  void JCL::readFromBuffer(int16_t* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->readFromBuffer<int16_t>(data, device_index, buffer, blocking);
  }

  void JCL::writeToBuffer(const uint8_t* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->writeToBuffer<uint8_t>(data, device_index, buffer, blocking);
  }

  void JCL::readFromBuffer(uint8_t* data, const uint32_t device_index, 
    const JCLBuffer buffer, const bool blocking) {
    context_->readFromBuffer<uint8_t>(data, device_index, buffer, blocking);
  }

  void JCL::useKernel(const char* filename, const char* kernel_name) {
    context_->useKernel(filename, kernel_name, strict_float_);
  }
  
  void JCL::useKernelCStr(const char* kernel_c_str, const char* kernel_name) {
    context_->useKernelCStr(kernel_c_str, kernel_name, strict_float_);
  }

  void JCL::setArg(const uint32_t index, JCLBuffer buf) {
    context_->setArg(index, buf);
  }

  void JCL::setArg(const uint32_t index, int val) {
    context_->setArg(index, val);
  }

  void JCL::setArg(const uint32_t index, float val) {
    context_->setArg(index, val);
  }

  void JCL::setArg(const uint32_t index, const uint32_t size, void* data) {
    context_->setArg(index, size, data);
  }

  void JCL::runKernel1D(const uint32_t device_index, 
    const int global_work_size, const int local_work_size, 
    const bool blocking) {
    context_->runKernel1D(device_index, global_work_size, local_work_size, 
      blocking);
  }

  void JCL::runKernel2D(const uint32_t device_index, 
    const Int2& global_work_size, const Int2& local_work_size, 
    const bool blocking) {
    context_->runKernel2D(device_index, global_work_size, local_work_size, 
      blocking);
  }

  void JCL::runKernel3D(const uint32_t device_index, 
    const Int3& global_work_size, const Int3& local_work_size, 
    const bool blocking) {
    context_->runKernel3D(device_index, global_work_size, local_work_size, 
      blocking);
  }

  void JCL::sync(const uint32_t device_index) {
    context_->sync(device_index);
  }

  void JCL::runKernel1D(const uint32_t device_index, 
    const int global_work_size, const bool blocking) {
    context_->runKernel1D(device_index, global_work_size, blocking);
  }

  void JCL::runKernel2D(const uint32_t device_index, 
    const Int2& global_work_size, const bool blocking) {
    context_->runKernel2D(device_index, global_work_size, blocking);
  }

  void JCL::runKernel3D(const uint32_t device_index, 
    const Int3& global_work_size, const bool blocking) {
    context_->runKernel3D(device_index, global_work_size, blocking);
  }

  void JCL::getOptimalLocalWorkgroupSizes1D(const uint32_t device_index,
    const int global_workgroup, int& local_workgroup) {
    context_->getOptimalLocalWorkgroupSizes1D(device_index, global_workgroup,
      local_workgroup);
  }

  int32_t JCL::queryMaxWorkgroupSizeForCurKernel(const uint32_t device_index) {
    return context_->queryMaxWorkgroupSizeForCurKernel(device_index);
  }

}  // namespace jcl
