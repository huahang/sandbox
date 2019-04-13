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
    cmd = 'protoc --grpc_out=./blade-bin --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $FIRST_SRC'
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
        '#grpc',
        '#grpc++',
        '#protobuf',
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
