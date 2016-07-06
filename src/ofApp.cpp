#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowTitle("MiniMilumin");
    ofSetWindowShape(800, 600);
    ofSetFrameRate(60);
    
    //ofDisableArbTex();
    
    client.setup();
    client.setServerName("Dranimate_Syphon_1");
    
    mesh.load("square.ply");
    for(int i = 0; i < mesh.getVertices().size(); i++) {
        ofVec3f v = mesh.getVertex(i);
        v.x *= 600;
        v.y *= 600;
        v.z *= 600;
        mesh.setVertex(i, v);
        mesh.addTexCoord(v);
    }
    
    test.load("test.jpg");
    
    hoveredOverVertexIndex = -1;
}

void ofApp::update(){
    
}

void ofApp::draw(){
    client.draw(0, 0);
    
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();
    
    ofSetColor(255,255,255,255);
    ofPushMatrix(); {
        //ofTranslate(0, 200);
        //ofRotate(180, 1, 0, 0);
        
        client.getTexture().bind();
        mesh.drawFaces();
        client.getTexture().unbind();
    } ofPopMatrix();
    
    mesh.drawWireframe();
    
    if(hoveredOverVertexIndex != -1) {
        ofVec3f v = mesh.getVertex(hoveredOverVertexIndex);
        ofSetColor(255,255,255);
        ofDrawCircle(v.x, v.y, 5);
    }
    
}

void ofApp::keyPressed(int key){
    /* Toggle fullscreen */
    if (key == 'f') {
        ofToggleFullscreen();
    }
}

void ofApp::keyReleased(int key){
    
}

void ofApp::mouseMoved(int x, int y ) {
    hoveredOverVertexIndex = -1;
    
    for(int i = 0; i < mesh.getVertices().size(); i++) {
        ofVec2f v = mesh.getVertex(i);
        ofVec2f m = ofVec2f(x,y);
        float d = v.distance(m);
        if(d < 10) {
            hoveredOverVertexIndex = i;
        }
    }
}

void ofApp::mouseDragged(int x, int y, int button){
    if(hoveredOverVertexIndex != -1) {
        mesh.setVertex(hoveredOverVertexIndex, ofVec3f(x,y));
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
