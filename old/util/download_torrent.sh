#!/bin/bash
# This script dowload torrent files in the Download directory automatically.

DOWNLOAD_DIR=$HOME/Downloads

find $DOWNLOAD_DIR -type f -name '*.torrent' -exec bash -c "transmission-cli '{}' &"  \;

