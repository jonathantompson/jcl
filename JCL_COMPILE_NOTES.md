Windows:
- Install the Intel SDK (for CPU support), CPU Only runtime from:
    - http://registrationcenter.intel.com/irc_nas/3782/intel_sdk_for_ocl_applications_2013_r3_runtime_x64_setup.msi
- For ATI CARDS: 
    - download AMD APP SDK and install using express settings: (I used version 2.8.1016.5) http://developer.amd.com/tools-and-sdks/heterogeneous-computing/amd-accelerated-parallel-processing-app-sdk/downloads/#one
    - Create windows environment variables (right click My Computer -> Properties -> Advanced System Settings -> Environment Variables -> System Variables -> New)
    - Name = OPENCL_INC_DIR, Value = C:\Program Files (x86)\AMD APP\include
    - Name = OPENCL_LIB_DIR, Value = C:\Program Files (x86)\AMD APP\lib\x86_64
- FOR NVIDIA CARDS: 
    - Download the NVidia GPU Computing SDK (by getting the CUDA SDK) https://developer.nvidia.com/gpu-computing-sdk (Desktop Windows 7 64bit)
    - Create windows environment variables (right click My Computer -> Properties -> Advanced System Settings -> Environment Variables -> System Variables -> New): 
    - Name = OPENCL_INC_DIR, Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include
    - Name = OPENCL_LIB_DIR, Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\lib\x64
    - Now download the hpp header from http://www.khronos.org/registry/cl/api/1.1/cl.hpp and put it in C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include\CL

- Compiling the library:
    - open jcl.sln
    - right click "test_jcl" in the Solution Explorer -> "Set as StartUp Project"
    - From the file menu: DEBUG -> Start Debugging... (or press F5)
    - TWO tests should run and at the end of each test there should be a "PASSED" printout.

MAC OS X
 - Just run cmake and then build (all frameworks should be included).  
 - cl.hpp doesn't exist by default but there is a copy of it in the local directory opencl_cpp_header.


