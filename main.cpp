//
// Created by aoweichen on 2025/5/27.
//
#define GLOG_USE_GLOG_EXPORT
#include <glog/logging.h>
#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "This is an info message";
    return 0;
}
