// Copyright (c) 2020 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale } from '../../../../../common/locale'
import VisibilityTimer from '../../../../helpers/visibilityTimer'
import * as Card from '../cardIntro'
import PresearchTodayLogoUrl from '../presearchTodayLogo.svg'

const timeToHideMs = 4000

type Props = {
  onRead: () => void
}

export default function IntroCard (props: Props) {
  const introElementRef = React.useRef(null)

  // Only mark as 'read' when it's been in the viewport for a
  // specific amount of time, and the tab is active.
  React.useEffect(() => {
    const element = introElementRef.current
    if (!element) {
      return
    }
    const observer = new VisibilityTimer(props.onRead, timeToHideMs, element)
    observer.startTracking()
    return () => {
      observer.stopTracking()
    }
  }, [introElementRef.current, props.onRead])

  return (
    <Card.Intro ref={introElementRef}>
      <Card.Image src={PresearchTodayLogoUrl} />
      <Card.Heading>{getLocale('presearchTodayIntroTitle')}</Card.Heading>
      <Card.Text>{getLocale('presearchTodayIntroDescription')}</Card.Text>
    </Card.Intro>
  )
}
