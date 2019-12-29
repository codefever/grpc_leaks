#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

DEFINE_uint64(times_for_once, 100000, "");
DEFINE_string(address, "localhost:50051", "");

void LeakCheck() {
  auto channel = grpc::CreateChannel(
      FLAGS_address, grpc::InsecureChannelCredentials());
  CHECK(channel);

  std::string dummy;
  while (std::getline(std::cin, dummy)) {
    for (size_t i = 0; i < FLAGS_times_for_once; ++i) {
      auto stub = Greeter::NewStub(channel);
      CHECK(stub);
    }
    LOG(INFO) << "Paused";
  }
}

int main(int argc, char** argv) {
  FLAGS_logtostderr = true;
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  LeakCheck();
  return 0;
}
