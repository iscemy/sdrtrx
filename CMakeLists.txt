cmake_minimum_required(VERSION 3.0.0)
project(modem VERSION 0.1.0 LANGUAGES C CXX)

# option(COMPILE_TO_PC "compile for pluto" ON)

include(CTest)
enable_testing()

add_executable(modem src/main.cpp 
            src/cpp/Math/ComplexArray/ComplexArray.cpp
            src/cpp/Math/FFTW/FFTW.cpp
            src/cpp/RadioInterfaces/TestRadioFile/TestRadioFile.cpp
            src/cpp/CarrierRecovery/FFTFreqCoarse/FFTFreqRecovery.cpp
            src/cpp/CarrierRecovery/Preprocess/SquarePreprocess.cpp
            src/cpp/Math/Convolve/BasicConvolve/BasicConvolve.cpp
            src/cpp/FilterDesign/remez/remez.c
            src/cpp/CarrierRecovery/PLLPhaseFreq/DualPll/DuallPll.cpp
            src/cpp/Modulator/BasicModulator/BasicModulator.cpp
            src/cpp/Filter/BasicFilter/BasicFilter.cpp
            src/cpp/Samplers/SamplerBrute/SamplerBrute.cpp
            src/cpp/AGC/BasicAGC/BasicAGC.cpp
            src/cpp/Receivers/BPSKReceiver/BPSKReceiver.cpp
            src/cpp/Transmitters/BPSKTransmitter/BPSKTransmitter.cpp
            src/cpp/RadioInterfaces/TestRadioUDP/TestRadioUDP.cpp
            )
            
include_directories("src/include")
target_compile_options(modem PUBLIC -Wall -Wextra -Wpedantic -Werror )
target_compile_options(modem PUBLIC -O0)


# link_directories("/usr/lib//x86_64-linux-gnu/")
# link_directories("/usr/lib/")
# target_link_libraries(modem fftw3f fftw3 profiler)
# target_link_libraries(modem fftw3f fftw3)
target_link_libraries(modem fftw3)

# set(CMAKE_CXX_COMPILER /home/cem/projects/sdr_modem/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
# set(CMAKE_C_COMPILER /home/cem/projects/sdr_modem/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
# set (CMAKE_SYSROOT "/home/cem/projects/sdr_modem/sysroot-v0.32/staging")

if(COMPILE_TO_PC)


else()

endif()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)