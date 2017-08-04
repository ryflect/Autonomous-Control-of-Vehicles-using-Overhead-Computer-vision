#include <stdio.h>
#include <math.h>
#include "opencv2/highgui.hpp"

#define PI 3.1415926535

using namespace std;
using namespace cv;

class orientation_control_system
{	private:
	//Path is integer Mat object with each row containing 2 values - First X then Y
	//Next row contains next point in the path, and so on till the end
	Mat Path;
	int num_points_skip;
	int look_ahead_distance;
	int num_path_points;
	double proportional_constant;
	//Speed limits lie between 0 - 1
	double upper_speed_limit;
	double lower_speed_limit;

	public:
	orientation_control_system(Mat input_Path, int input_num_points_skip = 0, int input_look_ahead_distance = 3, double input_proportional_constant = 1, input_upper_speed_limit = 0.8, input_lower_speed_limit = 0.4)
	{
		Path = input_Path;
		num_points_skip = input_num_points_skip;
		look_ahead_distance = input_look_ahead_distance;
		num_path_points = Path.rows;
		proportional_constant = input_proportional_constant;
		upper_speed_limit = input_upper_speed_limit;
		lower_speed_limit = input_lower_speed_limit;
	}

	void find_perpendicular_distance(int point1_X, int point1_Y, int point2_X, int point2_Y, double * perpendicular_distance)
	{
		*perpendicular_distance = sqrt(pow((double)point1_X - (double)point2_X,2) + pow((double)point1_Y - (double)point2_Y,2));
	}

	void find_angle(int currentpos_tail_X, int currentpos_tail_Y, int forwardpt_head_X, int forwardpt_head_Y, double * orientation)
	{
		*orientation = atan2((double)forwardpt_head_Y - (double)currentpos_tail_Y, (double)forwardpt_head_X - currentpos_tail_X);
		if (*orientation < 0)
			*orientation = *orientation + 2*PI;
	}

	void find_closest_path_coordinate(int current_X, int current_Y, int * closest_point_index)
	{	
		int min_distance_index = -1;
		double min_distance = 1000000;
		double perpendicular_distance = 0;

		for (int iter = 0; iter < num_path_points; iter = iter + 1 + num_points_skip)
		{
			find_perpendicular_distance(current_X, current_Y, Path.at<int>(iter,0), Path.at<int>(iter,1), &perpendicular_distance);

			if (perpendicular_distance < min_distance)
			{
				min_distance = perpendicular_distance;
				min_distance_index = iter;
			}

		}

		if min_distance_index == -1
		{
			printf("\nError: Could not find closest point to current position in function - find_closest_path_coordinate");
			exit()
		}

		*closest_point_index = min_distance_index;

	}

	void calculate_desired_orientation(int current_X, int current_Y, double * desired_orientation)
	{	
		int closest_point_index;

		find_closest_path_coordinate(current_X, current_Y, &closest_point_index);

		int forwardpt_index = closest_point_index + look_ahead_distance;

		if forwardpt_index >= num_path_points
			*desired_orientation = -1; // End of Path reached
		else
			find_angle(current_X, current_Y, Path.at<int>(forwardpt_index, 0), Path.at<int>(forwardpt_index, 1), desired_orientation);

	}

	void angle_error_correction(double error_angle, double * corrected_error_angle)
	{
		if error_angle < -PI
			corrected_error_angle = error_angle + 2*PI;
		else if error_angle > PI
			corrected_error_angle = error_angle - 2*PI;
		else
			corrected_error_angle = error_angle;
	}

	void controller(double corrected_error_angle, int * steering_microsteps, int * speed)
	{	
		double steering_angle, fraction_speed;

		steering_angle = proportional_constant * corrected_error_angle;
		if steering_angle < -PI/2
			steering_angle = -PI/2;
		else if steering_angle > PI/2
			steering_angle = PI/2;

		double abs_steering_angle = abs(steering_angle);

		double slope = (upper_speed_limit - lower_speed_limit) / (-PI / 2);
		double yintercept = upper_speed_limit;

		fraction_speed = (slope * abs_steering_angle) + yintercept;

		*steering_microsteps = int(steering_angle / (PI/100));
		*speed = int(fraction_speed * 127);

	}

	void orientation_control_system_driver(int current_X, int current_Y, double current_orientation, bool * EndOfPathReached, int * steering_microsteps, int * speed)
	{
		//1.Use image acquisition folder to get images from camera in proper sync
		//2.Use stitching folder to stitch the images
		//3.Use template matching folder to get current_X, current_Y and current_orientation
		//4.Calculate the desired orientation
		double desired_orientation, error_angle, corrected_error_angle;

		calculate_desired_orientation(current_X, current_Y, &desired_orientation);
		if (desired_orientation == -1)
		{	
			//End of Path reached - Send steering_angle and speed 0 over bluetooth
			*steering_microsteps = 0;
			*speed = 0;
			*EndOfPathReached = TRUE;
		}
		else
		{
			error_angle = desired_orientation - current_orientation;
			//5.Perform angle correction
			angle_error_correction(error_angle, &corrected_error_angle);
			//6.Send error to controller to get desired input signals
			controller(corrected_error_angle, steering_microsteps, speed);
			*EndOfPathReached = FALSE;
		}

	}

};