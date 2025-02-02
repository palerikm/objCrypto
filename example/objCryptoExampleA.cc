// SPDX-FileCopyrightText: Copyright (c) 2022 The ObjCrypto Project Authors
// SPDX-License-Identifier: BSD-2-Clause

#include <objCrypto/objCrypto.h>

#include <cassert>
#include <iostream>

using namespace ObjCrypto;

extern "C" void printAPublicKey();
extern "C" void doStuffWithCharBuffer(char* , int);


int main(/*int argc, char* argv[]*/) {
  // Set up cryptor object to keep track of keys
  ObjCryptor cryptor;

  // Create a key
  KeyID keyId = 5;
  // Key size need to match the chosen algorithm
  Key128 key128 = {0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C,
                   0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08};
  KeyInfo keyInfo(ObjCryptoAlg::AES_128_GCM_64, key128);
  // tag size needs to match chosen algorithm
  std::vector<uint8_t> tag(64 / 8);

  // Add the key to the cryptor
  auto err = cryptor.addKey(keyId, keyInfo);
  assert(err == Error::None);

  // Form the nonce, data to encrypt, and extra data to authenticate
  Nonce nonce = {0xCA, 0xFE, 0xBA, 0xBE, 0xFA, 0xCE,
                 0xDB, 0xAD, 0xDE, 0xCA, 0xF8, 0x88};
  std::vector<uint8_t> plainTextIn = {
      0xD9, 0x31, 0x32, 0x25, 0xF8, 0x84, 0x06, 0xE5, 0xA5, 0x59, 0x09, 0xC5,
      0xAF, 0xF5, 0x26, 0x9A, 0x86, 0xA7, 0xA9, 0x53, 0x15, 0x34, 0xF7, 0xDA,
      0x2E, 0x4C, 0x30, 0x3D, 0x8A, 0x31, 0x8A, 0x72, 0x1C, 0x3C, 0x0C, 0x95,
      0x95, 0x68, 0x09, 0x53, 0x2F, 0xCF, 0x0E, 0x24, 0x49, 0xA6, 0xB5, 0x25,
      0xB1, 0x6A, 0xED, 0xF5, 0xAA, 0x0D, 0xE6, 0x57, 0xBA, 0x63, 0x7B, 0x39};
  std::vector<uint8_t> authData = {0xFE, 0xED, 0xFA, 0xCE, 0xDE, 0xAD, 0xBE,
                                   0xEF, 0xFE, 0xED, 0xFA, 0xCE, 0xDE, 0xAD,
                                   0xBE, 0xEF, 0xAB, 0xAD, 0xDA, 0xD2};

  // encrypt plain text and create authentication tag
  std::vector<uint8_t> cipherText(plainTextIn.size());
  err = cryptor.seal(keyId, nonce, plainTextIn, authData, tag, cipherText);
  assert(err == Error::None);

  // decrypt the cipher text and check the authentication tag
  std::vector<uint8_t> plainTextOut(plainTextIn.size());
  err = cryptor.unseal(keyId, nonce, cipherText, authData, tag, plainTextOut);
  assert(err != Error::DecryptAuthFail);
  assert(err == Error::None);
  printAPublicKey();


  char buf[34] = "HeiHvorDetGaarHerTilGards";
  doStuffWithCharBuffer(buf, 34);
  printf("Back in c land the buffer is now: %s\n", buf);
  return 0;
}
