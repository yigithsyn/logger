const env = require('jsdoc/env');

exports.handlers = {
    parseBegin() {
        if (typeof env.opts.readme === 'string') {
            env.opts.readme = env.opts.readme.replace(
                /(src=['"])doc\/img\//g,
                '$1../img/'
            );
        }
    }
};
