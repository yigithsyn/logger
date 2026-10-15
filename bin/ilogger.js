#!/usr/bin/env node
process.argv.splice(2, 0, 'info');
await import('../cli.js');
