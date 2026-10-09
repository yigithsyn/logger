#!/usr/bin/env node
process.argv.splice(2, 0, 'debug');
require('../cli.js');
