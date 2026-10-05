# Simple and Colorful Logger Library

Logger library for colorful and simple logging in JavaScript applications.
It also adds timestamps to each log message.

## Installation

You can install the logger library using npm:

```bash
npm install @yigithsyn/logger
```

## Usage

### Vanilla JavaScript

```html
<script src="https://unpkg.com/@yigithsyn/logger"></script>

<script>
    console.debug('This is a debug message');
    console.log('This is a log message');
    console.info('This is an info message');
    console.warn('This is a warning message');
    console.error('This is an error message');
</script>
```

### CommonJS

```javascript
require('@yigithsyn/logger');

console.debug('This is a debug message');
console.log('This is a log message');
console.info('This is an info message');
console.warn('This is a warning message');
console.error('This is an error message');
```

### ES Modules

```javascript
import '@yigithsyn/logger';

console.debug('This is a debug message');
console.log('This is a log message');
console.info('This is an info message');
console.warn('This is a warning message');
console.error('This is an error message');
```

The ouput usually in following format:

```bash
[DEBUG] 2026-10-05T11:04:40.081Z Debug log printed
[LOG] 2026-10-05T11:04:40.087Z Log log printed
[WARN] 2026-10-05T11:04:40.087Z Warn log printed
[INFO] 2026-10-05T11:04:40.087Z Info log printed
[ERROR] 2026-10-05T11:04:40.088Z Error log printed
```

## License

This project is licensed under the BSD License.