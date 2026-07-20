#!/bin/bash
rsync -av --delete \
  --exclude 'Intermediate' \
  --exclude 'Binaries' \
  /Users/yoshitsugukosaka/Developments/ue5_av_system/Plugins/SyphonLink/ \
  /Users/yoshitsugukosaka/Documents/Test/SyphonePlugin/Plugins/SyphonLink/
echo "Synced LVis -> Test. Now: cd /Users/yoshitsugukosaka/Documents/Test/SyphonePlugin && git add -A && git commit"
