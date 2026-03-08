# Install script for directory: /Users/jkitzmann/FrequencyAnalysis

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimeLibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jkitzmann/FrequencyAnalysis/build/bin/FrequencyAnalysis")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis"
         OLD_RPATH "/dev/shm/gpjohnsn/spack-stage-itk-5.1.2-j3za4tc2x6mvauinmzdf7jt4pqivm7qf/spack-build-j3za4tc/lib:/Users/jkitzmann/FrequencyAnalysis/build/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libjpeg-turbo-2.0.6-3gqaynm/lib64:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libtiff-4.1.0-7bsdm4q/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libpng-1.6.37-ngfonf4/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/expat-2.2.10-v4nyrpx/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/zlib-1.2.11-ikqdhnh/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FrequencyAnalysis")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimeLibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/jkitzmann/FrequencyAnalysis/build/lib/libFrequencyAnalysisLib.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so"
         OLD_RPATH "/dev/shm/gpjohnsn/spack-stage-itk-5.1.2-j3za4tc2x6mvauinmzdf7jt4pqivm7qf/spack-build-j3za4tc/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/zlib-1.2.11-ikqdhnh/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/expat-2.2.10-v4nyrpx/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libjpeg-turbo-2.0.6-3gqaynm/lib64:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libtiff-4.1.0-7bsdm4q/lib:/opt/ssoft/apps/2021.1/linux-centos7-broadwell/gcc-9.3.0/libpng-1.6.37-ngfonf4/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFrequencyAnalysisLib.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimeLibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRuntimeLibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/Users/jkitzmann/FrequencyAnalysis/FrequencyAnalysis.xml")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/jkitzmann/FrequencyAnalysis/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
