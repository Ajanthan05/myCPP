cmake_minimum_required(VERSION 3.5)
project(ipc_benchmark C CXX)

message("-----------------------------------------------------------------------")
message("IPC Benchmarking for Embedded Applications")
message("-----------------------------------------------------------------------")

set(CMAKE_C_FLAGS "-pthread -std-gnu99")