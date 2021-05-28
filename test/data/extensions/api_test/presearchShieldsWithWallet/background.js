/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

function testBasics() {
  chrome.test.runTests([
    function presearchShieldsHasWalletAccessButNotSeed() {
      if (chrome.presearchWallet && !chrome.presearchWallet.getWalletSeed &&
          !chrome.presearchWallet.getBitGoSeed &&
          !chrome.presearchWallet.getProjectID &&
          !chrome.presearchWallet.getPresearchKey) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
  ]);
}

function testNoDappCheck() {
  chrome.test.runTests([
    function testNotENabled() {
      chrome.presearchWallet.shouldCheckForDapps((dappDetection) => {
        if (dappDetection) {
          chrome.test.fail();
        } else {
          chrome.test.succeed();
        }
      })
    },
  ]);
}

function testDappCheck() {
  chrome.test.runTests([
    function testNotENabled() {
      chrome.presearchWallet.shouldCheckForDapps((dappDetection) => {
        if (dappDetection) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    },
  ]);
}
