#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableDepthTest();

	for (auto v = 0; v < 360; v += 5) {

		for (auto u = 0; u < 360; u += 8) {

			this->location_list.push_back(this->make_point(200, 50, u, v));
			this->color_list.push_back(ofColor(0));
		}
	}

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face_mesh.clear();
	this->frame_mesh.clear();

	auto R = 200;
	auto r = 50;
	auto v_span = 5;
	auto u_span = 8;

	for (int k = 0; k < 3; k++) {

		auto location = this->make_point(200, 50, ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0065), 0, 1, -360, 360), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0065), 0, 1, -360, 360));

		for (int i = 0; i < this->location_list.size(); i++) {

			auto distance = glm::distance(location, this->location_list[i]);

			if (distance < 50) {

				if (k == 0) {

					auto r = this->color_list[i].r + ofMap(distance, 0, 50, 0, 255);
					this->color_list[i].r = r < 255 ? r : 255;
				}
				else if(k == 1) {

					auto g = this->color_list[i].g + ofMap(distance, 0, 50, 0, 255);
					this->color_list[i].g = g < 255 ? g : 255;
				}
				else {

					auto b = this->color_list[i].b + ofMap(distance, 0, 50, 0, 255);
					this->color_list[i].b = b < 255 ? b : 255;
				}
			}
			else {

				if (k == 0) {

					auto r = this->color_list[i].r - 5;
					this->color_list[i].r = r > 0 ? r : 0;
				}
				else if (k == 1) {

					auto g = this->color_list[i].g - 5;
					this->color_list[i].g = g > 0 ? g : 0;
				}
				else {
					
					auto b = this->color_list[i].b - 5;
					this->color_list[i].b = b > 0 ? b : 0;
				}
			}
		}
	}

	int color_index = 0;
	for (auto v = 0; v < 360; v += v_span) {

		for (auto u = 0; u < 360; u += u_span) {
			
			vector<glm::vec3> vertices;
			vertices.push_back(this->make_point(R, r, u, v));
			vertices.push_back(this->make_point(R, r, u + u_span, v));
			vertices.push_back(this->make_point(R, r, u + u_span, v + v_span));
			vertices.push_back(this->make_point(R, r, u, v + v_span));

			int index = this->face_mesh.getNumVertices();
			this->face_mesh.addVertices(vertices);
			this->frame_mesh.addVertices(vertices);

			// Face Index

			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 1); this->face_mesh.addIndex(index + 2);
			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 3);

			// Frame Index

			this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 1);
			this->frame_mesh.addIndex(index + 1); this->frame_mesh.addIndex(index + 2);
			this->frame_mesh.addIndex(index + 2); this->frame_mesh.addIndex(index + 3);
			this->frame_mesh.addIndex(index + 3); this->frame_mesh.addIndex(index + 0);

			this->face_mesh.addColor(this->color_list[color_index]);
			this->face_mesh.addColor(this->color_list[color_index]);
			this->face_mesh.addColor(this->color_list[color_index]);
			this->face_mesh.addColor(this->color_list[color_index]);

			color_index++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.6666666666666666);

	this->face_mesh.drawFaces();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
