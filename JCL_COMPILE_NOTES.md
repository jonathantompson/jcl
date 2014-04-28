1 OpenCL:
1.1 Windows:
 - Install the Intel SDK (for CPU support):
   Install the CPU Only runtime.
   http://registrationcenter.intel.com/irc_nas/3782/intel_sdk_for_ocl_applications_2013_r3_runtime_x64_setup.msi
 - On ATI: Download AMD APP SDK and install using express settings: 
   (I used version 2.8.1016.5)
   http://developer.amd.com/tools-and-sdks/heterogeneous-computing/amd-accelerated-parallel-processing-app-sdk/downloads/#one
   Create a windows environment variable (right click My Computer -> Properties -> 
   Advanced System Settings -> Environment Variables -> System Variables -> New): 
   Name = OPENCL_INC_DIR
   Value = C:\Program Files (x86)\AMD APP\include
   Name = OPENCL_LIB_DIR
   Value = C:\Program Files (x86)\AMD APP\lib\x86_64
 - On NVIDIA: Download the NVidia GPU Computing SDK (by getting the CUDA SDK)
   https://developer.nvidia.com/gpu-computing-sdk (Desktop Windows 7 64bit)
   Create a windows environment variable (right click My Computer -> Properties -> 
   Advanced System Settings -> Environment Variables -> System Variables -> New): 
   Name = OPENCL_INC_DIR
   Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include
   Name = OPENCL_LIB_DIR
   Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\lib\x64
    
   Now download the hpp header from http://www.khronos.org/registry/cl/api/1.1/cl.hpp
   and put it in C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include\CL

1.2 MAC OS X
 - Just run cmake and then build (all frameworks should be included).  cl.hpp doesn't 
   exist by default but there is a copy of it in the local directory opencl_cpp_header.
