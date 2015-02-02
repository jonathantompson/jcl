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
    OpenCLBufferData(const CLBufferType type, const uint32_t nelems,
      cl::Context& context);
    ~OpenCLBufferData();

    cl::Buffer buffer;
    const uint32_t nelems;  // ie width * height * feats
    const CLBufferType type;

    OpenCLBufferData& operator=(const OpenCLBufferData&);

  private:
    // Non-copyable, non-assignable.
    OpenCLBufferData(OpenCLBufferData&);
  };

};  // namespace jcl
