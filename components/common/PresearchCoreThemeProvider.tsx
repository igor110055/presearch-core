import * as React from 'react'
import { ThemeProvider } from 'styled-components'
import IPresearchTheme from 'brave-ui/theme/theme-interface'

export type Props = {
  initialThemeType?: chrome.presearchTheme.ThemeType
  dark: IPresearchTheme,
  light: IPresearchTheme
}
type State = {
  themeType?: chrome.presearchTheme.ThemeType
}

function themeTypeToState (themeType: chrome.presearchTheme.ThemeType): State {
  return {
    themeType
  }
}

export default class PresearchCoreThemeProvider extends React.Component<Props, State> {
  constructor (props: Props) {
    super(props)
    if (props.initialThemeType) {
      this.state = themeTypeToState(props.initialThemeType)
    }
    // Ensure we have access to presearchTheme before updating.
    // Otherwise this would break Storybook.
    if (chrome.presearchTheme) {
      chrome.presearchTheme.onPresearchThemeTypeChanged.addListener(this.setThemeState)
    }
  }

  setThemeState = (themeType: chrome.presearchTheme.ThemeType) => {
    this.setState(themeTypeToState(themeType))
  }

  componentDidUpdate (prevProps: Props) {
    // Update theme based on React prop changes.
    // This only runs on storybook and is needed
    // since it has no access to chrome.* APIs
    if (chrome.presearchTheme) {
      return
    }
    if (prevProps.initialThemeType !== this.props.initialThemeType) {
      this.setThemeState(this.props.initialThemeType || 'System')
    }
  }

  render () {
    // Don't render until we have a theme
    if (!this.state.themeType) return null
    // Render provided dark or light theme
    const selectedShieldsTheme = this.state.themeType === 'Dark'
                ? this.props.dark
                : this.props.light
    return (
      <ThemeProvider theme={selectedShieldsTheme}>
        {React.Children.only(this.props.children)}
      </ThemeProvider>
    )
  }
}
