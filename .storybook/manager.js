import { addons } from '@storybook/addons'
import { create } from '@storybook/theming'

const presearchTheme = create({
  base: 'dark',
  brandTitle: 'Presearch Browser UI',
  brandUrl: 'https://github.com/brave/presearch-core'
})

addons.setConfig({
  isFullscreen: false,
  showNav: true,
  showPanel: true,
  panelPosition: 'right',
  theme: presearchTheme
})
