#include<iostream>
#include"ImgProc.hpp"

int main(int argc, char* argv[]){
	std::cout << argv[0] << std::endl;
	std::string config_path = "../Config.csv";
	if (argc > 1)
		config_path = argv[1];
	ImgProc EXP1(config_path);
	if (EXP1.IsExist()){
		//EXP1.print_cmd();
		EXP1.main_flow();
	}
	else{
		std::cout << "Config does not exitst or no commands" << std::endl;
		system("pause");
	}
}
