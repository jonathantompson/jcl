#include <sstream>
#include <iostream>
#include "jcl/opencl_context.h"
#include "jcl/opencl_buffer_data.h"
#include "jtil/exceptions/wruntime_error.h"

#define SAFE_DELETE(x) if (x != NULL) { delete x; x = NULL; }
#define SAFE_FREE(x) if (x != NULL) { free(x); x = NULL; }
#define SAFE_DELETE_ARR(x) if (x != NULL) { delete[] x; x = NULL; }

using namespace jtil::data_str;
using std::wruntime_error;
using std::cout;
using std::endl;

namespace jcl {

  OpenCLBufferData::OpenCLBufferData(const CLBufferType type, const uint32_t w,
    const uint32_t h, const uint32_t d, cl::Context& context) : width(w), 
    height(h), depth(d), type(type){
    //cl_mem_flags flags = CL_MEM_USE_HOST_PTR;
    cl_mem_flags flags = 0;
    switch (type) {
    case CLBufferTypeRead:
      flags |= CL_MEM_READ_ONLY;
      break;
    case CLBufferTypeWrite:
      flags |= CL_MEM_WRITE_ONLY;
      break;
    case CLBufferTypeReadWrite:
      flags |= CL_MEM_READ_WRITE;
      break;
    default:
      throw std::wruntime_error("OpenCLBufferData::OpenCLBufferData() - "
        "ERROR: Memory type not supported!");
    }
    buffer = cl::Buffer(context, flags, sizeof(cl_float) * w * h * d);
  }

  OpenCLBufferData::~OpenCLBufferData() {
    // Nothing to do
  }

}  // namespace jcl
