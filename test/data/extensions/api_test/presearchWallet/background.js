/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

function arrayBufsMatch(ab1, ab2) {
  const dataView1 = new DataView(ab1)
  const dataView2 = new DataView(ab2)
  console.log('dataView1 byte length: ', dataView1.byteLength)
  console.log('dataView2 byte length: ', dataView2.byteLength)
  if (dataView1.byteLength === 32 &&
      dataView1.byteLength === dataView2.byteLength) {
    for (let i = 0; i < dataView1.byteLength; i++) {
      if (dataView1.getUint8(i) !== dataView2.getUint8(i)) {
        console.log('Mismatch at position i: ', i)
        return false;
      }
    }
  }
  return true;
}

function testKnownSeedValuesEndToEnd() {
  chrome.test.runTests([
    function testKnownSeedValues() {
      const buf = new Uint8Array([
        196, 34, 104, 152, 91, 63, 78, 171,
        234, 163, 25, 221, 80, 73, 158, 89,
        52, 53, 227, 231, 152, 214, 61, 210,
        33, 54, 68, 171, 140, 239, 3, 158])
      const expectedResult = new Uint8Array([
        142, 147, 10, 180, 36, 89, 142, 110,
        52, 85, 216, 222, 83, 56, 38, 206,
        104, 133, 77, 246, 219, 90, 105, 35,
        52, 76, 223, 24, 183, 138, 244, 72])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const expectedResultArrayBuf =
          expectedResult.buffer.slice(expectedResult.byteOffset,
            expectedResult.byteLength + expectedResult.byteOffset)
      chrome.presearchWallet.getWalletSeed(buf, (resultArrayBuf) => {
        if (arrayBufsMatch(resultArrayBuf, expectedResultArrayBuf)) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testKnownBitGoSeedValuesEndToEnd() {
  chrome.test.runTests([
    function testKnownBitGoSeedValues() {
      const buf = new Uint8Array([
        196, 34, 104, 152, 91, 63, 78, 171,
        234, 163, 25, 221, 80, 73, 158, 89,
        52, 53, 227, 231, 152, 214, 61, 210,
        33, 54, 68, 171, 140, 239, 3, 158])
      const expectedResult = new Uint8Array([
        101, 6, 89, 61, 129, 81, 104, 13,
        48, 59, 117, 46, 73, 177, 168, 248,
        91, 84, 145, 54, 61, 157, 27, 254,
        45, 203, 71, 123, 188, 29, 224, 203])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const expectedResultArrayBuf =
          expectedResult.buffer.slice(expectedResult.byteOffset,
            expectedResult.byteLength + expectedResult.byteOffset)
      chrome.presearchWallet.getBitGoSeed(buf, (resultArrayBuf) => {
        if (arrayBufsMatch(resultArrayBuf, expectedResultArrayBuf)) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsCryptoWallets() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.presearchWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsMetaMask() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.presearchWallet.getWeb3Provider((provider) => {
        if (provider === 'nkbihfbeogaeaoehlefnkodbefgpgknn') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsAsk() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.presearchWallet.getWeb3Provider((provider) => {
        if (provider.length === 0) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsNone() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.presearchWallet.getWeb3Provider((provider) => {
        if (provider.length === 0) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsPresearchWallet() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.presearchWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testBasics() {
  chrome.test.runTests([
    function presearchWalletExtensionHasAccess() {
      if (chrome.presearchWallet &&
          chrome.presearchWallet.shouldPromptForSetup &&
          chrome.presearchWallet.loadUI &&
          chrome.presearchWallet.promptToEnableWallet) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function presearchWalletExtensionSeedFunctionHasAccess() {
      if (chrome.presearchWallet && chrome.presearchWallet.getWalletSeed &&
          chrome.presearchWallet.getBitGoSeed &&
          chrome.presearchWallet.getProjectID &&
          chrome.presearchWallet.getPresearchKey &&
          chrome.presearchWallet.getWeb3Provider) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function presearchWalletWrongInputKeySizeFails() {
      const buf = new Uint8Array([...Array(3).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.presearchWallet.getWalletSeed(arraybuf, (seed) => {
        if (!seed) {
          chrome.test.succeed();
          return
        }
        chrome.test.fail();
      })
    },
    function presearchBitGoWrongInputKeySizeFails() {
      const buf = new Uint8Array([...Array(3).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.presearchWallet.getBitGoSeed(arraybuf, (seed) => {
        if (!seed) {
          chrome.test.succeed();
          return
        }
        chrome.test.fail();
      })
    },
    function presearchWalletExtensionGetWalletSeedSameSeedOnMultipleRequests() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.presearchWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.presearchWallet.getWalletSeed(arraybuf, (secondSeed) => {
          if (arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds differ across calls!')
          chrome.test.fail();
        })
      })
    },
    function presearchWalletExtensionGetBitGoSeedSameSeedOnMultipleRequests() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.presearchWallet.getBitGoSeed(arraybuf, (firstSeed) => {
        chrome.presearchWallet.getBitGoSeed(arraybuf, (secondSeed) => {
          if (arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds differ across calls!')
          chrome.test.fail();
        })
      })
    },
    function presearchWalletExtensionSeedsDiffValues() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.presearchWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.presearchWallet.getBitGoSeed(arraybuf, (secondSeed) => {
          if (!arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds are the same but should not be!')
          chrome.test.fail();
        })
      })
    },
    function presearchWalletExtensionGetWalletSeedDifferentKeysDoesNotWork() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const buf2 = new Uint8Array([11, ...Array(31).keys()])
      const arraybuf2 = buf2.buffer.slice(buf2.byteOffset, buf2.byteLength + buf2.byteOffset)
      chrome.presearchWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.presearchWallet.getWalletSeed(arraybuf2, (secondSeed) => {
          if (firstSeed && !secondSeed) {
            chrome.test.succeed();
            return
          }
          chrome.test.fail();
        })
      })
    },
    function presearchWalletExtensionGetBitGoSeedDifferentKeysDoesNotWork() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const buf2 = new Uint8Array([11, ...Array(31).keys()])
      const arraybuf2 = buf2.buffer.slice(buf2.byteOffset, buf2.byteLength + buf2.byteOffset)
      chrome.presearchWallet.getBitGoSeed(arraybuf, (firstSeed) => {
        chrome.presearchWallet.getBitGoSeed(arraybuf2, (secondSeed) => {
          if (firstSeed && !secondSeed) {
            chrome.test.succeed();
            return
          }
          chrome.test.fail();
        })
      })
    },
    function presearchWalletExtensionGetProjectIDWorks() {
      chrome.presearchWallet.getProjectID((projectID) => {
        if (projectID === "test-project-id") {
          chrome.test.succeed();
          return
        }
        console.log('Failed project ID is: ' + projectID)
        chrome.test.fail();
      })
    },
    function presearchWalletExtensionGetPresearchKeyWorks() {
      chrome.presearchWallet.getPresearchKey((key) => {
        if (key === "test-presearch-key") {
          chrome.test.succeed();
          return
        }
        console.log('Failure, presearch key is: ' + key)
        chrome.test.fail();
      })
    }
  ]);
}
