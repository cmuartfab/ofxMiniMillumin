#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxHomography.h"

#define PUPPET_STAGE_SYPHON_NAME "Dranimate_Syphon_1"
#define PUPPET_PARADE_PATH_SYPHON_NAME "Dranimate_PuppetParadePath"

#define DEFAULT_WINDOW_WIDTH 1440
#define DEFAULT_WINDOW_HEIGHT 900

#define INITIAL_DISPLAY_OFFSET_X 50
#define INITIAL_DISPLAY_OFFSET_Y 50

#define VERTEX_SELECT_THRESHOLD 50

#define SELECTED_VERTEX_CIRCLE_SIZE 10

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void drawGrid(int xPos, int yPos, int xDivs, int yDivs, int w, int h);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxSyphonClient puppetStageClient;
    ofxSyphonClient puppetParadePathClient;
    
    ofImage test;
    
    bool drawTestImage;
    bool drawPuppetParadePath;
    
    int hoveredOverVertexIndex;
    
    ofPoint originalCorners[4];
    ofPoint distortedCorners[4];
    ofMatrix4x4 homography;
};
