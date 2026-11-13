#!/usr/bin/env bash
set -euo pipefail

version=$(cat VERSION)
tag="v${version}"

if git rev-parse --verify --quiet "refs/tags/${tag}" >/dev/null; then
    printf 'Error: tag %s already exists\n' "$tag" >&2
    exit 1
fi

printf 'Creating and pushing tag %s...\n' "$tag"
git tag "$tag"
git push origin "$tag"
