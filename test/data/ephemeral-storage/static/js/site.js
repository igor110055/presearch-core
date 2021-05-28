(async _ => {
  const W = window
  const D = document
  const isDebug = false

  const presearchSoftwareOrigin = 'dev-pages.presearchsoftware.com'
  const presearchSoftwareComOrigin = 'dev-pages.presearch.software'

  let thisOrigin = document.location.host
  const bodyElm = document.body
  let otherOrigin

  if (thisOrigin.startsWith(presearchSoftwareOrigin)) {
      thisOrigin = presearchSoftwareOrigin
      otherOrigin = presearchSoftwareComOrigin
      bodyElm.className += ' presearch-software-com'
  } else if (thisOrigin.startsWith(presearchSoftwareComOrigin)) {
      thisOrigin = presearchSoftwareComOrigin
      otherOrigin = presearchSoftwareOrigin
      bodyElm.className += ' presearch-software'
  }

  const classToOrigin = {
    'other-origin': otherOrigin,
    'this-origin': thisOrigin
  }

  for (const [aClass, anOrigin] of Object.entries(classToOrigin)) {
    const elms = D.getElementsByClassName(aClass)
    for (const elm of elms) {
      const elmTagName = elm.tagName.toLowerCase()
      switch (elmTagName) {
        case 'iframe':
        case 'img':
        case 'script':
          elm.src = '/cross-site/' + anOrigin + elm.dataset.src
          break

        case 'a':
          elm.href = '/cross-site/' + anOrigin + elm.dataset.href
          break

        default:
          elm.textContent = anOrigin
          break
      }
    }
  }

  const logger = msg => {
    if (isDebug !== true) {
      return
    }
    console.log(typeof msg === 'string' ? msg : JSON.stringify(msg))
  }

  const thisOriginUrl = path => {
    return '/cross-site/' + thisOrigin + path
  }

  const otherOriginUrl = path => {
    return '/cross-site/' + otherOrigin + path
  }

  const switchOriginElm = D.querySelector('.breadcrumb .other-origin')
  // Frames will run this code, but not have breadcrumbs or links
  if (switchOriginElm) {
    switchOriginElm.href = otherOriginUrl(W.location.pathname)
  }

  const sendPostMsg = async (windowElm, action, msg) => {
    return new Promise((resolve) => {
      const messageNonce = Math.random().toString()
      const onResponseCallback = response => {
        const { nonce, direction, payload } = response.data
        if (direction !== 'response') {
          return
        }
        if (nonce !== messageNonce) {
          return
        }
        W.removeEventListener('message', onResponseCallback)

        resolve(payload)
      }
      W.addEventListener('message', onResponseCallback, false)

      const outMsg = {
        nonce: messageNonce,
        payload: msg,
        direction: 'sending',
        action
      }
      windowElm.postMessage(outMsg, '*')
    })
  }

  const receivePostMsg = async handler => {
    const onMessage = async msg => {
      const { action, payload, direction, nonce } = msg.data
      if (direction !== 'sending') {
        return
      }

      const receivedResult = await handler(action, payload)
      if (receivedResult === undefined) {
        logger(`No result for action: ${action}`)
        return
      }

      const response = {
        direction: 'response',
        payload: receivedResult,
        nonce
      }

      msg.source.postMessage(response, '*')
    }

    W.addEventListener('message', onMessage, false)
  }

  W.PRESEARCH = {
    logger,
    thisOriginUrl,
    otherOriginUrl,
    sendPostMsg,
    receivePostMsg
  }
})()
