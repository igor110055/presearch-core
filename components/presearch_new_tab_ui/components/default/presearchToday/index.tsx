// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import * as PresearchTodayElement from './default'
import CardIntro from './cards/cardIntro'
import CardLoading from './cards/cardLoading'
import { ReadFeedItemPayload } from '../../../actions/today_actions'
const Content = React.lazy(() => import('./content'))

type State = {
  hasInteractionStarted: boolean
  isIntroCardVisible: boolean
}

export type OnReadFeedItem = (args: ReadFeedItemPayload) => any
export type OnSetPublisherPref = (publisherId: string, enabled: boolean) => any
export type OnPromotedItemViewed = (item: PresearchToday.FeedItem) => any

export type Props = {
  isFetching: boolean
  isUpdateAvailable: boolean
  isIntroDismissed: boolean
  feed?: PresearchToday.Feed
  publishers?: PresearchToday.Publishers
  articleToScrollTo?: PresearchToday.FeedItem
  displayedPageCount: number
  onInteracting: (interacting: boolean) => any
  onReadFeedItem: OnReadFeedItem
  onPromotedItemViewed: OnPromotedItemViewed
  onFeedItemViewedCountChanged: (feedItemsViewed: number) => any
  onSetPublisherPref: OnSetPublisherPref
  onAnotherPageNeeded: () => any
  onCustomizePresearchToday: () => any
  onRefresh: () => any
  onCheckForUpdate: () => any
  onReadCardIntro: () => any
}

export const attributeNameCardCount = 'data-today-card-count'

class PresearchToday extends React.PureComponent<Props, State> {
  presearchTodayHitsViewportObserver: IntersectionObserver
  scrollTriggerToFocusPresearchToday: any // React.RefObject<any>

  constructor (props: Props) {
    super(props)
    // Don't remove Intro Card until the page refreshes
    this.state = {
      hasInteractionStarted: false,
      isIntroCardVisible: !props.isIntroDismissed
    }
  }

  componentDidMount () {
    const options = { root: null, rootMargin: '0px', threshold: 0.25 }

    this.presearchTodayHitsViewportObserver = new
      IntersectionObserver(this.handlePresearchTodayHitsViewportObserver, options)

    // Handle first card showing up so we can hide secondary UI
    this.presearchTodayHitsViewportObserver.observe(this.scrollTriggerToFocusPresearchToday)
  }

  handlePresearchTodayHitsViewportObserver = (entries: IntersectionObserverEntry[]) => {
    const isIntersecting = entries.some(entry => entry.isIntersecting)
    this.props.onInteracting(isIntersecting)
    if (isIntersecting) {
      this.setState({ hasInteractionStarted: true })
    }
  }

  render () {
    const shouldDisplayContent =
      this.state.hasInteractionStarted ||
      !!this.props.articleToScrollTo

    return (
      <PresearchTodayElement.Section>
        <div
          ref={scrollTrigger => (this.scrollTriggerToFocusPresearchToday = scrollTrigger)}
          style={{ position: 'sticky', top: '100px' }}
        />
        { !this.props.isIntroDismissed &&
        <CardIntro onRead={this.props.onReadCardIntro} />
        }
        { shouldDisplayContent &&
        <React.Suspense fallback={(<CardLoading />)}>
          <Content {...this.props} />
        </React.Suspense>
        }

      </PresearchTodayElement.Section>
    )
  }
}

export default PresearchToday
