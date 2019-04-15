gen_rule(
    name = 'gen_hello_proto_grpc',
    srcs = [
        'hello.proto',
    ],
    outs = [
        'hello.grpc.pb.cc',
        'hello.grpc.pb.h',
        'hello.pb.cc',
        'hello.pb.h',
    ],
    cmd = './tools/grpc/gen-proto-grpc $FIRST_SRC'
)

cc_library(
    name = 'hello_proto_grpc',
    srcs = [
        'hello.grpc.pb.cc',
        'hello.pb.cc',
    ],
    deps = [
        ':gen_hello_proto_grpc',
        '//thirdparty/grpc:grpc',
    ]
)
