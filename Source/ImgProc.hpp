#pragma once
#include <opencv.hpp>
#include <fstream>

//#include "Performance.hpp"
#include "Common.hpp"
#define clip(x) (x<=255?x>=0?x:0:255)
enum {MEDIAN, MAX, MIN};

template<class T> void insertion( std::vector<T> &cont, T value ) {
	typename std::vector<T>::iterator it = std::upper_bound( cont.begin(), cont.end(), value, std::less<T>() );
	cont.insert( it, value );
}

class ImgProc
{
private:
	//config 
	bool isExist;
	int Mat_num;
	std::vector<std::string>Cmds;
	std::string input_path;
	std::string output_path;
	std::string filter_path;
	//Image container
	std::vector<cv::Mat> Matvec;
	std::vector<std::vector<int> > filter;
public:
	ImgProc(std::string config_path_in);
	bool IsExist();
	int MatNum();
	void print_cmd();
	void main_flow();
	int read_grayscale(std::vector<std::string> splitted_cmd);
	int show_img(std::vector<std::string> splitted_cmd);
	int box_filter(std::vector<std::string> splitted_cmd);
	int nonlinear_filter(std::vector<std::string> splitted_cmd, int type);
	int linear_filter(std::vector<std::string> splitted_cmd);
	int sobel_filter(std::vector<std::string> splitted_cmd);
	void Convolution2D(int offset, cv::Mat *pad);

};
ImgProc::ImgProc(std::string config_path_in){
	std::fstream config_file;
	std::cout << config_path_in << std::endl;
	config_file.open(config_path_in.c_str(), std::ios::in);
	if (config_file.is_open()){
		isExist = true;
		std::cout << "%% Parsing config file \" " << config_path_in << "\"" << std::endl;
		std::string raw_line;
		while (getline(config_file, raw_line)){
			std::vector<std::string> splitted_string = Token(raw_line, ',');
			if (splitted_string.size() > 0){
				if (raw_line[0] == '#')
					std::cout << raw_line << std::endl;
				else{
					if (splitted_string[0] == "input_path"){
						input_path = splitted_string[1];
					}
					else if (splitted_string[0] == "output_path"){
						output_path = splitted_string[1];
					}
					else if (splitted_string[0] == "filter_path"){
						filter_path = splitted_string[1];
					}
					else if (splitted_string[0] == "Mat_num"){
						Mat_num = ConvertFromString<int>(splitted_string[1]);
					}
					else if (splitted_string[0] == "Command:"){
						while (getline(config_file, raw_line)){
								Cmds.push_back(raw_line);
						}
					}
				}
			}
		}
	}
	else{
		isExist = false;
		std::cout << "%% Error : config file doesn't exist." << std::endl;
	}
	if (Cmds.size() == 0)
		isExist = false;
}

bool ImgProc::IsExist(){
	return isExist;
}
int ImgProc::MatNum(){
	return Mat_num;
}
void ImgProc::print_cmd(){
	std::cout << Cmds.size() << std::endl;
	for (int i = 0; i < Cmds.size(); i++)
		std::cout << Cmds[i] << std::endl;
}


int ImgProc::read_grayscale(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 4){
		std::cout << "%% Error: Wrong cmd format in read" << std::endl;
		return 0;
	}
	int object = ConvertFromString<int>(splitted_cmd[1]);
	if (object >= Mat_num){
		std::cout << "%% Error: Object doesn't exist" << std::endl;
		return 0;
	}
	Matvec.at(object) = cv::imread(input_path + splitted_cmd[2], cv::IMREAD_GRAYSCALE);
	if (Matvec[object].cols<1 || Matvec[object].rows<1){
		std::cout << "%% Error: Read image fails" << std::endl;
		return 0;
	}
	return 1;
}
int ImgProc::show_img(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 4){
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int object = ConvertFromString<int>(splitted_cmd[1]);
	if (object >= Mat_num){
		std::cout << "%% Error: Object doesn't exist" << std::endl;
		return 0;
	}
	if (Matvec[object].cols<1 || Matvec[object].rows<1){
		std::cout << "%% Error: Image does not exist" << std::endl;
		return 0;
	}
	cv::imshow(splitted_cmd[2], Matvec[object]);
	cv::imwrite(output_path + splitted_cmd[2] + ".png", Matvec[object]);
	cvWaitKey();
	return 1;
}
int ImgProc::box_filter(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 4){
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int subject = ConvertFromString<int>(splitted_cmd[1]);
	int object = ConvertFromString<int>(splitted_cmd[2]);
	int size = ConvertFromString<int>(splitted_cmd[3]);
	filter = std::vector<std::vector<int> >(size, std::vector<int>(size, 1));
	if (object >= Mat_num || subject >= Mat_num){
		std::cout << "%% Error: Object/Subject doesn't exist" << std::endl;
		return 0;
	}
	int step = (size + 1) / 2;
	cv::Mat pad;
	padding(&Matvec[subject], &pad, step);
	Matvec[object] = cv::Mat(Matvec[subject].size(), Matvec[subject].type());
	Convolution2D(object, &pad);

	return 1;
}

