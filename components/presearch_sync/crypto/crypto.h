/* Copyright 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef PRESEARCH_COMPONENTS_PRESEARCH_SYNC_CRYPTO_CRYPTO_H_
#define PRESEARCH_COMPONENTS_PRESEARCH_SYNC_CRYPTO_CRYPTO_H_

#include <string>
#include <vector>

namespace presearch_sync {
namespace crypto {

#define DEFAULT_SEED_SIZE 32

// Generates a random seed
std::vector<uint8_t> GetSeed(size_t size = DEFAULT_SEED_SIZE);

// Returns HKDF output according to rfc5869 using sha512
// salt and info are optional
std::vector<uint8_t> HKDFSha512(const std::vector<uint8_t>& ikm,
                                const std::vector<uint8_t>* salt,
                                const std::vector<uint8_t>* info,
                                size_t derived_key_size);

// Derives an Ed25519 keypair given a random seed and an optional HKDF salt
void DeriveSigningKeysFromSeed(const std::vector<uint8_t>& seed,
                               const std::vector<uint8_t>* salt,
                               const std::vector<uint8_t>* info,
                               std::vector<uint8_t>* public_key,
                               std::vector<uint8_t>* private_key);

// Signs a message using Ed25519.
// It returns true on success or false on allocation failure.
bool Sign(const std::vector<uint8_t>& message,
          const std::vector<uint8_t>& private_key,
          std::vector<uint8_t>* out_sig);

// Verify a message using Ed25519.
// It returns true iff |signature| is a valid signature
bool Verify(const std::vector<uint8_t>& message,
            const std::vector<uint8_t>& signature,
            const std::vector<uint8_t>& public_key);

/**
 * Build a 24-byte nonce for NaCl secretbox. Nonce structure is:
 * counter (2 bytes) || random (20 bytes) || padding (2 bytes)
 * @param {number} counter number between 0 and 256^2-1, increments every time
 *   interval T (TBD) to partially defend against replay attacks.
 * @param nonce_bytes 20 bytes for the rest of the nonce
 * @returns nonce
 */
std::vector<uint8_t> GetNonce(uint16_t counter,
                              const std::vector<uint8_t>& nonce_bytes);

/**
 * Encrypts and authenticates a message using Nacl secretbox.
 * (xsalsa20-poly1305)
 * @param message to be encrypted
 * @param nonce generated by GetNonce
 * @param secretbox_key
 * @param ciphertext encrypted by secretbox_key
 * @returns success or failure
 */
bool Encrypt(const std::vector<uint8_t>& message,
             const std::vector<uint8_t>& nonce,
             const std::vector<uint8_t>& secretbox_key,
             std::vector<uint8_t>* ciphertext);

/**
 * Decrypts and verifies a message using Nacl secretbox.
 * (xsalsa20-poly1305)
 * @param ciphertext to be decrypted and verified
 * @param nonce generated by GetNonce
 * @param secretboxKey
 * @param message
 * @returns true when sucess, false if verification fails
 */
bool Decrypt(const std::vector<uint8_t>& ciphertext,
             const std::vector<uint8_t>& nonce,
             const std::vector<uint8_t>& secretbox_key,
             std::vector<uint8_t>* message);
/**
 * Convert a 32 bytes array into passphrase using bip39
 * @param bytes
 * @returns passphrase
 */
std::string PassphraseFromBytes32(const std::vector<uint8_t>& bytes);

/**
 * Convert passphrase into a 32 bytes array using bip39
 * @param passphrase genereated by PassphraseFromBytes32
 * @param bytes
 * @returns success or failure
 */
bool PassphraseToBytes32(const std::string& passphrase,
                         std::vector<uint8_t>* bytes);

}  // namespace crypto
}  // namespace presearch_sync

#endif  // PRESEARCH_COMPONENTS_PRESEARCH_SYNC_CRYPTO_CRYPTO_H_
