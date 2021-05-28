function notifyMain(result) {
  self.clients.matchAll({
    includeUncontrolled: true,
    type: 'window',
  }).then((clients) => {
    if (clients && clients.length) {
      clients[0].postMessage(result);
    }
  });
}

function waitForFunction() {
  if (self.presearch != undefined && self.presearch.fetchBackupResults != undefined) {
    console.log('calling fetch backup results')
    self.presearch.fetchBackupResults('test', 'en', 'us', 'US', true)
        .then((response) => {
          notifyMain({result: 'INJECTED', response: response});
        })
  } else {
    notifyMain({result: 'FAILED'});
  }
}
setTimeout(waitForFunction, 1000);
