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

For notes on installing the dependencies for jcl, see here: JCL\_COMPILE\_NOTES.txt.

**Style**
---------

This project follows the Google C++ style conventions: 

<http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml>
