// Copyright (c) 2021 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { boolean, object, number } from '@storybook/addon-knobs'

// Components
import TableContribute, { DetailRow as ContributeDetailRow } from '../components/tableContribute'
import TableDonation, { DetailRow as DonationDetailRow } from '../components/tableDonation'
import TableTransactions, { DetailRow as TransactionsRow } from '../components/tableTransactions'
import TablePending, { DetailRow as PendingDetailRow } from '../components/tablePending'

const favicon = require('./img/presearch-favicon.png')
const ddgo = require('./img/ddgo.jpg')
const wiki = require('./img/wiki.jpg')
const buzz = require('./img/buzz.jpg')
const guardian = require('./img/guardian.jpg')
const eich = require('./img/eich.jpg')

const doNothing = () => {
  console.log('nothing')
}

export default {
  title: 'Rewards/Table',
  parameters: {
    layout: 'centered'
  }
}

export const contribution = () => {
  const header: string[] = [
    'Site',
    'Attention'
  ]

  const rows: ContributeDetailRow[] = [
    {
      profile: {
        name: 'Jonathon Doe',
        verified: true,
        provider: 'youtube',
        src: favicon
      },
      url: 'https://presearch.com',
      attention: 40,
      onRemove: doNothing
    },
    {
      profile: {
        name: 'duckduckgo.com',
        verified: true,
        src: ddgo
      },
      url: 'https://presearch.com',
      attention: 20,
      onRemove: doNothing
    },
    {
      profile: {
        name: 'buzzfeed.com',
        verified: false,
        src: buzz
      },
      url: 'https://presearch.com',
      attention: 10,
      onRemove: doNothing
    },
    {
      profile: {
        name: 'theguardian.com',
        verified: true,
        src: guardian
      },
      url: 'https://presearch.com',
      attention: 5,
      onRemove: doNothing
    },
    {
      profile: {
        name: 'wikipedia.org',
        verified: false,
        src: wiki
      },
      url: 'https://presearch.com',
      attention: 4,
      onRemove: doNothing
    }
  ]
  return (
    <div style={{ width: '595px' }}>
      <TableContribute
        header={object('Header', header)}
        rows={object('Rows', rows)}
        allSites={boolean('Are this all sites?', false)}
        numSites={number('Number of all sites?', 55)}
        showRowAmount={boolean('Show row amount', false)}
        showRemove={boolean('Show remove action', true)}
        headerColor={boolean('Colored header', true)}
      >
        Please visit some sites
      </TableContribute>
    </div>
  )
}

export const conation = () => {
  const rows: DonationDetailRow[] = [
    {
      profile: {
        name: 'Jonathon Doe',
        verified: true,
        provider: 'youtube',
        src: favicon
      },
      url: 'https://presearch.com',
      type: 'recurring',
      contribute: {
        tokens: '2.0',
        converted: '0.20'
      },
      onRemove: doNothing
    },
    {
      profile: {
        verified: false,
        name: 'theguardian.com',
        src: guardian
      },
      url: 'https://presearch.com',
      type: 'donation',
      contribute: {
        tokens: '12000.0',
        converted: '6000.20'
      },
      text: 'May 7'
    },
    {
      profile: {
        verified: false,
        name: 'BrendanEich',
        provider: 'twitter',
        src: eich
      },
      url: 'https://presearch.com',
      type: 'tip',
      contribute: {
        tokens: '1.0',
        converted: '0.20'
      },
      text: 'May 2'
    }
  ]
  return (
    <div style={{ width: '595px' }}>
      <TableDonation
        rows={object('Rows', rows)}
        allItems={boolean('Are this all items?', false)}
        numItems={number('Number of all items?', 55)}
        headerColor={boolean('Colored header', true)}
      >
        Please visit some sites
      </TableDonation>
    </div>
  )
}

export const transactions = () => {
  const rows: TransactionsRow[] = [
    {
      date: 1576066103000,
      type: 'ads',
      description: 'Presearch Ads payment for May',
      amount: {
        value: '5.0',
        converted: '5.00'
      }
    },
    {
      date: 1576066103000,
      type: 'tip',
      description: {
        publisher: 'Jonathon Doe',
        platform: 'YouTube'
      },
      amount: {
        isNegative: true,
        value: '5.0',
        converted: '11.00'
      }
    },
    {
      date: 1576066103000,
      type: 'contribute',
      description: 'Monthly payment',
      amount: {
        isNegative: true,
        value: '5.0',
        converted: '15.00'
      }
    }
  ]
  return (
    <div style={{ width: '595px' }}>
      <TableTransactions
        rows={object('Rows', rows)}
      >
        Sorry no transactions.
      </TableTransactions>
    </div>
  )
}

export const pendingContributions = () => {
  const rows: PendingDetailRow[] = [
    {
      profile: {
        name: 'Jonathon Doe',
        verified: true,
        provider: 'youtube',
        src: favicon
      },
      url: 'https://presearch.com',
      type: 'recurring',
      amount: {
        tokens: '2.0',
        converted: '0.20'
      },
      date: 'Jan 2',
      onRemove: doNothing
    },
    {
      profile: {
        verified: false,
        name: 'theguardian.com',
        src: guardian
      },
      url: 'https://presearch.com',
      type: 'tip',
      amount: {
        tokens: '12000.0',
        converted: '6000.20'
      },
      date: 'May 7',
      onRemove: doNothing
    },
    {
      profile: {
        verified: false,
        name: 'BrendanEich',
        provider: 'twitter',
        src: eich
      },
      url: 'https://presearch.com',
      type: 'ac',
      amount: {
        tokens: '1.0',
        converted: '0.20'
      },
      date: 'May 2',
      onRemove: doNothing
    }
  ]
  return (
    <div style={{ width: '595px' }}>
      <TablePending
        rows={object('Rows', rows)}
      >
        Please visit some sites
      </TablePending>
    </div>
  )
}
