    /* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import {
  Grid,
  HeaderGrid,
  ButtonGroup,
  Box,
  Content,
  HeaderBox,
  Title,
  SubTitle,
  Text,
  PrivateImage,
  DuckDuckGoImage, TorLockImage,
  Separator,
  FakeButton,
  Link

} from '../../components/private'
import { Toggle } from '../../components/toggle'

import locale from './fakeLocale'
const privateWindowImg = require('../../../img/newtab/private-window.svg')

export default class PrivateTab extends React.PureComponent<{}, {}> {
  render () {
    return (
      <Grid>
        <HeaderBox>
          <HeaderGrid>
            <PrivateImage src={privateWindowImg} />
            <div>
              <SubTitle>{locale.headerLabel}</SubTitle>
              <Title>{locale.headerTitle}</Title>
              <Text>{locale.headerText}</Text>
              {/* <FakeButton href='https://support.presearch.org/hc/en-us/articles/360017840332' target='_blank'>
                {locale.headerButton}
              </FakeButton> */}
            </div>
          </HeaderGrid>
        </HeaderBox>
      </Grid>
    )
  }
}
