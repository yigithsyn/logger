#!/usr/bin/env node
process.argv.splice(2, 0, 'error');
require('../cli.js');
