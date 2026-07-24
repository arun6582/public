#!/usr/bin/env bash

# set -x

SNAP_FOLDER=$1

logmsg "*********free local space start"

if [[ ! -z $SNAP_FOLDER ]]; then
  logmsg 'cleaning snaps folder'
  find $SNAP_FOLDER -maxdepth 1 -mtime +60 | xargs -I {} rm -rf "{}"
fi

# free docker files
/usr/local/bin/docker system prune -af --volumes

# free trash
ls ~/.Trash/ | xargs -I {} rm -rf "$HOME/.Trash/{}"

# free npm cache
rm -rf ~/.npm/_cacache/tmp/*

# clear nix temps
nix-collect-garbage

logmsg "*********free local space finished"
# notify 'Local space freed'
