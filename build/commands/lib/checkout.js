const config = require('../lib/config')
const util = require('../lib/util')

const checkout = (presearchCoreRef, options) => {
  async function RunCommand () {
    config.update(options)

    let diff = util.run('git', ['-C', config.presearchCoreDir, 'diff', '--shortstat'], options)
    if (diff.status === 0 && diff.stdout.toString() !== '') {
      console.error('Please stash or commit your changes')
      console.error(diff.stdout.toString())
      process.exit(1)
    }

    let revParse = util.run('git', ['-C', config.presearchCoreDir, 'rev-parse', '--symbolic-full-name', presearchCoreRef], options)
    if (revParse.status === 0 && revParse.stdout.toString() !== '') {
      presearchCoreRef = revParse.stdout.toString()
    }

    let args = ['sync', '--nohooks', '--force', '--reset', '--upstream', '--revision', 'src/presearch@' + presearchCoreRef]

    util.gclientSync(false, options)

    await util.applyPatches()

    util.gclientRunhooks()
  }

  RunCommand().catch((err) => {
    console.error(err)
    process.exit(1)
  })
}

module.exports = checkout
