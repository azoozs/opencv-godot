/* opencv.cpp */

#include "opencv.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
//#include <fstream>
#include <vector>
#include <iostream>
#include <string>

//#include <openssl/bio.h>
//#include <openssl/evp.h>
//#include <openssl/buffer.h>

// android
#include <filesystem>
#include <fstream>
#include <core/io/file_access.h>
#include <core/variant/dictionary.h>



// my side libraries


void opencv::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_image_from_path", "path"), &opencv::load_image_from_path);
	ClassDB::bind_method(D_METHOD("get_opencv_version"), &opencv::get_opencv_version);
	ClassDB::bind_method(D_METHOD("read_base64_img_as_opencv_img", "base64_string"), &opencv::read_base64_img_as_opencv_img);


	// start video capture #################################################################################################
	ClassDB::bind_method(D_METHOD("set_video_bool", "true_or_false"), &opencv::set_video_bool);
	ClassDB::bind_method(D_METHOD("get_video_bool"), &opencv::get_video_bool);

	ClassDB::bind_method(D_METHOD("get_frame"), &opencv::get_frame);
	ClassDB::bind_method(D_METHOD("set_frame","frame"), &opencv::set_frame);
	ClassDB::bind_method(D_METHOD("video_capture"), &opencv::video_capture);
	// end video capture ###################################################################################################
}





String opencv::get_opencv_version() {
	return CV_VERSION;
}

Ref<ImageTexture> opencv::load_image_from_path(String path) {
	// Convert Godot string to C++ string
	std::string cpp_path = path.utf8().get_data();

	// Load image using OpenCV
	cv::Mat image_opencv = cv::imread(cpp_path, cv::IMREAD_UNCHANGED);

	// Check if the image loaded successfully
	if (image_opencv.empty()) {
		std::cout << "Error: Failed to load the image from the specified path." << std::endl;
		return Ref<ImageTexture>(); // Return an empty ImageTexture
	}

	// Convert image to RGBA format
	cv::Mat rgba_image;
	cv::cvtColor(image_opencv, rgba_image, cv::COLOR_BGRA2RGBA);

	// Convert cv::Mat data to PackedByteArray
	PackedByteArray bytes;
	int width = rgba_image.cols;
	int height = rgba_image.rows;
	int sizear = width * height * rgba_image.channels();
	bytes.resize(sizear);

	if (rgba_image.isContinuous()) {
		std::memcpy(bytes.ptrw(), rgba_image.ptr(), sizear);
	} else {
		for (int i = 0; i < sizeof(rgba_image); i++) {
			std::memcpy(bytes.ptrw(), rgba_image.ptr(i), sizear);
		}
	}

	Ref<Image> img = Image::create_from_data(width, height, false, img->FORMAT_RGBA8, bytes);
	Ref<ImageTexture> imageTex = ImageTexture::create_from_image(img);

	return imageTex;
}


// start video capture #################################################################################################
Ref<ImageTexture> opencv::set_frame(Ref<ImageTexture> frame)
{
	opencv::frame = frame;
	return opencv::frame;
}

Ref<ImageTexture> opencv::get_frame()
{
	return opencv::frame;
}

bool opencv::set_video_bool(bool true_or_false)
{
	opencv::is_video = true_or_false;
	return opencv::is_video;
}

bool opencv::get_video_bool()
{
	return opencv::is_video;
}

Ref<ImageTexture> opencv::video_capture(int deviceID)
{
	cv::Mat image_opencv;
	Ref<Image> img;
	Ref<ImageTexture> imageTex;
	cv::VideoCapture cap(0);
	if(!cap.isOpened())
		std::cout << "cannot open camera" << std::endl;

	PackedByteArray bytes;
	cv::Mat mat;

	while(get_video_bool() == true)
	{
		cap >> image_opencv;
		int width = image_opencv.cols;
		int height = image_opencv.rows;
		// int sizear = width * height * image_opencv.channels();
		//bytes.resize(sizear);
  
		cv::cvtColor(image_opencv, mat, cv::COLOR_BGRA2RGBA);
		image_opencv.release();
		int sizear = width * height * mat.channels();
		
		bytes.resize(sizear);
  
		if (mat.isContinuous())
		{
			std::memcpy(bytes.ptrw(), mat.ptr(), sizear);
		}
		else
		{
		  for (int i = 0; i < sizeof(mat); i++)
		  {
				std::memcpy(bytes.ptrw(), mat.ptr(i), sizear);
		  }
		}
		
		/*img->create_from_data(width, height, false, img->FORMAT_RGB8, bytes);
		tex->create_from_image(img);*/
		img = Image::create_from_data(width, height, false, img->FORMAT_RGBA8, bytes);
		imageTex = ImageTexture::create_from_image(img);
		set_frame(imageTex);
		mat.release();
    }
	
  return imageTex;
}
// end video capture #################################################################################################




