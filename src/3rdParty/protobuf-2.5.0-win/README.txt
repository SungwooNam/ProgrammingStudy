
This zip archive contains headers and pre-built C++ libraries for 
building against protobuf 2.5.0 on Windows using Microsoft Visual 
Studio 2012 (MSVC 11.0) and Microsoft Visual Studio 2013 (MSVC 12.0). 

BUILD NOTES
-----------

* The VS 2012 libs are built from unchanged protobuf 2.5.0 source files. 
  However, a minor patch needed to be applied to 2.5.0 for the
  VS 2013 build due to a compiler error (see protobuf revision 512,
  "Add missing #include of <algorithm> for min/max on VS2013" at 
  https://code.google.com/p/protobuf/source/detail?r=512 for details). 
  
* The libraries were built using default platform toolsets that do not
  support Windows XP (that is, the v110_xp and v120_xp toolsets were 
  not used to produce these libraries).
  
  See http://msdn.microsoft.com/en-us/library/vstudio/jj851139.aspx
  for more information on Visual Studio platform toolsets.

NOTICES
-------

* ScaleOut Software, Inc. is not responsible for, nor does it grant 
  any licenses to, this software package. This package may include 
  dependencies which are governed by additional licenses. Visit the 
  protobuf project page at https://code.google.com/p/protobuf/ to 
  determine any additional dependencies.

* THIS BUILD OF PROTOCOL BUFFERS IS PROVIDED BY SCALEOUT SOFTWARE, INC 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SCALEOUT SOFTWARE 
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF BINARIES IN THIS ARCHIVE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

* This archive includes software developed by the Protocol Buffers
  project: http://code.google.com/apis/protocolbuffers

  Copyright 2008, Google Inc.
  All rights reserved.

  See the included PROTOBUF_LICENSE.txt for a list of disclaimers and 
  conditions.