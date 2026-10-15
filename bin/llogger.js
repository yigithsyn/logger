#!/usr/bin/env node
process.argv.splice(2, 0, 'log');
await import('../cli.js');
