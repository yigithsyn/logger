// 'log' logs are prefixed with [LOG] in green color
const originalConsoleLog = console.log.bind(console);
console.log = (...args) => {
    const prefix = '\x1b[32m[LOG]\x1b[0m'; // Green
    originalConsoleLog(`${prefix} ${(new Date()).toISOString()}`, ...args);
}


// 'debug' logs are prefixed with [DEBUG] in magenta color
const originalConsoleDebug = console.debug.bind(console);
console.debug = (...args) => {
    const prefix = '\x1b[35m[DEBUG]\x1b[0m'; // Magenta
    originalConsoleDebug(`${prefix} ${(new Date()).toISOString()}`, ...args);
};

// 'warn' logs are prefixed with [WARN] in yellow color
const originalConsoleWarn = console.warn.bind(console);
console.warn = (...args) => {
    const prefix = '\x1b[33m[WARN]\x1b[0m'; // Yellow
    originalConsoleWarn(`${prefix} ${(new Date()).toISOString()}`, ...args);
};

// 'info' logs are prefixed with [INFO] in cyan color
const originalConsoleInfo = console.info.bind(console);
console.info = (...args) => {
    const prefix = '\x1b[36m[INFO]\x1b[0m'; // Cyan
    originalConsoleInfo(`${prefix} ${(new Date()).toISOString()}`, ...args);
};

// 'error' logs are prefixed with [ERROR] in red color
const originalConsoleError = console.error.bind(console);
console.error = (...args) => {
    const prefix = '\x1b[31m[ERROR]\x1b[0m'; // Red
    originalConsoleError(`${prefix} ${(new Date()).toISOString()}`, ...args);
};