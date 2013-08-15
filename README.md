Pagespeed for Apache Traffic Server
====================================
_This plugin is highly experimental and is still very much under development._ 

** THIS README IS INCOMPLETE, I WILL FINISH IT IN THE NEXT FEW DAYS **

Building
-------------------------------------
Dependencies to Resolve:
pcre-devel
yaml-cpp
atscppapi
zlib-devel


0. Download and Install atscppapi
ats_pagespeed is built on top of atscppapi, which is an opensource c++ api wrapper for Apache Traffic Server developed by LinkedIn.

    // INSTRUCTIONS HERE   

1. Install yaml-cpp-0.3.0

Typically this can be done via
    
    sudo yum install yaml-cpp-0.3.0 yaml-cpp-devel-0.3.0

If not download the 0.3.0 source from and follow these commands     
    
    mkdir build
    cd build
    cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON ..
    make
    sudo make install
    
2. Build Pagespeed Libraries
    
    cd lib/
    
2.1 Install Depot Tools
    
    cd lib & \
    svn co http://src.chromium.org/svn/trunk/tools/depot_tools && \
    export PATH=$PATH:`pwd`/depot_tools && \
    cd -
        
2.2 Build Pagespeed

More information on the following steps can be found at https://developers.google.com/speed/docs/mod_pagespeed/build_from_source.
From within the lib/ folder
    
    cd lib && \
    gclient config http://modpagespeed.googlecode.com/svn/branches/27/src ; \
    gclient sync --force --jobs=1

You will now have a src/ folder created under lib with the 27 branch version of mod_pagespeed.

Next, build mod_pagespeed, although we are not using mod_pagespeed in order to get the pagespeed
sdk you must build mod_pagespeed.
 
    make V=1 -j -C lib/src/net/instaweb/automatic/ BUILDTYPE=Release &&
    make V=1 -j -C lib/src/ BUILDTYPE=Release
    
To build in debug mode:
For some reason with -O0 it causes stat64 to be undefined at runtime.
    cd lib
    tar xzvf psol.src.tar.gz 
    export CCFLAGS="-O1" && \
    export CFLAGS="-O1" && \
    export CXXFLAGS="-O1" && \
    make V=1 -j -C lib/src/net/instaweb/automatic/ BUILDTYPE=Debug && \
    make V=1 -j -C lib/src/ BUILDTYPE=Debug
    
3. Build ats_pagespeed

From within the root ats_pagespeed directory, run the following:

    autoreconf -i
    ./configure
    make -j V=1

License
---------------------
Copyright (c) 2013 LinkedIn Corp. All rights reserved. 
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the license at
http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the
License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied.    
