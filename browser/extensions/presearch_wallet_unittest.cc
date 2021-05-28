/* Copyright (c) 2019 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/components/presearch_wallet/browser/presearch_wallet_service.h"

#include "presearch/components/presearch_wallet/browser/pref_names.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "extensions/buildflags/buildflags.h"
#include "testing/gtest/include/gtest/gtest.h"

class PresearchWalletUnitTest : public testing::Test {
 public:
  PresearchWalletUnitTest()
      : testing_profile_manager_(TestingBrowserProcess::GetGlobal()) {
  }
  ~PresearchWalletUnitTest() override {}

 protected:
  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    ASSERT_TRUE(testing_profile_manager_.SetUp(temp_dir_.GetPath()));
  }

  PrefService* GetPrefs() {
    return ProfileManager::GetActiveUserProfile()->GetPrefs();
  }

  content::BrowserTaskEnvironment task_environment_;
  TestingProfileManager testing_profile_manager_;
  base::ScopedTempDir temp_dir_;
};

TEST_F(PresearchWalletUnitTest, TestGetRandomNonce) {
  std::string nonce = PresearchWalletService::GetRandomNonce();
  ASSERT_EQ(nonce.size(), PresearchWalletService::kNonceByteLength);
}

TEST_F(PresearchWalletUnitTest, TestGetRandomSeed) {
  std::string seed = PresearchWalletService::GetRandomSeed();
  ASSERT_EQ(seed.size(), PresearchWalletService::kSeedByteLength);
}

TEST_F(PresearchWalletUnitTest, TestGetEthereumRemoteClientSeedFromRootSeed) {
  const char seed[32] = {
    48, 196, 56, 174, 243, 75, 120, 235,
    37, 174, 254, 97, 37, 205, 101, 93,
    181, 23, 190, 82, 53, 180, 51, 198,
    232, 187, 188, 220, 160, 187, 212, 28
  };
  const char expected_derived_seed[32] = {
    142, 147, 10, 180, 36, 89, 142, 110,
    52, 85, 216, 222, 83, 56, 38, 206,
    104, 133, 77, 246, 219, 90, 105, 35,
    52, 76, 223, 24, 183, 138, 244, 72
  };
  std::string derived =
    PresearchWalletService::GetEthereumRemoteClientSeedFromRootSeed(
        std::string(seed, base::size(seed)));
  ASSERT_EQ(derived, std::string(expected_derived_seed,
      base::size(expected_derived_seed)));
}

TEST_F(PresearchWalletUnitTest, TestBitGoSeedFromRootSeed) {
  const char seed[32] = {
    48, 196, 56, 174, 243, 75, 120, 235,
    37, 174, 254, 97, 37, 205, 101, 93,
    181, 23, 190, 82, 53, 180, 51, 198,
    232, 187, 188, 220, 160, 187, 212, 28
  };
  const char expected_derived_seed[32] = {
    101, 6, 89, 61, 129, 81, 104, 13,
    48, 59, 117, 46, 73, 177, 168, 248,
    91, 84, 145, 54, 61, 157, 27, 254,
    45, 203, 71, 123, 188, 29, 224, 203
  };
  std::string derived =
    PresearchWalletService::GetBitGoSeedFromRootSeed(
        std::string(seed, base::size(seed)));
  ASSERT_EQ(derived, std::string(expected_derived_seed,
      base::size(expected_derived_seed)));
}

TEST_F(PresearchWalletUnitTest, TestSealSeed) {
  const char seed[32] = {
    48, 196, 56, 174, 243, 75, 120, 235,
    37, 174, 254, 97, 37, 205, 101, 93,
    181, 23, 190, 82, 53, 180, 51, 198,
    232, 187, 188, 220, 160, 187, 212, 28
  };
  const char key[32] = {
    196, 34, 104, 152, 91, 63, 78, 171,
    234, 163, 25, 221, 80, 73, 158, 89,
    52, 53, 227, 231, 152, 214, 61, 210,
    33, 54, 68, 171, 140, 239, 3, 158
  };
  const char nonce[12] = {
    200, 153, 224, 40, 58, 249, 156, 33, 152, 207, 177, 12
  };
  const char expected_cipher_seed[48] = {
    33, 11, 185, 125, 67, 27, 92, 110,
    132, 238, 255, 8, 79, 7, 8, 40,
    189, 211, 35, 122, 236, 183, 66, 212,
    213, 68, 187, 103, 16, 138, 166, 0,
    6, 128, 179, 64, 55, 160, 219, 8,
    222, 231, 48, 93, 132, 131, 178, 177
  };
  std::string cipher_seed;
  ASSERT_TRUE(PresearchWalletService::SealSeed(
     std::string(seed, base::size(seed)), std::string(key, base::size(key)),
     std::string(nonce, base::size(nonce)), &cipher_seed));
  ASSERT_EQ(cipher_seed, std::string(expected_cipher_seed,
      base::size(expected_cipher_seed)));
}

TEST_F(PresearchWalletUnitTest, TestOpenSeed) {
  const char cipher_seed[48] = {
    33, 11, 185, 125, 67, 27, 92, 110,
    132, 238, 255, 8, 79, 7, 8, 40,
    189, 211, 35, 122, 236, 183, 66, 212,
    213, 68, 187, 103, 16, 138, 166, 0,
    6, 128, 179, 64, 55, 160, 219, 8,
    222, 231, 48, 93, 132, 131, 178, 177
  };
  const char key[32] = {
    196, 34, 104, 152, 91, 63, 78, 171,
    234, 163, 25, 221, 80, 73, 158, 89,
    52, 53, 227, 231, 152, 214, 61, 210,
    33, 54, 68, 171, 140, 239, 3, 158
  };
  const char nonce[12] = {
    200, 153, 224, 40, 58, 249, 156, 33, 152, 207, 177, 12
  };
  const char expected_seed[32] = {
    48, 196, 56, 174, 243, 75, 120, 235,
    37, 174, 254, 97, 37, 205, 101, 93,
    181, 23, 190, 82, 53, 180, 51, 198,
    232, 187, 188, 220, 160, 187, 212, 28
  };
  std::string seed;
  ASSERT_TRUE(PresearchWalletService::OpenSeed(
       std::string(cipher_seed, base::size(cipher_seed)),
       std::string(key, base::size(key)),
       std::string(nonce, base::size(nonce)), &seed));
  ASSERT_EQ(seed, std::string(expected_seed, 32));
}

TEST_F(PresearchWalletUnitTest, TestLoadFromPrefs) {
  GetPrefs()->SetString(kPresearchWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kPresearchWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");

  std::string cipher_seed;
  std::string nonce;
  ASSERT_TRUE(PresearchWalletService::LoadFromPrefs(
      ProfileManager::GetActiveUserProfile()->GetPrefs(),
      &cipher_seed, &nonce));

  const char expected_nonce[12] = {
    200, 153, 224, 40, 58, 249, 156, 33, 152, 207, 177, 12
  };
  const char expected_cipher_seed[48] = {
    33, 11, 185, 125, 67, 27, 92, 110,
    132, 238, 255, 8, 79, 7, 8, 40,
    189, 211, 35, 122, 236, 183, 66, 212,
    213, 68, 187, 103, 16, 138, 166, 0,
    6, 128, 179, 64, 55, 160, 219, 8,
    222, 231, 48, 93, 132, 131, 178, 177
  };
  ASSERT_EQ(nonce, std::string(expected_nonce, base::size(expected_nonce)));
  ASSERT_EQ(std::string(expected_cipher_seed, base::size(expected_cipher_seed)),
      cipher_seed);
}

TEST_F(PresearchWalletUnitTest, TestSaveToPrefs) {
  const char nonce[12] = {
    200, 153, 224, 40, 58, 249, 156, 33, 152, 207, 177, 12
  };
  const char cipher_seed[48] = {
    33, 11, 185, 125, 67, 27, 92, 110,
    132, 238, 255, 8, 79, 7, 8, 40,
    189, 211, 35, 122, 236, 183, 66, 212,
    213, 68, 187, 103, 16, 138, 166, 0,
    6, 128, 179, 64, 55, 160, 219, 8,
    222, 231, 48, 93, 132, 131, 178, 177
  };
  PresearchWalletService::SaveToPrefs(
      ProfileManager::GetActiveUserProfile()->GetPrefs(),
      std::string(cipher_seed, base::size(cipher_seed)),
      std::string(nonce, base::size(nonce)));

  ASSERT_EQ(GetPrefs()->GetString(kPresearchWalletAES256GCMSivNonce),
      "yJngKDr5nCGYz7EM");
  ASSERT_EQ(GetPrefs()->GetString(kPresearchWalletEncryptedSeed),
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
}
