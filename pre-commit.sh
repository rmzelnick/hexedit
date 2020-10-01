#!/bin/sh

STASH_NAME="pre-commit-$(date +%s)"
git stash save -q --keep-index $STASH_NAME

# Test prospective commit
./bootstrap.sh
./configure
make check
cd tests
./test
RESULT=$?
cd..

git stash pop -q
[ $RESULT -ne 0 ] && exit 1
exit 0

STASHES=$(git stash list)
if [[ $STASHES == "$STASH_NAME" ]]; then
  git stash pop -q
fi

# source: https://codeinthehole.com/tips/tips-for-using-a-git-pre-commit-hook/
