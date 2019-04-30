#!/bin/bash

# Clones dependencies for selfberry
# Inspired by ObviousJim and ofxTimeline "clone_addons.sh" script
# 20170415 was videodromm/ofxGifEncoder.git and videodromm/ofxRPiCameraVideoGrabber.git

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}julapy/ofxFFT.git

${PREFIX}comoc/ofxAbletonLink.git

${PREFIX}Kj1/ofxSpout2.git

echo "Done cloning addons! :)"
