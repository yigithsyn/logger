#!/usr/bin/env node
process.argv.splice(2, 0, 'info');
require('../cli.js');
