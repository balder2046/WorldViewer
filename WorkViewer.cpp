// WorkViewer.cpp : Defines the entry point for the console application.
//

#include "GL/glew.h"
#include "GL/glut.h"
#include <string.h>
#include <iostream>
#include <fstream>
// Sample includes
#include "Mirror.h"
#include "TrackingViewer.hpp"
// ZED includes
#include <sl/Camera.hpp>
#include "TestTri.h"
#include "Mirror.h"

void display();
void update();
using namespace std;
//// Create ZED object (camera, callback, pose)
std::thread zed_callback; //Thread to handle ZED data
sl::Camera zed; //ZED Camera
sl::Pose camera_pose; //ZED SDK pose to handle the position of the camera in space.

					  //// States
bool exit_ = false; //boolean for exit

					// OpenGL Window to display the ZED in world space
TrackingViewer viewer;

// CSV log file to store Motion Tracking data (with timestamp)
std::string csvName;

//// Sample functions
void startZED();
void run();
void close();
void transformPose(sl::Transform &pose, float tx);
int main(int argc,char **argv)
{
	//return mainMirror(argc, argv);
	//return mainTestTri(argc, argv);

	// Open the ZED
		// Initialize OpenGL viewer
	viewer.init();

	// Start ZED callback
	startZED();

	// Set GLUT callback
	glutCloseFunc(close);
	glutMainLoop();

	return 0;

    return 0;
}

/**
*  This functions start the ZED's thread that grab images and data.
**/
void startZED()
{
	exit_ = false;
	zed_callback = std::thread(run);
}



/**
*  This function loops to get images and data from the ZED. It can be considered as a callback.
*  You can add your own code here.
**/
void run() {

	float tx, ty, tz = 0;
	float rx, ry, rz = 0;

	// Get the translation from the left eye to the center of the camera
	float camera_left_to_center = .0f;//zed.getCameraInformation().calibration_parameters.T.x *0.5f;

	// Create text for GUI
	char text_rotation[256];
	char text_translation[256];

	// If asked, create the CSV motion tracking file
	ofstream outputFile;
	if (!csvName.empty()) {
		outputFile.open(csvName + ".csv");
		if (!outputFile.is_open())
			cout << "WARNING: Can't create CSV file. Launch the sample with Administrator rights" << endl;
		else
			outputFile << "Timestamp(ns);Rotation_X(rad);Rotation_Y(rad);Rotation_Z(rad);Position_X(m);Position_Y(m);Position_Z(m);" << endl;
	}

	// loop until exit_ flag has been set to true
	while (!exit_)
	{
		if (false && !zed.grab()) {
			// Get camera position in World frame
			sl::TRACKING_STATE tracking_state = zed.getPosition(camera_pose, sl::REFERENCE_FRAME_WORLD);

			// Get motion tracking confidence
			int tracking_confidence = camera_pose.pose_confidence;

			if (tracking_state == sl::TRACKING_STATE_OK) {

				// Extract 3x1 rotation from pose
				sl::Vector3<float> rotation = camera_pose.getRotationVector();
				rx = rotation.x;
				ry = rotation.y;
				rz = rotation.z;

				// Extract translation from pose
				sl::Vector3<float> translation = camera_pose.getTranslation();
				tx = translation.tx;
				ty = translation.ty;
				tz = translation.tz;

				// Fill text
				sprintf(text_translation, "%3.2f; %3.2f; %3.2f", tx, ty, tz);
				sprintf(text_rotation, "%3.2f; %3.2f; %3.2f", rx, ry, rz);

				/// Save the pose data in the csv file
				if (outputFile.is_open())
					outputFile << zed.getCameraTimestamp() << "; " << text_translation << "; " << text_rotation << ";" << endl;

				// Separate the tracking frame (reference for pose) and the camera frame (reference for the image)
				// to have the pose given at the center of the camera. If you are not using this function, the tracking frame and camera frame will be the same (Left sensor).
				// In a more generic way, the formulae is as follow : Pose(new reference frame) = M.inverse() * Pose (camera frame) * M, where M is the transform to go from one frame to another.
				// Here, to move it at the center, we just add half a baseline to "transform.translation.tx".
				transformPose(camera_pose.pose_data, camera_left_to_center);

				// Send all the information to the viewer
				viewer.updateZEDPosition(camera_pose.pose_data);
			}

			// Even if tracking state is not OK, send the text (state, confidence, values) to the viewer
			viewer.updateText(string(text_translation), string(text_rotation), tracking_state);
		}
		else sl::sleep_ms(1);
	}
}



/**
*  This function frees and close the ZED, its callback(thread) and the viewer
**/
void close() {
	exit_ = true;
	zed_callback.join();
	viewer.exit();
}


/**
*  This function separates the camera frame and the motion tracking frame.
*  In this sample, we move the motion tracking frame to the center of the ZED ( baseline/2 for the X translation)
*  By default, the camera frame and the motion tracking frame are at the same place: the left sensor of the ZED.
**/
void transformPose(sl::Transform &pose, float tx) {
	sl::Transform transform_;
	transform_.setIdentity(); // Create the transformation matrix to separate camera frame and motion tracking frame
	transform_.tx = tx; // Move the tracking frame at the center of the ZED (between ZED's eyes)
	pose = sl::Transform::inverse(transform_) * pose * transform_; // apply the transformation
}






