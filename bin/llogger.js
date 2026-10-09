#!/usr/bin/env node
process.argv.splice(2, 0, 'log');
require('../cli.js');