int ImgProc::nonlinear_filter(std::vector<std::string> splitted_cmd, int type) {
	if (splitted_cmd.size() != 4) {
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int subject = ConvertFromString<int>(splitted_cmd[1]);
	int object = ConvertFromString<int>(splitted_cmd[2]);
	int size = ConvertFromString<int>(splitted_cmd[3]);
	if (object >= Mat_num || subject >= Mat_num){
		std::cout << "%% Error: Object/Subject doesn't exist" << std::endl;
		return 0;
	}
	int step = (size + 1) / 2;
	cv::Mat pad;
	padding(&Matvec[subject], &pad, step);
	Matvec[object] = cv::Mat(Matvec[subject].size(), Matvec[subject].type());
	uchar *pad_ptr, *obj_ptr;
	std::vector<uchar> elem;
	for (int i = 0; i < Matvec[object].rows; i++){
		obj_ptr = Matvec[object].ptr(i);
		for (int j = 0; j < Matvec[object].cols; j++){
			elem.clear();
			for (int ix = -step; ix <= step; ix++){
				pad_ptr = pad.ptr(i + step + ix, j);
				for (int jx = -step; jx <= step; jx++){
					insertion(elem, *pad_ptr++);
				}
			}
			switch(type) {
				case MEDIAN:
					*obj_ptr++ = elem[(elem.size()+1)/2];
					break;
				case MAX:
					*obj_ptr++ = elem[elem.size()-1];
					break;
				case MIN:
					*obj_ptr++ = elem[0];
					break;
			}
		}
	}
	return 1;
}

int ImgProc::linear_filter(std::vector<std::string> splitted_cmd) {
	if (splitted_cmd.size() != 4) {
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int subject = ConvertFromString<int>(splitted_cmd[1]);
	int object = ConvertFromString<int>(splitted_cmd[2]);
	filter.clear();
	
	std::fstream filter_file;
	std::cout << filter_path << splitted_cmd[3] << std::endl;
	filter_file.open(filter_path + splitted_cmd[3], std::ios::in);
	if (filter_file.is_open()){
		std::cout << "%% Parsing filter file \" " << filter_path + splitted_cmd[3] << "\"" << std::endl;
		std::string raw_line;
		while (getline(filter_file, raw_line)){
			filter.push_back(IntToken(raw_line, ' '));
		}
	} else {
		std::cout << "%% Error: File Not Exist." << std::endl;
		return 0;
	}

	if (object >= Mat_num || subject >= Mat_num){
		std::cout << "%% Error: Object/Subject doesn't exist" << std::endl;
		return 0;
	}
	cv::Mat pad;
	padding(&Matvec[subject], &pad, (int)(filter.size()+1) / 2);
	Matvec[object] = cv::Mat(Matvec[subject].size(), Matvec[subject].type());
	Convolution2D(object, &pad);

	return 1;
}

int ImgProc::sobel_filter(std::vector<std::string> splitted_cmd) {
	if(splitted_cmd.size() != 4) {
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int sobelx = ConvertFromString<int>(splitted_cmd[1]);
	int sobely = sobelx+1;
	int object = ConvertFromString<int>(splitted_cmd[2]);

	Matvec[object] = cv::Mat(Matvec[sobelx].size(), Matvec[sobelx].type());
	for(int i = 0; i < Matvec[object].rows; i++) {
		uchar *obj_ptr = Matvec[object].ptr(i);
		uchar *x_ptr = Matvec[sobelx].ptr(i);
		uchar *y_ptr = Matvec[sobely].ptr(i);
		for(int j = 0; j < Matvec[object].cols; j++) {
			*obj_ptr++ = sqrt(*x_ptr * *x_ptr + *y_ptr * *y_ptr);
			x_ptr++;
			y_ptr++;
		}
	}
	return 1;
}


void ImgProc::Convolution2D(int offset, cv::Mat *pad) {
	int size = (int)filter.size();
	int sum, num;
	for(int i = 0; i < Matvec[offset].rows; i++) {
		uchar *obj_ptr = Matvec[offset].ptr(i);
		for(int j = 0; j < Matvec[offset].cols; j++) {
			sum = 0;
			num = 0;
			for(int ix = 0; ix < size; ix++) {
				uchar *pad_ptr = pad->ptr(i + ix, j);
				for(int jx = 0; jx < size; jx++) {
					sum += filter[ix][jx] * *pad_ptr++;
					num += filter[ix][jx];
				}
			}
			if(num != 0)
				*obj_ptr++ = int(clip(sum/num) + 0.5);
			else
				*obj_ptr++ = int(clip(sum) + 0.5);
		}
	}
}

void ImgProc::main_flow(){
	//Create Mat objects
	cv::Mat A;
	for (int i = 0; i < Mat_num; i++){
		Matvec.push_back(A);
	}
	
	int i;
	for (i = 0; i < Cmds.size(); i++){
		//parse cmd
		if(Cmds[i][0] == '#') {
			cv::destroyAllWindows();
			std::cout << Cmds[i] << std::endl;
		} else {
			std::cout << "%% " << Cmds[i].substr(0,Cmds[i].size()-1) << " ..." << std::endl;
			std::vector<std::string> splitted_cmd = Token(Cmds[i], ',');
			splitted_cmd[3] = splitted_cmd[3].substr(0,splitted_cmd[3].size()-1);
			
			if (splitted_cmd[0] == "read_grayscale"){
				if (!read_grayscale(splitted_cmd))
					break;
			}
			else if (splitted_cmd[0] == "show_img"){
				if (!show_img(splitted_cmd))
					break;
			}
			else if (splitted_cmd[0] == "box_filter"){
				assert(box_filter(splitted_cmd));
			}
			else if (splitted_cmd[0] == "median_filter") {
				assert(nonlinear_filter(splitted_cmd, MEDIAN));
			}
			else if (splitted_cmd[0] == "max_filter") {
				assert(nonlinear_filter(splitted_cmd, MAX));
			}
			else if (splitted_cmd[0] == "min_filter") {
				assert(nonlinear_filter(splitted_cmd, MIN));
			}
			else if (splitted_cmd[0] == "linear_filter") {
				assert(linear_filter(splitted_cmd));
			}
			else if (splitted_cmd[0] == "sobel_filter") {
				assert(sobel_filter(splitted_cmd));
			}
			
		}
	}
	
	cv::waitKey();
}
