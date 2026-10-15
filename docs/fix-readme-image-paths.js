import fs from 'node:fs';

const indexPath = new URL('./index.html', import.meta.url);
const html = fs.readFileSync(indexPath, 'utf8');
const fixedHtml = html.replace(/(src=['"])docs\/img\//g, '$1./img/');

fs.writeFileSync(indexPath, fixedHtml);
