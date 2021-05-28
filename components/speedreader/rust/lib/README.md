# Presearch SpeedReader

At a high level, SpeedReader:

* Distills text-focused document content from a suitable HTML
* Works on HTML documents before rendering them
* Generates HTML output with no external styling or scripting
* Content styled with Presearch-designed themes

## Structure

SpeedReader has two methods of deciding whether a page is readable:

* `classifier` that analyses HTML content of a page and applies a pre-trained
  classifier to decide whether the page is article-style, text-oriented
* `whitelist` that uses adblock-style rules that explciitly whitelist pages that
  should be distilled

Any page that is readable should then be passed to an instance of `SpeedReader`
. Internally it uses the `whitelist` to determine whether to apply streaming,
rule-based rewriting (using [lol_html](https://github.com/cloudflare/lol-html)
to support output streaming), or heuristics approach loosely based on [Mozilla's
Readability](https://github.com/mozilla/readability) and Rust implementation
improved from https://github.com/kumabook/readability.

## Running the tests

You need to clone the speedreader test data to the
`components/speedreader/rust/lib` directory.

```
$ git@github.com:presearch-experiments/speedreader-test-data.git data
$ cargo test
```
