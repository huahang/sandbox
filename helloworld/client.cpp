#include <helloworld/proto/hello.grpc.pb.h>
#include <helloworld/proto/hello.pb.h>

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace {

class GreeterClient {
public:
  explicit GreeterClient(std::shared_ptr<grpc::Channel> channel) 
    : greeterStub(helloworld::proto::Greeter::NewStub(channel)) {}

  void SayHello(const std::string& user) {
    helloworld::proto::HelloRequest request;
    request.set_name(user);
    AsyncClientCall* call = new AsyncClientCall;
    call->responseReader = greeterStub->PrepareAsyncSayHello(
      &call->context,
      request, &completionQueue
    );
    call->responseReader->StartCall();
    call->responseReader->Finish(&call->reply, &call->status, (void*)call);
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
    helloworld::proto::HelloReply reply;
    grpc::ClientContext context;
    grpc::Status status;
    std::unique_ptr<
      grpc::ClientAsyncResponseReader<helloworld::proto::HelloReply>
    > responseReader;
  };

  std::unique_ptr<helloworld::proto::Greeter::Stub> greeterStub;

  grpc::CompletionQueue completionQueue;
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
