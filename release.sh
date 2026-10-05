#!/usr/bin/bash

ver=$(jq -r '.version' package.json)
echo "- Releasing version: $ver ..."

printf "  - Creating tag v%s ... " "$ver"
git tag v$ver && git push origin v$ver
printf "DONE\n"

printf "  - Publishing version %s to NPM registry ... " "$ver"
npm publish --access public
printf "DONE\n"
