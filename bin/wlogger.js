#!/usr/bin/env node
process.argv.splice(2, 0, 'warn');
await import('../cli.js');
