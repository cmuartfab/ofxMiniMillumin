#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowTitle("MiniMilumin");
    ofSetWindowShape(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    ofSetFrameRate(60);
    
    ofEnableAntiAliasing();
    
    puppetStageClient.setup();
    puppetStageClient.setServerName(PUPPET_STAGE_SYPHON_NAME);
    
    puppetParadePathClient.setup();
    puppetParadePathClient.setServerName(PUPPET_PARADE_PATH_SYPHON_NAME);
    
    test.load("test.jpg");
    
    hoveredOverVertexIndex = -1;
    
    // Top left corner
    originalCorners [0].set(INITIAL_DISPLAY_OFFSET_X,
                            INITIAL_DISPLAY_OFFSET_Y);
    distortedCorners[0].set(INITIAL_DISPLAY_OFFSET_X,
                            INITIAL_DISPLAY_OFFSET_Y);
    
    // Top right corner
    originalCorners [1].set(DEFAULT_WINDOW_WIDTH - INITIAL_DISPLAY_OFFSET_X,
                            INITIAL_DISPLAY_OFFSET_X);
    distortedCorners[1].set(DEFAULT_WINDOW_WIDTH - INITIAL_DISPLAY_OFFSET_X,
                            INITIAL_DISPLAY_OFFSET_X);
    
    // Bottom right corner
    originalCorners [2].set(DEFAULT_WINDOW_WIDTH  - INITIAL_DISPLAY_OFFSET_X,
                            DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y);
    distortedCorners[2].set(DEFAULT_WINDOW_WIDTH  - INITIAL_DISPLAY_OFFSET_X,
                            DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y);
    
    // Bottom left corner
    originalCorners [3].set(INITIAL_DISPLAY_OFFSET_X,
                            DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y);
    distortedCorners[3].set(INITIAL_DISPLAY_OFFSET_X,
                            DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y);
}

void ofApp::update(){
    
}

void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();
    
    ofPushMatrix(); {
        ofMultMatrix(homography);
        
        ofSetColor(255, 255, 255);
        if(drawTestImage) {
            drawGrid(INITIAL_DISPLAY_OFFSET_X,
                     INITIAL_DISPLAY_OFFSET_Y,
                     20,
                     20,
                     DEFAULT_WINDOW_WIDTH  - INITIAL_DISPLAY_OFFSET_X * 2,
                     DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y * 2);
        } else if (drawPuppetParadePath) {
            puppetParadePathClient
                .draw(INITIAL_DISPLAY_OFFSET_X,
                      INITIAL_DISPLAY_OFFSET_Y,
                      DEFAULT_WINDOW_WIDTH  - INITIAL_DISPLAY_OFFSET_X * 2,
                      DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y * 2);
        } else {
            puppetStageClient
                .draw(INITIAL_DISPLAY_OFFSET_X,
                      INITIAL_DISPLAY_OFFSET_Y,
                      DEFAULT_WINDOW_WIDTH  - INITIAL_DISPLAY_OFFSET_X * 2,
                      DEFAULT_WINDOW_HEIGHT - INITIAL_DISPLAY_OFFSET_Y * 2);
        }
    } ofPopMatrix();
    
    if(hoveredOverVertexIndex != -1) {
        ofVec3f v = distortedCorners[hoveredOverVertexIndex];
        ofSetColor(255,255,255);
        ofDrawCircle(v.x, v.y, SELECTED_VERTEX_CIRCLE_SIZE);
    }
    
}

void ofApp::drawGrid (int xPos, int yPos, int xDivs, int yDivs, int w, int h) {
    for(int x = 0; x <= xDivs; x++) {
        ofDrawLine(xPos + (x * w / xDivs), yPos,
                   xPos + (x * w / xDivs), yPos + h);
    }
    for(int y = 0; y <= yDivs; y++) {
        ofDrawLine(xPos,     yPos + (y * h / yDivs),
                   xPos + w, yPos + (y * h / yDivs));
    }
}

void ofApp::keyPressed (int key) {
    
    /* Toggle fullscreen */
    if (key == 'f') {
        ofToggleFullscreen();
    }
    
    /* Refresh syphon input */
    if (key == 'r') {
        puppetStageClient.setup();
        puppetStageClient.setServerName(PUPPET_STAGE_SYPHON_NAME);
        
        puppetParadePathClient.setup();
        puppetParadePathClient.setServerName(PUPPET_PARADE_PATH_SYPHON_NAME);
    }

    /* Toggle show test image */
    if (key == 't') {
        if(!drawTestImage) {
            drawTestImage = true;
            drawPuppetParadePath = false;
        } else {
            drawTestImage = false;
            drawPuppetParadePath = false;
        }
    }
    
    /* Toggle show parade path */
    if (key == 'p') {
        if(!drawPuppetParadePath) {
            drawPuppetParadePath = true;
            drawTestImage = false;
        } else {
            drawPuppetParadePath = false;
            drawTestImage = false;
        }
    }
    
}

void ofApp::keyReleased (int key) {
    
}

void ofApp::mouseMoved (int x, int y) {
    hoveredOverVertexIndex = -1;
    
    for(int i = 0; i < 4; i++) {
        ofVec2f v = distortedCorners[i];
        ofVec2f m = ofVec2f(x,y);
        float d = v.distance(m);
        if(d < VERTEX_SELECT_THRESHOLD) {
            hoveredOverVertexIndex = i;
        }
    }
    
}

void ofApp::mouseDragged(int x, int y, int button){
    if(hoveredOverVertexIndex != -1) {
        distortedCorners[hoveredOverVertexIndex].set(x, y);
        homography = ofxHomography::findHomography(originalCorners, distortedCorners);
    }
}

void ofApp::mousePressed(int x, int y, int button){
    
}

void ofApp::mouseReleased(int x, int y, int button){
    
}

void ofApp::windowResized(int w, int h){
    
}

void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
