/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { shallow } from 'enzyme'
import { applicationState, torrentState, torrentObj } from '../testData'
import { TorrentState } from '../../../presearch_webtorrent/extension/constants/webtorrentState'
import { PresearchWebtorrentPage, mapStateToProps } from '../../../presearch_webtorrent/extension/components/app'
import { StyledTorrentViewer } from '../../../presearch_webtorrent/extension/styles/styles'

describe('PresearchWebtorrentPage component', () => {
  describe('mapStateToProps', () => {
    it('should map the default state', () => {
      expect(mapStateToProps(applicationState, { tabId: 0 })).toEqual({
        torrentState
      })
    })
  })

  describe('render PresearchWebtorrentPage component', () => {
    it('renders the MediaViewer component with a torrent and ix', () => {
      const torrentStateWithIx: TorrentState = { ...torrentState, ix: 1 }
      const wrapper = shallow(
        <PresearchWebtorrentPage
          torrentState={torrentStateWithIx}
          torrentObj={torrentObj}
          actions={{}}
        />
      ).dive()

      const assertion = wrapper.find('.mediaViewer')
      expect(assertion.length).toBe(1)
    })
    it('renders the TorrentViewer component with a valid torrent state', () => {
      const wrapper = shallow(
        <PresearchWebtorrentPage
          torrentState={torrentState}
          actions={{}}
        />
      ).dive()

      expect(wrapper.find(StyledTorrentViewer)).toHaveLength(1)
    })
  })
})
