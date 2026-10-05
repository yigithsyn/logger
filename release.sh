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
    previous_tag=$(git describe --tags --abbrev=0 "${current_sha}^" 2>/dev/null || true)
    echo "Previous tag: $previous_tag"
    if [[ -n "$previous_tag" ]]; then
    git log --format='- %s' "${previous_tag}..${GITHUB_SHA}" > release-notes.md
    else
    git log --format='- %s' "${GITHUB_SHA}" > release-notes.md
    fi
    gh release create "v$current_version" --notes-file release-notes.md
    rm release-notes.md

    echo "- Publishing to NPM registry."
    npm publish --access public

fi


