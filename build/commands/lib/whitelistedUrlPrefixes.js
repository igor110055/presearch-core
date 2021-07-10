// Before adding to this list, get approval from the security team
module.exports = [
  'http://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.presearch.org. should never actually connect to googleapis.com.
  'https://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.presearch.org. should never actually connect to googleapis.com.
  'https://safebrowsing.googleapis.com/v4/threatListUpdates', // allowed because it 307's to safebrowsing.presearch.org
  'https://clients2.googleusercontent.com/crx/blobs/',
  'http://dl.google.com/', // allowed because it 307's to redirector.presearch.org
  'https://dl.google.com/', // allowed because it 307's to redirector.presearch.org
  'https://no-thanks.invalid/', // fake gaia URL
  'https://go-updater.presearch.org/',
  'https://static.presearch.org/',
  'https://engine.presearch.org/',
  'https://presearch.org/',
  'https://dns.google/dns-query', // needed for DoH on Mac build machines
  'https://chrome.cloudflare-dns.com/dns-query', // needed for DoH on Mac build machines
  'https://support.presearch.org/',
]
