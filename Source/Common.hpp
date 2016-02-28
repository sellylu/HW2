#pragma once
#include<opencv.hpp>
//Config Parsing Utilities
template <class T>	T ConvertFromString(const std::string &s) {
	T value;
	std::stringstream ss(s);
	ss >> value;
	return value;
}
std::vector<int> IntToken(std::string in, char delimeter){
	std::istringstream iss(in);
	std::vector<int> elements;
	std::string item;
	while (getline(iss, item, delimeter)){
		int value;
		std::stringstream ss(item);
		ss >> value;
		elements.push_back(value);
	}
	return elements;
}
std::vector<std::string> Token(std::string in, char delimeter) {
	std::istringstream iss(in);
	std::vector<std::string> elements;
	std::string item;
	while (getline(iss, item, delimeter)){
		elements.push_back(item);
	}
	return elements;
}
int padding(cv::Mat* input, cv::Mat* pad, int step){
	if (!pad->empty())
		pad->release();
	*pad = cv::Mat(input->rows + step * 2, input->cols + step * 2, input->type());
	int range;
	range = input->channels();
	//cout << "type : " << input->elemSize1() << endl;
	//cout << "range : " << range << endl;
	switch (input->elemSize1()) {
	case 1:{
			   //cout << "into float" << endl;
			   uchar *p1, *inptr0, *inptr;
			   for (int i = 0; i<pad->rows; i++) {
				   if (i < step)
					   inptr0 = input->ptr<uchar>(step - i - 1);
				   else if (i >= input->rows + step)
					   inptr0 = input->ptr<uchar>(input->rows - (i - input->rows - step) - 1);
				   else
					   inptr0 = input->ptr<uchar>(i - step);
				   for (int j = 0; j < pad->cols; j++) {
					   p1 = pad->ptr<uchar>(i, j);
					   if (j < step)
						   inptr = inptr0 + (step - j - 1)*range;
					   else if (j >= input->cols + step)
						   inptr = inptr0 + (input->cols - (j - input->cols - step) - 1)*range;
					   else
						   inptr = inptr0 + (j - step)*range;
					   for (int d = 0; d < range; d++) {
						   p1[d] = inptr[d];
					   }
				   }
			   }
	}break;
	case 2:{
			   //cout << "into float" << endl;
			   short *p1, *inptr0, *inptr;
			   for (int i = 0; i<pad->rows; i++) {
				   if (i < step)
					   inptr0 = input->ptr<short>(step - i - 1);
				   else if (i >= input->rows + step)
					   inptr0 = input->ptr<short>(input->rows - (i - input->rows - step) - 1);
				   else
					   inptr0 = input->ptr<short>(i - step);
				   for (int j = 0; j < pad->cols; j++) {
					   p1 = pad->ptr<short>(i, j);
					   if (j < step)
						   inptr = inptr0 + (step - j - 1)*range;
					   else if (j >= input->cols + step)
						   inptr = inptr0 + (input->cols - (j - input->cols - step) - 1)*range;
					   else
						   inptr = inptr0 + (j - step)*range;
					   for (int d = 0; d < range; d++) {
						   p1[d] = inptr[d];
					   }
				   }
			   }
	}break;
	case 4:{
			   //cout << "into float" << endl;
			   float *p1, *inptr0, *inptr;
			   for (int i = 0; i<pad->rows; i++) {
				   if (i < step)
					   inptr0 = input->ptr<float>(step - i - 1);
				   else if (i >= input->rows + step)
					   inptr0 = input->ptr<float>(input->rows - (i - input->rows - step) - 1);
				   else
					   inptr0 = input->ptr<float>(i - step);
				   for (int j = 0; j < pad->cols; j++) {
					   p1 = pad->ptr<float>(i, j);
					   if (j < step)
						   inptr = inptr0 + (step - j - 1)*range;
					   else if (j >= input->cols + step)
						   inptr = inptr0 + (input->cols - (j - input->cols - step) - 1)*range;
					   else
						   inptr = inptr0 + (j - step)*range;
					   for (int d = 0; d < range; d++) {
						   p1[d] = inptr[d];
					   }
				   }
			   }
	}break;
	case 8:{
			   //cout << "into double" << endl;
			   double *p1, *inptr0, *inptr;
			   for (int i = 0; i<pad->rows; i++) {
				   if (i < step)
					   inptr0 = input->ptr<double>(step - i - 1);
				   else if (i >= input->rows + step)
					   inptr0 = input->ptr<double>(input->rows - (i - input->rows - step) - 1);
				   else
					   inptr0 = input->ptr<double>(i - step);
				   for (int j = 0; j < pad->cols; j++) {
					   p1 = pad->ptr<double>(i, j);
					   if (j < step)
						   inptr = inptr0 + (step - j - 1)*range;
					   else if (j >= input->cols + step)
						   inptr = inptr0 + (input->cols - (j - input->cols - step) - 1)*range;
					   else
						   inptr = inptr0 + (j - step)*range;
					   for (int d = 0; d < range; d++) {
						   p1[d] = inptr[d];
					   }
				   }
			   }
	}break;
	default:
		break;
	}
	return 0;

}
