/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import modalBackground from './assets/opt_in_modal_bg.svg'

export const root = styled.div`
  height: 100%;
  width: 100%;
  background-color: var(--presearch-palette-white);
  background-image: url('${modalBackground}');
  background-repeat: no-repeat;
  background-position: 0 0;
  background-size: auto 220px;
  border-radius: 4px;
  font-family: var(--presearch-font-heading);
  text-align: center;
  padding: 90px 0;

  a {
    color: var(--presearch-color-brandBat);
    font-weight: 600;
    text-decoration: none;
  }
`

export const heading = styled.div`
  font-weight: 600;
  font-size: 36px;
  line-height: 60px;

  .icon {
    height: 45px;
    vertical-align: middle;
    margin-bottom: 6px;
    margin-right: 13px;
  }
`

export const subHeading = styled.div`
  font-size: 18px;
  line-height: 24px;
  color: var(--presearch-palette-neutral600);
`

export const text = styled.div`
  margin: 19px auto 0;
  color: var(--presearch-palette-neutral700);
  font-size: 14px;
  line-height: 22px;
  max-width: 325px;
`

export const takeTour = styled.div`
  margin-top: 20px;
  color: var(--presearch-color-brandBat);

  button {
    font-weight: 600;
    font-size: 14px;
    line-height: 21px;
    border: 0;
    background: 0;
    margin: 0;
    padding: 0;
    cursor: pointer;
  }
`

export const enable = styled.div`
  margin-top: 62px;

  button {
    min-width: 330px;
  }
`

export const footer = styled.div`
  width: 320px;
  margin: 10px auto;
  color: var(--presearch-palette-neutral600);
  font-size: 12px;
  line-height: 18px;
`
