#include "helloworld/proto/hello.grpc.pb.h"
#include "helloworld/proto/hello.pb.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::proto::HelloRequest;
using helloworld::proto::HelloReply;
using helloworld::proto::Greeter;

namespace {

class GreeterClient {
public:
  explicit GreeterClient(std::shared_ptr<Channel> channel) 
    : greeterStub(Greeter::NewStub(channel)) {}

  void SayHello(const std::string& user) {
    HelloRequest request;
    request.set_name(user);
    AsyncClientCall* call = new AsyncClientCall;
    call->response_reader = greeterStub->PrepareAsyncSayHello(
      &call->context,
      request, &completionQueue
    );
    call->response_reader->StartCall();
    call->response_reader->Finish(&call->reply, &call->status, (void*)call);
  }

  void AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;
    while (completionQueue.Next(&got_tag, &ok)) {
      AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
      GPR_ASSERT(ok);
      if (call->status.ok()) {
        std::cout 
          << "Greeter received: " << call->reply.message() 
          << std::endl;
      } else {
        std::cout 
          << "RPC failed"
          << std::endl;
      }
      delete call;
    }
  }

private:
  struct AsyncClientCall {
    HelloReply reply;
    ClientContext context;
    Status status;
    std::unique_ptr<ClientAsyncResponseReader<HelloReply>> response_reader;
  };

  std::unique_ptr<Greeter::Stub> greeterStub;

  CompletionQueue completionQueue;
};

} // anonymous namespace

int main(int argc, char** argv) {
  GreeterClient greeter(
    grpc::CreateChannel(
      "localhost:50051",
      grpc::InsecureChannelCredentials()
    )
  );
  std::thread thread = std::thread(&GreeterClient::AsyncCompleteRpc, &greeter);
  for (int i = 0; i < 100; i++) {
    std::string user("world " + std::to_string(i));
    greeter.SayHello(user);
  }
  std::cout << "Press control-c to quit" << std::endl << std::endl;
  thread.join();
  return 0;
}
