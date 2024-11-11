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

clean_service_worker() {
  cd "$1"
  find . -type d -name "Service Worker" | xargs -I {} rm -rf {}
}
clean_service_worker '/Users/arun/Library/Application Support/Thorium/'
clean_service_worker '/Users/arun/Library/Application Support/Google/Chrome'

# free browser cache
rm -rf $HOME/Library/Caches/Google
rm -rf $HOME/Library/Caches/Thorium
rm -rf $HOME/Library/Caches/Firefox

# free trash
ls ~/.Trash/ | xargs -I {} rm -rf "$HOME/.Trash/{}"

# free npm cache
rm -rf ~/.npm/_cacache/tmp/*

logmsg "*********free local space finished"
notify 'Local space freed'
