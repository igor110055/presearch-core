import styled, * as Styled from 'styled-components'
import IPresearchTheme from 'brave-ui/theme/theme-interface'
import IWelcomeTheme from './welcome-theme'

type Theme = IPresearchTheme & IWelcomeTheme

export default styled as unknown as Styled.ThemedStyledInterface<Theme>
export const css = Styled.css as Styled.ThemedCssFunction<Theme>
