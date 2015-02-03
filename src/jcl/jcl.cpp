#include <iostream>
#include "jcl/cl_include.h"
#include "jcl/jcl.h"
#include "jcl/opencl_context.h"

#define SAFE_DELETE(x) if (x != NULL) { delete x; x = NULL; }
#define SAFE_DELETE_ARR(x) if (x != NULL) { delete[] x; x = NULL; }

using std::runtime_error;
using std::string;
using std::cout;
using std::endl;

namespace jcl {

  std::mutex JCL::context_lock_;

  JCL::JCL(const CLDevice device, const CLVendor vendor, 
    const bool strict_float) {
    context_ = NULL;
    device_ = device;
    vendor_ = vendor;
    strict_float_ = strict_float;

    std::cout << "\tCreating OpenCL Context..." << std::endl;

    // Aquire lock to prevent multiple initilizations:
    std::lock_guard<std::mutex> lock(context_lock_);

    context_ = new OpenCLContext();
    context_->createContext(device, vendor);
    context_->InitDevices(device);
    context_->createCommandQueues();  // For each device
  }

  JCL::~JCL() {
    std::cout << "\tShutting down OpenCL Context..." << std::endl;
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

  uint32_t JCL::getMaxWorkitemSize(const uint32_t device_index, const uint32_t dim) {
    return context_->getMaxWorkitemSize(device_index, dim);
  }

  CLDevice JCL::getDeviceType(const uint32_t device_index) {
    return context_->getDeviceType(device_index);
  }

  JCLBuffer JCL::allocateBuffer(const CLBufferType type, 
    const uint32_t nelems) {
    return context_->allocateBuffer(type, nelems);
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

  void JCL::runKernel(const uint32_t device_index, 
    const uint32_t dim,  const uint32_t* global_work_size, 
    const uint32_t* local_work_size, const bool blocking) {
    context_->runKernel(device_index, dim, global_work_size, 
      local_work_size, blocking);
  }

  void JCL::sync(const uint32_t device_index) {
    context_->sync(device_index);
  }

  void JCL::runKernel(const uint32_t device_index, 
    const uint32_t dim, const uint32_t* global_work_size, 
    const bool blocking) {
    context_->runKernel(device_index, dim, global_work_size, blocking);
  }

  uint32_t JCL::queryMaxWorkgroupSizeForCurKernel(const uint32_t device_index) {
    return context_->queryMaxWorkgroupSizeForCurKernel(device_index);
  }

  std::string JCL::CLDeviceToString(const CLDevice device) {
    switch (device) {
    case CLDeviceDefault:
      return "CLDeviceDefault";
    case CLDeviceAll:
      return "CLDeviceAll";
    case CLDeviceCPU:
      return "CLDeviceCPU";
    case CLDeviceGPU:
      return "CLDeviceGPU";
    case CLDeviceAccelerator:
      return "CLDeviceDefault";
    default:
      throw std::runtime_error("Bad CLDevice");
    }
  }

  std::string JCL::CLVendorToString(const CLVendor vendor) {
    switch (vendor) {
    case CLVendorAny:
      return "CLVendorAny";
    case CLDeviceNVidia:
      return "CLDeviceNVidia";
    case CLDeviceAMD:
      return "CLDeviceAMD";
    case CLDeviceIntel:
      return "CLDeviceIntel";
    default:
      throw std::runtime_error("Bad CLVendor");
    }
  }

}  // namespace jcl