// start azooz4
static const char *B64chars = "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";

static const int B64index[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 63,
	0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

const std::string b64encode(const void *data, const size_t &len) {
	std::string result((len + 2) / 3 * 4, '=');
	unsigned char *p = (unsigned char *)data;
	char *str = &result[0];
	size_t j = 0, pad = len % 3;
	const size_t last = len - pad;

	for (size_t i = 0; i < last; i += 3) {
		int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
		str[j++] = B64chars[n >> 18];
		str[j++] = B64chars[n >> 12 & 0x3F];
		str[j++] = B64chars[n >> 6 & 0x3F];
		str[j++] = B64chars[n & 0x3F];
	}
	if (pad) /// Set padding
	{
		int n = --pad ? int(p[last]) << 8 | p[last + 1] : p[last];
		str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
		str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
		str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
	}
	return result;
}

const std::string b64decode(const void *data, const size_t &len) {
	if (len == 0)
		return "";

	unsigned char *p = (unsigned char *)data;
	size_t j = 0,
		   pad1 = len % 4 || p[len - 1] == '=',
		   pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
	const size_t last = (len - pad1) / 4 << 2;
	std::string result(last / 4 * 3 + pad1 + pad2, '\0');
	unsigned char *str = (unsigned char *)&result[0];

	for (size_t i = 0; i < last; i += 4) {
		int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
		str[j++] = n >> 16;
		str[j++] = n >> 8 & 0xFF;
		str[j++] = n & 0xFF;
	}
	if (pad1) {
		int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
		str[j++] = n >> 16;
		if (pad2) {
			n |= B64index[p[last + 2]] << 6;
			str[j++] = n >> 8 & 0xFF;
		}
	}
	return result;
}

std::string b64encode(const std::string &str) {
	return b64encode(str.c_str(), str.size());
}

std::string b64decode(const std::string &str64) {
	return b64decode(str64.c_str(), str64.size());
}


Ref<ImageTexture> opencv::read_base64_img_as_opencv_img(String base64_string) {
	// Decode the base64 string to a byte array
	std::string decoded_data = b64decode(base64_string.utf8().get_data(), base64_string.length());

	// Convert the decoded data to a vector
	std::vector<uchar> img_vector(decoded_data.begin(), decoded_data.end());

	// Decode the vector to an OpenCV image
	cv::Mat image_opencv = cv::imdecode(cv::Mat(img_vector), cv::IMREAD_UNCHANGED);

	if (image_opencv.empty()) {
		// Handle the case where the image decoding fails
		std::cout << "Image decoding failed." << std::endl;
		return Ref<ImageTexture>();
	}

	// Convert the image to RGBA format
	cv::Mat rgba_image;
	cv::cvtColor(image_opencv, rgba_image, cv::COLOR_BGRA2RGBA);

	// Convert cv::Mat data to PackedByteArray
	PackedByteArray bytes;
	int width = rgba_image.cols;
	int height = rgba_image.rows;
	int sizear = width * height * rgba_image.channels();
	bytes.resize(sizear);

	if (rgba_image.isContinuous()) {
		std::memcpy(bytes.ptrw(), rgba_image.ptr(), sizear);
	} else {
		for (int i = 0; i < rgba_image.rows; i++) {
			std::memcpy(bytes.ptrw() + i * width * rgba_image.channels(), rgba_image.ptr(i), width * rgba_image.channels());
		}
	}

	// Create an Image from the data
	Ref<Image> img = Image::create_from_data(width, height, false, Image::FORMAT_RGBA8, bytes);
	Ref<ImageTexture> imageTex = ImageTexture::create_from_image(img);

	return imageTex;
}
// end azooz4


opencv::opencv() {
    
}
