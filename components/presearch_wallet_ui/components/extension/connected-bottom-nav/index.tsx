import * as React from 'react'

// Styled Components
import {
  StyledWrapper,
  AppsIcon,
  NavButton,
  NavButtonText,
  NavDivider,
  NavOutline
} from './style'

import { PanelTypes } from '../../../constants/types'

export interface Props {
  action: (path: PanelTypes) => void
}

export default class ConnectedBottomNav extends React.PureComponent<Props> {

  navigate = (path: PanelTypes) => () => {
    this.props.action(path)
  }

  render () {
    return (
      <StyledWrapper>
        <NavOutline>
          <NavButton onClick={this.navigate('buy')}>
            <NavButtonText>Buy</NavButtonText>
          </NavButton>
          <NavDivider />
          <NavButton onClick={this.navigate('send')}>
            <NavButtonText>Send</NavButtonText>
          </NavButton>
          <NavDivider />
          <NavButton onClick={this.navigate('swap')}>
            <NavButtonText>Swap</NavButtonText>
          </NavButton>
          <NavDivider />
          <NavButton onClick={this.navigate('apps')}>
            <AppsIcon />
          </NavButton>
        </NavOutline>
      </StyledWrapper>
    )
  }
}
