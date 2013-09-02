**jcl - Jonathan's OpenCL Wrapper**
---------
---------

**Overview**
--------

This library is a C++ wrapper of OpenCL.  I built it to make prototyping OpenCL applications faster and to create a constant and simple interface for building and running OpenCL kernels.  OpenCL is amazing but I tend to use a very limited subset of OpenCL's hetrogeneous compute features; so this library makes a few simpifying assumptions about memory and kernel usage to make creating an OpenCL context easy (i.e, a one liner).

**Compilation**
---------------

Building jcl uses Visual Studio 2012 on Windows, and cmake + gcc 4.7 (or greater) on Mac OS X.  The only real dependancy is the jtil library.  See <https://github.com/jonathantompson/jtil> for more details.  You will also need a graphics card or CPU driver that supports OpenCL (some notes about this are here: JCL\_COMPILE\_NOTES.txt).

VS2012 and cmake expect a specific directory structure:

- \\include\\WIN\\
- \\include\\MAC\_OS\_X\\
- \\lib\\WIN\\
- \\lib\\MAC\_OS\_X\\
- \\jtil\\
- \\jcl\\

So the dependancy headers and static libraries (.lib on Windows and .a on Mac OS X) are separated by OS and exist in directories at the same level as jtil and jcl.  I have pre-compiled the dependencies and put them in dropbox, let me know if you need the link.

**Style**
---------

This project follows the Google C++ style conventions: 

<http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml>
