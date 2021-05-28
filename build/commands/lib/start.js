const path = require('path')
const fs = require('fs-extra')
const ip = require('ip')
const URL = require('url').URL
const config = require('../lib/config')
const util = require('../lib/util')
const whitelistedUrlPrefixes = require('./whitelistedUrlPrefixes')
const whitelistedUrlPatterns = require('./whitelistedUrlPatterns')
const whitelistedUrlProtocols = [
  'chrome-extension:',
  'chrome:',
  'presearch:',
  'file:',
  'data:',
  'blob:'
]

const start = (passthroughArgs, buildConfig = config.defaultBuildConfig, options) => {
  config.buildConfig = buildConfig
  config.update(options)

  let presearchArgs = [
    '--enable-logging',
    '--v=' + options.v,
  ]
  if (options.vmodule) {
    presearchArgs.push('--vmodule=' + options.vmodule);
  }
  if (options.no_sandbox) {
    presearchArgs.push('--no-sandbox')
  }
  if (options.disable_presearch_extension) {
    presearchArgs.push('--disable-presearch-extension')
  }
  if (options.disable_presearch_rewards_extension) {
    presearchArgs.push('--disable-presearch-rewards-extension')
  }
  if (options.disable_pdfjs_extension) {
    presearchArgs.push('--disable-pdfjs-extension')
  }
  if (options.disable_webtorrent_extension) {
    presearchArgs.push('--disable-webtorrent-extension')
  }
  if (options.ui_mode) {
    presearchArgs.push(`--ui-mode=${options.ui_mode}`)
  }
  if (!options.enable_presearch_update) {
    // This only has meaning with MacOS and official build.
    presearchArgs.push('--disable-presearch-update')
  }
  if (options.disable_doh) {
    presearchArgs.push('--disable-doh')
  }
  if (options.enable_smart_tracking_protection) {
    presearchArgs.push('--enable-smart-tracking-protection')
  }
  if (options.single_process) {
    presearchArgs.push('--single-process')
  }
  if (options.show_component_extensions) {
    presearchArgs.push('--show-component-extension-options')
  }
  if (options.rewards) {
    presearchArgs.push(`--rewards=${options.rewards}`)
  }
  if (options.presearch_ads_testing) {
    presearchArgs.push('--presearch-ads-testing')
  }
  if (options.presearch_ads_debug) {
    presearchArgs.push('--presearch-ads-debug')
  }
  if (options.presearch_ads_production) {
    presearchArgs.push('--presearch-ads-production')
  }
  if (options.presearch_ads_staging) {
    presearchArgs.push('--presearch-ads-staging')
  }
  presearchArgs = presearchArgs.concat(passthroughArgs)

  let user_data_dir
  if (options.user_data_dir_name) {
    if (process.platform === 'darwin') {
      user_data_dir = path.join(process.env.HOME, 'Library', 'Application\\ Support', 'PresearchSoftware', options.user_data_dir_name)
    } else if (process.platform === 'win32') {
      user_data_dir = path.join(process.env.LocalAppData, 'PresearchSoftware', options.user_data_dir_name)
    } else {
      user_data_dir = path.join(process.env.HOME, '.config', 'PresearchSoftware', options.user_data_dir_name)
    }
    presearchArgs.push('--user-data-dir=' + user_data_dir);
  }
  const networkLogFile = path.resolve(path.join(config.rootDir, 'network_log.json'))
  if (options.network_log) {
    presearchArgs.push(`--log-net-log=${networkLogFile}`)
    presearchArgs.push(`--net-log-capture-mode=Everything`)
    if (user_data_dir) {
      // clear the data directory before doing a network test
      fs.removeSync(user_data_dir.replace('\\', ''))
      if (fs.existsSync(networkLogFile)) {
        fs.unlinkSync(networkLogFile)
      }
      if (fs.existsSync('network-audit-results.json')) {
        fs.unlinkSync('network-audit-results.json')
      }
    }
  }

  let cmdOptions = {
    stdio: 'inherit',
    timeout: options.network_log ? 120000 : undefined,
    continueOnFail: options.network_log ? true : false,
    shell: process.platform === 'darwin' ? true : false,
    killSignal: options.network_log && process.env.RELEASE_TYPE ? 'SIGKILL' : 'SIGTERM'
  }

  if (options.network_log) {
    console.log('Network audit started. Logging requests for the next 2min or until you quit Presearch...')
  }

  let outputPath = options.output_path
  if (!outputPath) {
    outputPath = path.join(config.outputDir, 'presearch')
    if (process.platform === 'win32') {
      outputPath = outputPath + '.exe'
    } else if (process.platform === 'darwin') {
      outputPath = fs.readFileSync(outputPath + '_helper').toString().trim()
    }
  }
  util.run(outputPath, presearchArgs, cmdOptions)

  if (options.network_log) {
    let exitCode = 0
    let jsonOutput = {}
    // Read the network log
    let jsonContent = fs.readFileSync(networkLogFile, 'utf8').trim()
    // On windows netlog ends abruptly causing JSON parsing errors
    if (!jsonContent.endsWith('}]}')) {
      const n = jsonContent.lastIndexOf('},')
      jsonContent = jsonContent.substring(0, n) + '}]}'
    }
    jsonOutput = JSON.parse(jsonContent)

    const URL_REQUEST_TYPE = jsonOutput.constants.logSourceType.URL_REQUEST
    const URL_REQUEST_FAKE_RESPONSE_HEADERS_CREATED = jsonOutput.constants.logEventTypes.URL_REQUEST_FAKE_RESPONSE_HEADERS_CREATED
    const urlRequests = jsonOutput.events.filter((event) => {
      if (event.type === URL_REQUEST_FAKE_RESPONSE_HEADERS_CREATED) {
        // showing these helps determine which URL requests which don't
        // actually hit the network
        return true
      }
      if (event.source.type === URL_REQUEST_TYPE) {
        if (!event.params) {
          return false
        }
        const url = event.params.url
        if (!url) {
          return false
        }
        const urlParsed = new URL(url)
        const hostname = urlParsed.hostname
        if (/^[a-z]+$/.test(hostname)) {
          // Chromium sometimes sends requests to random non-resolvable hosts
          return false
        }
        if (whitelistedUrlProtocols.includes(urlParsed.protocol)) {
          return false
        }
        const foundPrefix = whitelistedUrlPrefixes.find((prefix) => {
          return url.startsWith(prefix)
        })
        const foundPattern = whitelistedUrlPatterns.find((pattern) => {
          return RegExp('^' + pattern).test(url)
        })
        if (!foundPrefix && !foundPattern) {
          // Check if the URL is a private IP
          try {
            if (ip.isPrivate(hostname)) {
              // Warn but don't fail the audit
              console.log('NETWORK AUDIT WARN:', url)
              return true
            }
          } catch (e) {}
          // This is not a whitelisted URL! log it and exit with non-zero
          console.log('NETWORK AUDIT FAIL:', url)
          exitCode = 1
        }
        return true
      }
      return false
    })
    fs.writeJsonSync('network-audit-results.json', urlRequests)
    if (exitCode > 0) {
      console.log(`network-audit failed. import ${networkLogFile} in chrome://net-internals for more details.`)
    } else {
      console.log('network audit passed.')
    }
    process.exit(exitCode)
  }
}

module.exports = start
