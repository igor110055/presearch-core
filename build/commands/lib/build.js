const config = require('../lib/config')
const util = require('../lib/util')
const path = require('path')
const fs = require('fs-extra')

const updateFileUTimesIfOverrideIsNewer = (original, override) => {
  if (fs.statSync(override).mtimeMs - fs.statSync(original).mtimeMs > 0) {
    const date = new Date()
    fs.utimesSync(original, date, date)
    console.log(original + ' is touched.')
  }
}

const getAdditionalGenLocation = () => {
  if (config.targetOS === 'android') {
    if (config.targetArch === 'arm64') {
      return 'android_clang_arm'
    } else if (config.targetArch === 'x64') {
      return 'android_clang_x86'
    }
  } else if ((process.platform === 'darwin' || process.platform === 'linux') && config.targetArch === 'arm64') {
    return 'clang_x64_v8_arm64'
  }
  return undefined
}

const touchOverriddenFiles = () => {
  console.log('touch original files overridden by chromium_src...')

  // Return true when original file of |file| should be touched.
  const applyFileFilter = (file) => {
    // Exclude test files
    if (file.indexOf('browsertest') > -1 || file.indexOf('unittest') > -1) { return false }

    // Only includes cc and h files.
    const ext = path.extname(file)
    if (ext !== '.cc' && ext !== '.h' && ext !== '.mm') { return false }

    return true
  }

  const chromiumSrcDir = path.join(config.srcDir, 'presearch', 'chromium_src')
  var sourceFiles = util.walkSync(chromiumSrcDir, applyFileFilter)
  const additionalGen = getAdditionalGenLocation()

  // Touch original files by updating mtime.
  const chromiumSrcDirLen = chromiumSrcDir.length
  sourceFiles.forEach(chromiumSrcFile => {
    const relativeChromiumSrcFile = chromiumSrcFile.slice(chromiumSrcDirLen)
    var overriddenFile = path.join(config.srcDir, relativeChromiumSrcFile)
    // If the original file doesn't exist, assume that it's in the gen dir.
    const tryOverrideGen = !fs.existsSync(overriddenFile)
    if (tryOverrideGen) {
      overriddenFile = path.join(config.outputDir, 'gen', relativeChromiumSrcFile)
    }

    if (fs.existsSync(overriddenFile)) {
      // If overriddenFile is older than file in chromium_src, touch it to trigger rebuild.
      updateFileUTimesIfOverrideIsNewer(overriddenFile, chromiumSrcFile)
      // If the original file is in gen dir, then also check for secondary gen dir.
      if (tryOverrideGen && !!additionalGen) {
        overriddenFile = path.join(config.outputDir, additionalGen, 'gen', relativeChromiumSrcFile)
        // Since gen file exists, expecting secondary gen file to exist too.
        if (!fs.existsSync(overriddenFile)) {
          throw 'Expected to find [' + overriddenFile + ']. Check if the name of the gen parent directory has changed.'
        }
        updateFileUTimesIfOverrideIsNewer(overriddenFile, chromiumSrcFile)
      }
    }
  })
}

const touchOverriddenVectorIconFiles = () => {
  console.log('touch original vector icon files overridden by presearch/vector_icons...')

  // Return true when original file of |file| should be touched.
  const applyFileFilter = (file) => {
    // Only includes icon files.
    const ext = path.extname(file)
    if (ext !== '.icon') { return false }
    return true
  }

  const presearchVectorIconsDir = path.join(config.srcDir, 'presearch', 'vector_icons')
  var presearchVectorIconFiles = util.walkSync(presearchVectorIconsDir, applyFileFilter)

  // Touch original files by updating mtime.
  const presearchVectorIconsDirLen = presearchVectorIconsDir.length
  presearchVectorIconFiles.forEach(presearchVectorIconFile => {
    var overriddenFile = path.join(config.srcDir, presearchVectorIconFile.slice(presearchVectorIconsDirLen))
    if (fs.existsSync(overriddenFile)) {
      // If overriddenFile is older than file in vector_icons, touch it to trigger rebuild.
      updateFileUTimesIfOverrideIsNewer(overriddenFile, presearchVectorIconFile)
    }
  })
}

/**
 * Checks to make sure the src/chrome/VERSION matches presearch-browser's package.json version
 */
const checkVersionsMatch = () => {
  const srcChromeVersionDir = path.resolve(path.join(config.srcDir, 'chrome', 'VERSION'))
  const versionData = fs.readFileSync(srcChromeVersionDir, 'utf8')
  const re = /MAJOR=(\d+)\s+MINOR=(\d+)\s+BUILD=(\d+)\s+PATCH=(\d+)/
  const found = versionData.match(re)
  const presearchVersionFromChromeFile = `${found[2]}.${found[3]}.${found[4]}`
  if (presearchVersionFromChromeFile !== config.presearchVersion) {
    // Only a warning. The CI environment will choose to proceed or not within its own script.
    console.warn(`Version files do not match!\nsrc/chrome/VERSION: ${presearchVersionFromChromeFile}\npresearch-browser package.json version: ${config.presearchVersion}`)
  }
}

const build = (buildConfig = config.defaultBuildConfig, options) => {
  config.buildConfig = buildConfig
  config.update(options)
  checkVersionsMatch()

  touchOverriddenFiles()
  touchOverriddenVectorIconFiles()
  util.updateBranding()

  if (config.xcode_gen_target) {
    util.generateXcodeWorkspace()
  } else {
    util.buildTarget()
  }
}

module.exports = build
