//
//  opencl_buffer_data.h
//
//  Created by Jonathan Tompson on 5/12/13.
//
//  Struct to store the OpenCL buffer data.  This is an internal class.  User
//  code should only interact with OpenClBuffer type (which are only handles)
//

#pragma once

#include "jcl/cl_include.h"
#include "jcl/jcl.h"

namespace jcl {

  struct OpenCLBufferData {
  public:
    OpenCLBufferData(const CLBufferType type, const uint32_t w,
      const uint32_t h, const uint32_t d, cl::Context& context);
    ~OpenCLBufferData();

    cl::Buffer buffer;
    const uint32_t width;
    const uint32_t height;
    const uint32_t depth;
    const CLBufferType type;

    OpenCLBufferData& operator=(const OpenCLBufferData&);

  private:
    // Non-copyable, non-assignable.
    OpenCLBufferData(OpenCLBufferData&);
  };

};  // namespace jcl
