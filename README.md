**jcl - Jonathan's OpenCL Wrapper**
---------
---------

**Overview**
--------

This library is a C++ wrapper of OpenCL.  I built it to make prototyping OpenCL applications faster and to create a constant and simple interface for building and running OpenCL kernels.  OpenCL is amazing but I tend to use a very limited subset of OpenCL's heterogeneous compute features; so this library makes a few simplifying assumptions about memory and kernel usage to make creating an OpenCL context easy (i.e, a one liner).

**Compilation**
---------------

Building jcl uses Visual Studio 2012 on Windows, and cmake + gcc 4.7 (or greater) on Mac OS X.  

You will also need a graphics card or CPU driver that supports OpenCL.

For notes on installing the dependencies for jcl:

### Windows:
- **Intel SDK (for CPU support)**
    - Download and install the CPU Only runtime from: http://registrationcenter.intel.com/irc_nas/3782/intel_sdk_for_ocl_applications_2013_r3_runtime_x64_setup.msi
- **For ATI CARDS:**
    - Download AMD APP SDK and install using express settings: (I used version 2.8.1016.5) http://developer.amd.com/tools-and-sdks/heterogeneous-computing/amd-accelerated-parallel-processing-app-sdk/downloads/#one
    - Create windows environment variables (right click My Computer -> Properties -> Advanced System Settings -> Environment Variables -> System Variables -> New)
    - Name = `OPENCL_INC_DIR`, Value = C:\Program Files (x86)\AMD APP\include
    - Name = `OPENCL_LIB_DIR`, Value = C:\Program Files (x86)\AMD APP\lib\x86_64
- **FOR NVIDIA CARDS:**
    - Download the NVidia GPU Computing SDK (by getting the CUDA SDK) https://developer.nvidia.com/gpu-computing-sdk (Desktop Windows 7 64bit)
    - Create windows environment variables (right click My Computer -> Properties -> Advanced System Settings -> Environment Variables -> System Variables -> New): 
    - Name = `OPENCL_INC_DIR`, Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include
    - Name = `OPENCL_LIB_DIR`, Value = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\lib\x64
    - Now download the hpp header from http://www.khronos.org/registry/cl/api/1.1/cl.hpp and put it in C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\include\CL
- **Compiling the library:**
    - open jcl.sln
    - right click "test_jcl" in the Solution Explorer -> "Set as StartUp Project"
    - From the file menu: DEBUG -> Start Debugging... (or press F5)
    - TWO tests should run and at the end of each test there should be a "PASSED" printout.

### MAC OS X:
 - Just run cmake and then build (all frameworks should be included).  
 - cl.hpp doesn't exist by default but there is a copy of it in the local directory opencl_cpp_header.

**Style**
---------

This project follows the Google C++ style conventions: 

<http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml>

**License**
-----------
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
