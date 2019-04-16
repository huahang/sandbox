#include <helloworld/proto/hello.grpc.pb.h>
#include <helloworld/proto/hello.pb.h>

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace {

class GreeterServer final {
 public:
  virtual ~GreeterServer() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    completionQueue->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run() {
    using grpc::ServerBuilder;

    std::string server_address("0.0.0.0:50051");

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    completionQueue = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    // Proceed to the server's main loop.
    HandleRpcs();
  }

 private:
  class CallData {
   public:
    CallData(
      helloworld::proto::Greeter::AsyncService* s, 
      grpc::ServerCompletionQueue* queue
    ) : 
      service(s), 
      completionQueue(queue), 
      responseWriter(&serverContext), 
      callStatus(CREATE) {
      Proceed();
    }

    void Proceed() {
      if (callStatus == CREATE) {
        // Make this instance progress to the PROCESS state.
        callStatus = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        service->RequestSayHello(
          &serverContext,
          &helloRequest,
          &responseWriter,
          completionQueue,
          completionQueue,
          this
        );
      } else if (callStatus == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        new CallData(service, completionQueue);

        // The actual processing.
        std::string prefix("Hello ");
        reply_.set_message(prefix + helloRequest.name());

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        callStatus = FINISH;
        responseWriter.Finish(reply_, grpc::Status::OK, this);
      } else {
        GPR_ASSERT(callStatus == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

   private:
    helloworld::proto::Greeter::AsyncService* service;
    grpc::ServerCompletionQueue* completionQueue;
    grpc::ServerContext serverContext;

    // What we get from the client.
    helloworld::proto::HelloRequest helloRequest;
    // What we send back to the client.
    helloworld::proto::HelloReply reply_;

    grpc::ServerAsyncResponseWriter<
      helloworld::proto::HelloReply
    > responseWriter;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus callStatus;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service, completionQueue.get());
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(completionQueue->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::unique_ptr<grpc::ServerCompletionQueue> completionQueue;
  helloworld::proto::Greeter::AsyncService service;
  std::unique_ptr<grpc::Server> server_;
};

} // anonymous namespace

int main(int argc, char** argv) {
  GreeterServer server;
  server.Run();
  return 0;
}
