//
//  ofxPCA
//
//  Created by Roy Macdonald on 6/5/21.
//
//
#pragma once

#include "ofMain.h"
#include "opencv2/core.hpp"

class ofxPCA{
public:
struct Results{
	vector<glm::vec3> vecs;
	vector<double> vals;
	glm::vec3 center;
	
	void draw();
	
	void flipVecs();
	
	string toString();
	
};

	
	static Results analyze(const ofMesh& mesh);
	static Results analyze(const cv::Mat& mat);

private:
	
	static Results _analyze(const cv::Mat& data_pts);

};
