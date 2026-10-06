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


/**
 * Store the log in the database
 */
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');

const sqlite3 = (() => {
    try {
        return require('node:sqlite');
    } catch (error) {
        console.error(`Failed to load sqlite3 module: ${error.message}`);
        return null;
    }
})();

function getLogDatabasePath() {
    if (process.platform === 'win32') {
        const localAppData = process.env.LOCALAPPDATA || path.join(os.homedir(), 'AppData', 'Local');
        const logDir = path.join(localAppData, 'logger');

        try {
            fs.mkdirSync(logDir, { recursive: true });
        } catch (error) {
            console.error(`Failed to create log directory: ${error.message}`);
        }

        return path.join(logDir, 'logger.db');
    }

    const logDir = path.join(os.homedir(), 'logger');

    try {
        fs.mkdirSync(logDir, { recursive: true });
    } catch (error) {
        return path.join(os.homedir(), 'logger', 'logger.db');
    }

    return path.join(logDir, 'logger.db');
}

function storeLog(logLevel, logMessage) {

    if (!sqlite3) {
        console.error(`SQLite3 module is not available. Cannot store log: [${logLevel}] ${logMessage}`);
        return;
    }
    const dbPath = getLogDatabasePath();
    const database = new sqlite3.DatabaseSync(dbPath)
    database.exec('PRAGMA journal_mode = WAL');   // WAL mode
    database.exec('PRAGMA busy_timeout = 5000');  // Queue operations instead of locking
    database.exec('PRAGMA synchronous = NORMAL'); // Safe, high-performance disk writing


    try {
        database.exec(`
            CREATE TABLE IF NOT EXISTS logs (
                id INTEGER PRIMARY KEY,
                date TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
                logLevel TEXT NOT NULL,
                message TEXT NOT NULL
            )
        `);
    } catch (error) {
        console.error(`Failed to create logs table: ${error.message}`);
        database.close();
        return;
    }

    const insertLogQuery = database.prepare('INSERT INTO logs (logLevel, message) VALUES (?, ?)');
    try {
        insertLogQuery.run(logLevel, logMessage);
    } catch (insertError) {
        console.error(`Failed to insert log: ${insertError.message}`);
    }

    database.close();
}


// Store the log in the database
storeLog(logLevel, logMessage);

