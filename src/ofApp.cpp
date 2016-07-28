#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowTitle("MiniMilumin");
    ofSetWindowShape(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    ofSetFrameRate(60);
    
    ofEnableAntiAliasing();
    
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
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setAutoDraw(false);
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    
    dir.setup();
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dir.refresh(true);
    vector<ofxSyphonServerDescription> list = dir.getServerList();
    for(int i = 0; i < list.size(); i++) {
        ofLog() << list[i].serverName;
    }
}

void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        //if(gui->getButton(dir.serverName) == NULL) { //doesn't work
            gui->addButton(dir.serverName);
            gui->getButton(dir.serverName)->setVisible(true);
        //}

    }
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg){
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
        gui->getButton(dir.serverName)->setVisible(false);
    }
}

void ofApp::update(){
    gui->update();
    
    int moveCornerIndex = -1;
    
    if(oneKeyDown)   moveCornerIndex = 0;
    if(twoKeyDown)   moveCornerIndex = 1;
    if(threeKeyDown) moveCornerIndex = 2;
    if(fourKeyDown)  moveCornerIndex = 3;
    
    if(moveCornerIndex != -1) {
        if(upKeyDown)    moveHomCornerPos(moveCornerIndex, 0, -2);
        if(downKeyDown)  moveHomCornerPos(moveCornerIndex, 0, 2);
        if(leftKeyDown)  moveHomCornerPos(moveCornerIndex, -2, 0);
        if(rightKeyDown) moveHomCornerPos(moveCornerIndex, 2, 0);
    }
}

void ofApp::moveHomCornerPos(int i, int dx, int dy) {
    distortedCorners[i].set(distortedCorners[i].x + dx, distortedCorners[i].y + dy);
    homography = ofxHomography::findHomography(originalCorners, distortedCorners);
}

void ofApp::setHomCornerPos(int i, int x, int y) {
    distortedCorners[i].set(x, y);
    homography = ofxHomography::findHomography(originalCorners, distortedCorners);
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
    
    if(drawGui) {
        gui->draw();
    }
    
}

void ofApp::drawGrid (int xPos, int yPos, int xDivs, int yDivs, int w, int h) {
    ofSetLineWidth(2);
    for(int x = 0; x <= xDivs; x++) {
        ofDrawLine(xPos + (x * w / xDivs), yPos,
                   xPos + (x * w / xDivs), yPos + h);
    }
    for(int y = 0; y <= yDivs; y++) {
        ofDrawLine(xPos,     yPos + (y * h / yDivs),
                   xPos + w, yPos + (y * h / yDivs));
    }
    ofSetLineWidth(1);
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
    
    /* Toggle draw GUI */
    if (key == 'g') {
        drawGui = !drawGui;
    }
    
    /* Keys for controlling corners */
    if(key == OF_KEY_SHIFT) shiftKeyDown = true;
    if(key == OF_KEY_UP) upKeyDown = true;
    if(key == OF_KEY_DOWN) downKeyDown = true;
    if(key == OF_KEY_LEFT) leftKeyDown = true;
    if(key == OF_KEY_RIGHT) rightKeyDown = true;
    if(key == '1') oneKeyDown = true;
    if(key == '2') twoKeyDown = true;
    if(key == '3') threeKeyDown = true;
    if(key == '4') fourKeyDown = true;
    
}

void ofApp::keyReleased (int key) {
    if(key == OF_KEY_SHIFT) shiftKeyDown = false;
    if(key == OF_KEY_UP) upKeyDown = false;
    if(key == OF_KEY_DOWN) downKeyDown = false;
    if(key == OF_KEY_LEFT) leftKeyDown = false;
    if(key == OF_KEY_RIGHT) rightKeyDown = false;
    if(key == '1') oneKeyDown = false;
    if(key == '2') twoKeyDown = false;
    if(key == '3') threeKeyDown = false;
    if(key == '4') fourKeyDown = false;
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
        setHomCornerPos(hoveredOverVertexIndex, x, y);
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

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
    if(e.target->getName() == "syphon input") {
        puppetStageClient.setup();
        puppetStageClient.setServerName(e.target->getLabel());
    }
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
    puppetStageClient.setup();
    puppetStageClient.setServerName(e.target->getLabel());
    ofLog() << e.target->getLabel();
}
