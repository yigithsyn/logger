#!/usr/bin/env bash

set -e
set -o pipefail

# Fetch the latest changes from the origin and compare
git fetch origin
current_sha=$(git rev-parse HEAD)
previous_sha=$(git rev-parse HEAD~1)

# Compare current SHA package.json version field with the previous SHA package.json version field
echo "Comparing package.json versions between current and previous commits..."
current_version=$(git show "$current_sha":package.json | jq -r '.version')
previous_version=$(git show "$previous_sha":package.json | jq -r '.version')
echo "- Current version: $current_version"
echo "- Previous version: $previous_version"

# If current version is different from the previous version:
if [ "$current_version" != "$previous_version" ]; then
    echo "Version has changed. Processing..."
    echo "- Creating a new tag for version $current_version."
    git tag "v$current_version"
    git push origin "v$current_version"

    echo "- Creating release."
    echo v$current_version >> release-notes.md
    git log --format='- %s' "${previous_sha}..${current_sha}" > release-notes.md
    cat release-notes.md
    # gh release create "v$current_version" --notes-file release-notes.md
    # rm release-notes.md

    echo "- Publishing to NPM registry."
    npm publish --access public

fi


