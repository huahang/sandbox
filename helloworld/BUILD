cc_binary(
  name = 'helloworld_basic',
  srcs = [
    'basic.cpp',
  ]
)

cc_test(
  name = 'helloworld_test',
  srcs = [
    'test.cpp',
  ]
)

cc_binary(
    name = 'helloworld_server',
    srcs = [
        'server.cpp',
    ],
    deps = [
        './proto:hello_proto_grpc',
    ]
)

cc_binary(
    name = 'helloworld_client',
    srcs = [
        'client.cpp',
    ],
    deps = [
        './proto:hello_proto_grpc',
    ]
)
