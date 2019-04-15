proto_library(
    name = 'hello_proto',
    srcs = 'hello.proto'
)

gen_rule(
    name = 'gen_hello_proto_grpc',
    srcs = [
        'hello.proto',
    ],
    outs = [
        'hello.grpc.pb.cc',
        'hello.grpc.pb.h',
    ],
    cmd = 'protoc --grpc_out=./blade-bin --plugin=protoc-gen-grpc=/usr/local/opt/grpc/bin/grpc_cpp_plugin $FIRST_SRC'
)

cc_library(
    name = 'hello_proto_grpc',
    srcs = [
        'hello.grpc.pb.cc',
        'hello.grpc.pb.h',
    ],
    deps = [
        ':gen_hello_proto_grpc',
        ':hello_proto',
        '#protobuf',
        '#address_sorting',
        '#gpr',
        '#grpc',
        '#grpc++',
        '#grpc++_error_details',
        '#grpcpp_channelz',
        '#grpc++_reflection',
        '#grpc_unsecure',
        '#grpc++_unsecure',
        '#z',
        '#cares',
    ]
)

cc_binary(
    name = 'server',
    srcs = [
        'server.cpp',
    ],
    deps = [
        ':hello_proto_grpc',
    ]
)

cc_binary(
    name = 'client',
    srcs = [
        'client.cpp',
    ],
    deps = [
        ':hello_proto_grpc',
    ]
)
