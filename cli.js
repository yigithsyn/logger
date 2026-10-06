#!/usr/bin/env node
const help = `
Usage: logger <logLevel> <logMessage> [-h, --help]

Options:
logLevel    The log level to use (e.g., "info", "warn", "error"). Defaults to "info".
  logMessage  The log message to output.
  -h, --help  Show this help message.
  `;

////////////////////////////////////////////////////////////////////////////////
// Manual argument parsing
////////////////////////////////////////////////////////////////////////////////

// Check if the user requested help
if (process.argv.includes('--help') || process.argv.includes('-h')) {
    console.log(help);
    process.exit(0);
}

// Check command line arguments for log level
if (process.argv.length < 3) {
    console.log("Insufficient arguments provided. Use -h or --help for usage information.");
    process.exit(1);
}

const args = process.argv.slice(2);

// Parse log level and validate
const logLevel = args[0];
if (logLevel.includes('-') || logLevel.includes('--')) {
    console.log(`Invalid option: ${logLevel}.`);
    console.log(help);
    process.exit(1);
}

if (!['debug', 'log', 'info', 'warn', 'error'].includes(logLevel)) {
    console.log(`Invalid log level: ${logLevel}. Use  "debug", "log", "info", "warn", or "error".`);
    process.exit(1);
}

// Parse log message
const logMessage = args.slice(1).join(' ');

require('./index.js');

switch (logLevel) {
    case 'debug':
        console.debug(logMessage);
        break;
    case 'log':
        console.log(logMessage);
        break;
    case 'info':
        console.info(logMessage);
        break;
    case 'warn':
        console.warn(logMessage);
        break;
    case 'error':
        console.error(logMessage);
        break;
    default:
        console.log(logMessage);
}


