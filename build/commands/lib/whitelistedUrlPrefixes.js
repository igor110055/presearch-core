// Before adding to this list, get approval from the security team
module.exports = [
  'http://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.presearch.com. should never actually connect to googleapis.com.
  'https://update.googleapis.com/service/update2', // allowed because it 307's to go-updater.presearch.com. should never actually connect to googleapis.com.
  'https://safebrowsing.googleapis.com/v4/threatListUpdates', // allowed because it 307's to safebrowsing.presearch.com
  'https://clients2.googleusercontent.com/crx/blobs/',
  'http://dl.google.com/', // allowed because it 307's to redirector.presearch.com
  'https://dl.google.com/', // allowed because it 307's to redirector.presearch.com
  'https://no-thanks.invalid/', // fake gaia URL
  'https://go-updater.presearch.com/',
  'https://safebrowsing.presearch.com/',
  'https://presearch-core-ext.s3.presearch.com/',
  'https://laptop-updates.presearch.com/',
  'https://static.presearch.com/',
  'https://static1.presearch.com/',
  'http://componentupdater.presearch.com/service/update2', // allowed because it 307's to https://componentupdater.presearch.com
  'https://componentupdater.presearch.com/service/update2',
  'https://crlsets.presearch.com/',
  'https://crxdownload.presearch.com/crx/blobs/',
  'https://updates.presearchsoftware.com/',
  'https://p3a.presearch.com/',
  'https://dns.google/dns-query', // needed for DoH on Mac build machines
  'https://chrome.cloudflare-dns.com/dns-query', // needed for DoH on Mac build machines
  'https://tor.presearchsoftware.com/', // for fetching tor client updater component
  'https://redirector.presearch.com/',
  'https://sync-v2.presearch.com/v2', // presearch sync v2 production
  'https://sync-v2.presearchsoftware.com/v2', // presearch sync v2 staging
  'https://sync-v2.presearch.software/v2', // presearch sync v2 dev
  'https://variations.presearch.com/seed', // presearch A/B testing
  'https://presearch-today-cdn.presearch.com/', // Presearch Today (production)
  'https://pcdn.presearch.com/', // Presearch's Privacy-focused CDN
]
