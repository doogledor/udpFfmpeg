#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sampleRate = 44100;
    channels = 2;

    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(364, 280);
//    vidRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg")); // use this is you have ffmpeg installed in your data folder

    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

    vidRecorder.setFfmpegOutputOptions(" -vcodec mpeg4 -f mpegts -preset ultrafast -tune zerolatency -b 200000");
    vidRecorder2.setFfmpegOutputOptions(" -vcodec mpeg4 -f mpegts -preset ultrafast -tune zerolatency -b 200000");
    vidRecorder3.setFfmpegOutputOptions(" -vcodec mpeg4 -f mpegts -preset ultrafast -tune zerolatency -b 200000");
    vidRecorder4.setFfmpegOutputOptions(" -vcodec mpeg4 -f mpegts -preset ultrafast -tune zerolatency -b 200000");

    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);

    ofSetWindowShape(vidGrabber.getWidth(), vidGrabber.getHeight()	);

    bRecording = false;
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    if(vidGrabber.isFrameNew() && bRecording){
        ofPixels _videoPixels = vidGrabber.getPixels();
        //bool success = vidRecorder.addFrame(_videoPixels);
        bool success = vidRecorder2.addFrame(_videoPixels);
        /*vidRecorder3.addFrame(_videoPixels);
        vidRecorder4.addFrame(_videoPixels);*/
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }

    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }

    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    vidGrabber.draw(0,0);

    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;

    ofSetColor(0,0,0,100);
    ofDrawRectangle(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);

    if(bRecording){
    ofSetColor(255, 0, 0);
    ofDrawCircle(ofGetWidth() - 20, 20, 5);
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 1, "myfile.mov");
            //vidRecorder2.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 1, "udp://192.168.0.106:1235");
            //vidRecorder3.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 1, "udp://192.168.0.106:1236");
            //vidRecorder4.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, 0, 1, "udp://192.168.0.106:1237");
            // Start recording
            vidRecorder.start();
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
