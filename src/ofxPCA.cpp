#include "ofxPCA.h"



static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);


ofxPCA::Results ofxPCA::analyze(const ofMesh& mesh){
	const auto & pts = mesh.getVertices();
	//Construct a buffer used by the pca analysis
	int sz = static_cast<int>(pts.size());
	cv::Mat data_pts = cv::Mat(sz, 3, CV_64F);
	for (int i = 0; i < data_pts.rows; i++)
	{
		data_pts.at<double>(i, 0) = pts[i].x;
		data_pts.at<double>(i, 1) = pts[i].y;
		data_pts.at<double>(i, 2) = pts[i].z;
	}
	
	
	return  _analyze(data_pts);
	
}

ofxPCA::Results ofxPCA::analyze(const cv::Mat& data_pts){

	if(data_pts.cols == 3){
		return _analyze(data_pts);
	}else if(data_pts.cols == 6){
		// remove normals before procesing
		int sz = static_cast<int>(data_pts.rows);
		cv::Mat new_data_pts = cv::Mat(sz, 3, CV_64F);
		for (int i = 0; i < data_pts.rows; i++)
		{
			for (int j = 0; j < 3; j++){
				new_data_pts.at<double>(i, j) = data_pts.at<double>(i, j);
			}
		}
		
		return _analyze(new_data_pts);
	}else{
		ofLogError("ofxPCA::analyze", "Wrong number of cols");
	}
	return ofxPCA::Results();
}

ofxPCA::Results ofxPCA::_analyze(const cv::Mat& data_pts){
	ofxPCA::Results result;
	
	//Perform PCA analysis
	cv::PCA pca_analysis(data_pts, cv::Mat(), cv::PCA::DATA_AS_ROW);
	//Store the center of the object
	//Store the center of the object
	result.center = glm::vec3 (pca_analysis.mean.at<double>(0, 0),
							   pca_analysis.mean.at<double>(0, 1),
							   pca_analysis.mean.at<double>(0, 2));
	
	//	cout << "Eigen Vectors = " << endl << " "  << pca_analysis.eigenvectors << endl << endl;
	//	cout << "Eigen Values  = " << endl << " "  << pca_analysis.eigenvalues << endl << endl;
	//Store the eigenvalues and eigenvectors
	result.vecs.resize(3);
	result.vals.resize(3);
	for (int i = 0; i < 3; i++)
	{
		result.vecs[i].x = pca_analysis.eigenvectors.at<double>(i, 0);
		result.vecs[i].y = pca_analysis.eigenvectors.at<double>(i, 1);
		result.vecs[i].z = pca_analysis.eigenvectors.at<double>(i, 2);
		
		result.vals[i] = pca_analysis.eigenvalues.at<double>(i);
	}
	return result;
	
}


void ofxPCA::Results::draw(){
	if(vecs.size() != 3 || vals.size() != 3 ) return;
	auto p1 = center + 0.02 * (vecs[0] * vals[0]);
	auto p2 = center - 0.05 * (vecs[1] * vals[1]);
	auto p3 = center - 0.02 * (vecs[2] * vals[1]);
	
	ofPushStyle();
	ofSetLineWidth(4);
	ofSetColor(cyanPrint);
	ofDrawLine(center, p1);
	ofSetColor(yellowPrint);
	ofDrawLine(center, p2);
	ofSetColor(magentaPrint);
	ofDrawLine(center, p3);
	ofPopStyle();
}

void ofxPCA::Results::flipVecs(){
	for(auto&  v: vecs){
		v = v * -1;
	}
}

string ofxPCA::Results::toString(){
	stringstream ss;
	ss << "vecs:\n";
	for(auto& v: vecs){ss << "    " << v <<endl;}
	ss << "vals:\n";
	for(auto& v: vals){ss << "    "<< v <<endl;}
	ss << "center: " << center << endl;
	
	return ss.str();
}
