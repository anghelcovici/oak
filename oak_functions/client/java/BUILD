#
# Copyright 2021 The Project Oak Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

load("@rules_java//java:defs.bzl", "java_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

java_library(
    name = "client",
    srcs = [
        "src/com/google/oak/functions/client/AttestationClient.java",
    ],
    runtime_deps = [
        "@io_grpc_grpc_java//netty",
    ],
    deps = [
        "//oak_functions/proto:invocation_java_proto",
        "//oak_functions/proto:invocation_proto",
        "//oak_functions/proto:server_java_grpc",
        "//oak_functions/proto:server_java_proto",
        "//oak_functions/proto:server_proto",
        "//remote_attestation/java:remote_attestation",
        "@com_google_guava_guava",
        "@com_google_protobuf//:protobuf_java",
        "@com_google_protobuf//:protobuf_java_util",
        "@io_grpc_grpc_java//api",
        "@io_grpc_grpc_java//protobuf",
        "@io_grpc_grpc_java//stub",
    ],
)

java_library(
    name = "client_tink",
    srcs = [
        "src/com/google/oak/functions/client/AttestationClient.java",
    ],
    runtime_deps = [
        "@io_grpc_grpc_java//netty",
    ],
    deps = [
        "//oak_functions/proto:invocation_java_proto",
        "//oak_functions/proto:invocation_proto",
        "//oak_functions/proto:server_java_grpc",
        "//oak_functions/proto:server_java_proto",
        "//oak_functions/proto:server_proto",
        "//remote_attestation/java:remote_attestation_tink",
        "@com_google_guava_guava",
        "@com_google_protobuf//:protobuf_java",
        "@com_google_protobuf//:protobuf_java_util",
        "@io_grpc_grpc_java//api",
        "@io_grpc_grpc_java//protobuf",
        "@io_grpc_grpc_java//stub",
    ],
)
