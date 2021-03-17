/*
 * Copyright 2021 The Project Oak Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "oak/common/pkcs8.h"

#include <cstring>

#include "absl/strings/escaping.h"
#include "glog/logging.h"

namespace oak {

namespace pkcs8 {

// Template of the form |prefix|middle| for serializing and deserializing Ed25519 keys as
// unencrypted PKCS#8 byte arrays. With this template, the 32-byte
// private key (or seed) will be placed after the `prefix`, and the 32-byte public key will be
// inserted at the end, resulting in |prefix|private-key|middle|public-key|. With this template,
// no additional attributes can be included in the encoded byte array. This template should be used
// in combination with `kEd25519Pkcs8Template`, which specifies the location of the split between
// `prefix` and `middle`.
//
// This is copied from the Rust ring crate
// https://github.com/briansmith/ring/blob/main/src/ec/curve25519/ed25519/ed25519_pkcs8_v2_template.der.
const char kBase64Ed25519Pkcs8Template[] = "MFMCAQEwBQYDK2VwBCIEIKEjAyEA";

// Template for serializing and deserializing Ed25519 keys. The template encapsulates a string
// representation of the template bytes, the version (for convenience), and the index at which the
// template bytes should split into `prefix` and `middle`.
const Template kEd25519Pkcs8Template = template_from_base64(kBase64Ed25519Pkcs8Template, 1, 16, 32);

// Splits the bytes in the given template at the `private_key_index` into `|prefix|middle|`. Inserts
// the private key and the public key to form `|prefix|private-key|middle|public-key|`.
std::string to_pkcs8(const PrivateKeyInfo& data, const Template& pkcs8_template) {
  if (pkcs8_template.private_key_len != data.private_key.length()) {
    LOG(FATAL) << "The length of the given private key does not match the length expected in the "
                  "template.";
  }

  std::stringstream bytes;

  // copy `prefix` from template to bytes: |prefix|
  bytes << pkcs8_template.bytes.substr(0, pkcs8_template.private_key_index);

  // copy private key to bytes: |prefix|private-key|
  bytes << data.private_key;

  // copy `middle` from template to bytes: |prefix|private-key|middle|
  int middle_len = pkcs8_template.bytes.length() - pkcs8_template.private_key_index;
  bytes << pkcs8_template.bytes.substr(pkcs8_template.private_key_index, middle_len);

  // copy public key to bytes: |prefix|private-key|middle|public-key|
  bytes << data.public_key;

  return bytes.str();
}

// Splits the bytes in the given template at the `private_key_index` into `|e_prefix|e_middle|`.
// Then using the template, splits the input PKCS#8-encoded string at `private_key_index` and
// `private_key_index + private_key_len` into |a_prefix|private-key|a_middle|public-key|. If
// `e_prefix` and `a_prefix` match, and `e_middle` and `a_middle` match, returns the private and
// public keys in a PrivateKeyInfo object.
PrivateKeyInfo from_pkcs8(const std::string& pkcs_str, const Template& pkcs8_template) {
  // Template `prefix` check
  if (pkcs_str.substr(0, pkcs8_template.private_key_index) !=
      pkcs8_template.bytes.substr(0, pkcs8_template.private_key_index)) {
    LOG(FATAL) << "PKCS#8 template prefix mismatch.";
  }

  // Template `middle` check
  int middle_ind = pkcs8_template.private_key_index + pkcs8_template.private_key_len;
  int middle_len = pkcs8_template.bytes.length() - pkcs8_template.private_key_index;
  if (pkcs_str.substr(middle_ind, middle_len) !=
      pkcs8_template.bytes.substr(pkcs8_template.private_key_index, middle_len)) {
    LOG(FATAL) << "PKCS#8 template middle mismatch.";
  }

  // Extract private key and public key values
  int public_key_index = pkcs8_template.bytes.length() + pkcs8_template.private_key_len;
  int public_key_len = pkcs_str.length() - public_key_index;
  PrivateKeyInfo p{
      pkcs_str.substr(pkcs8_template.private_key_index, pkcs8_template.private_key_len),
      pkcs_str.substr(public_key_index, public_key_len)};
  return p;
}

Template template_from_base64(const std::string& base64_template, int version,
                              int private_key_index, unsigned int private_key_len) {
  std::string decoded_template;
  if (!absl::Base64Unescape(base64_template, &decoded_template)) {
    LOG(FATAL) << "Couldn't decode base64 template.";
  }
  return Template{decoded_template, version, private_key_index, private_key_len};
}

}  // namespace pkcs8

}  // namespace oak
