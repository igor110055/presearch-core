/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "base/logging.h"
#include "base/test/task_environment.h"
#include "presearch/components/sync/driver/presearch_sync_profile_sync_service.h"
#include "presearch/components/sync/driver/profile_sync_service_delegate.h"
#include "components/os_crypt/os_crypt_mocker.h"
#include "components/sync/driver/data_type_manager_impl.h"
#include "components/sync/driver/fake_data_type_controller.h"
#include "components/sync/driver/fake_sync_api_component_factory.h"
#include "components/sync/driver/profile_sync_service_bundle.h"
#include "components/sync/test/engine/fake_sync_engine.h"
#include "components/sync/test/engine/fake_sync_manager.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

using testing::_;
using testing::ByMove;
using testing::Return;

namespace syncer {

namespace {

const char kValidSyncCode[] =
    "fringe digital begin feed equal output proof cheap "
    "exotic ill sure question trial squirrel glove celery "
    "awkward push jelly logic broccoli almost grocery drift";

}  // namespace

class ProfileSyncServiceDelegateMock : public ProfileSyncServiceDelegate {
 public:
  ProfileSyncServiceDelegateMock() {}
  ~ProfileSyncServiceDelegateMock() override {}
  void SuspendDeviceObserverForOwnReset() override {}
  void ResumeDeviceObserver() override {}
};

class PresearchProfileSyncServiceTest : public testing::Test {
 public:
  PresearchProfileSyncServiceTest()
      : presearch_sync_prefs_(profile_sync_service_bundle_.pref_service()),
        sync_prefs_(profile_sync_service_bundle_.pref_service()) {
    profile_sync_service_bundle_.identity_test_env()
        ->SetAutomaticIssueOfAccessTokens(true);
    presearch_sync::Prefs::RegisterProfilePrefs(
        profile_sync_service_bundle_.pref_service()->registry());
  }

  ~PresearchProfileSyncServiceTest() override { sync_service_->Shutdown(); }

  void CreateSyncService(
      ProfileSyncService::StartBehavior start_behavior,
      ModelTypeSet registered_types = ModelTypeSet(BOOKMARKS)) {
    DataTypeController::TypeVector controllers;
    for (ModelType type : registered_types) {
      controllers.push_back(std::make_unique<FakeDataTypeController>(type));
    }

    std::unique_ptr<SyncClientMock> sync_client =
        profile_sync_service_bundle_.CreateSyncClientMock();
    ON_CALL(*sync_client, CreateDataTypeControllers(_))
        .WillByDefault(Return(ByMove(std::move(controllers))));

    sync_service_ = std::make_unique<PresearchProfileSyncService>(
        profile_sync_service_bundle_.CreateBasicInitParams(
            start_behavior, std::move(sync_client)),
        std::make_unique<ProfileSyncServiceDelegateMock>());
  }

  presearch_sync::Prefs* presearch_sync_prefs() { return &presearch_sync_prefs_; }

  SyncPrefs* sync_prefs() { return &sync_prefs_; }

  PresearchProfileSyncService* presearch_sync_service() { return sync_service_.get(); }

  FakeSyncApiComponentFactory* component_factory() {
    return profile_sync_service_bundle_.component_factory();
  }

  FakeSyncEngine* engine() {
    return component_factory()->last_created_engine();
  }

 private:
  content::BrowserTaskEnvironment task_environment_;
  ProfileSyncServiceBundle profile_sync_service_bundle_;
  presearch_sync::Prefs presearch_sync_prefs_;
  SyncPrefs sync_prefs_;
  std::unique_ptr<PresearchProfileSyncService> sync_service_;
};

TEST_F(PresearchProfileSyncServiceTest, ValidPassphrase) {
  OSCryptMocker::SetUp();

  CreateSyncService(ProfileSyncService::MANUAL_START);

  presearch_sync_service()->Initialize();
  EXPECT_FALSE(engine());

  bool set_code_result = presearch_sync_service()->SetSyncCode(kValidSyncCode);
  EXPECT_TRUE(set_code_result);

  EXPECT_EQ(presearch_sync_prefs()->GetSeed(), kValidSyncCode);

  OSCryptMocker::TearDown();
}

TEST_F(PresearchProfileSyncServiceTest, InvalidPassphrase) {
  OSCryptMocker::SetUp();

  CreateSyncService(ProfileSyncService::MANUAL_START);

  presearch_sync_service()->Initialize();
  EXPECT_FALSE(engine());

  bool set_code_result =
      presearch_sync_service()->SetSyncCode("word one and then two");
  EXPECT_FALSE(set_code_result);

  EXPECT_EQ(presearch_sync_prefs()->GetSeed(), "");

  OSCryptMocker::TearDown();
}

TEST_F(PresearchProfileSyncServiceTest, ValidPassphraseLeadingTrailingWhitespace) {
  OSCryptMocker::SetUp();

  CreateSyncService(ProfileSyncService::MANUAL_START);

  presearch_sync_service()->Initialize();
  EXPECT_FALSE(engine());

  std::string sync_code_extra_whitespace =
      std::string(" \t\n") + kValidSyncCode + std::string(" \t\n");
  bool set_code_result =
      presearch_sync_service()->SetSyncCode(sync_code_extra_whitespace);
  EXPECT_TRUE(set_code_result);

  EXPECT_EQ(presearch_sync_prefs()->GetSeed(), kValidSyncCode);

  OSCryptMocker::TearDown();
}

}  // namespace syncer
