//============================================================================
// Name        : Assignment2.cpp
// Author      : Gorkem Bedel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

void add_apartment(string name, string position, int max_bandwidth,struct apartment*& head);
vector<vector<string> > read_input_file(string s);
void write_apartments(apartment* &head);
void list_apartments(apartment* &head,fstream &outputfile);
void add_flat(apartment* &head,string name, int position, int initial_bandwith, int flat_id);
apartment* remove_apartment(apartment* &head, string name);
void make_flat_empty(apartment* &head, string name, int flat_id);
int find_sum_of_max_bandwidth(apartment* &head,fstream &outputfile);
apartment* merge_two_apartments(apartment* &head, string to_be_added, string to_be_deleted);
void relocate_flats_to_same_apartment(apartment* &head, string apartment_name, int before_flat_id, vector<string> flats);







struct flat{
		struct flat* next;
		struct flat* prev;
		int bandwidth;
		int id;
		bool is_empty;
	};


struct apartment{
	int number_of_flats;
	string name;
	int max_bandwidth;
	struct flat *headFlat;
	struct apartment* next;

};

//*********************************************************************************************************

int main(int argc, char** argv) {
	apartment* head = NULL;
	string file_name = argv[1]; 
	string output_txt = argv[2];
	fstream myFile;
	myFile.open(output_txt.c_str(), ios::out);
	vector<vector<string> > vect = read_input_file(file_name);
	int size_of_list = vect.size();
	myFile<<"starting"<<endl;
	for(int i = 0; i < size_of_list; i++){
		if(vect[i][0] == "add_apartment"){
			string apartment_name = vect[i][1];
			string position = vect[i][2];
			string string_max_bandwith = vect[i][3];
			int max_bandwith;
			stringstream converter(string_max_bandwith);
			converter >> max_bandwith;
			add_apartment(apartment_name, position , max_bandwith, head);
		}

		if(vect[i][0] == "add_flat"){
			string name = vect[i][1];
			string string_position = vect[i][2];
			string string_initial_bandwith = vect[i][3];
			string string_flat_id = vect[i][4];
			stringstream converter1(string_position);
			stringstream converter2(string_initial_bandwith);
			stringstream converter3(string_flat_id);
			int position;
			int initial_bandwith;
			int flat_id;
			converter1 >> position;
			converter2 >> initial_bandwith;
			converter3 >> flat_id;
			add_flat(head, name, position, initial_bandwith, flat_id);
		}
		if(vect[i][0] == "remove_apartment"){
			remove_apartment(head, vect[i][1]);

		}
		if(vect[i][0] == "make_flat_empty"){
			string name = vect[i][1];
			string string_flat_id = vect[i][2];
			stringstream converter1(string_flat_id);
			int flat_id;
			converter1 >> flat_id;
			make_flat_empty(head, name, flat_id);
		}
		if(vect[i][0] == "find_sum_of_max_bandwidths"){
			find_sum_of_max_bandwidth(head,myFile);
		}
		if(vect[i][0] == "merge_two_apartments"){
			merge_two_apartments(head, vect[i][1], vect[i][2]);
		}
		if(vect[i][0] == "relocate_flats_to_same_apartment"){
			string numbers_without_brackets = vect[i][3].substr(1,(vect[i][3].length()-2));
			stringstream ss(numbers_without_brackets);
			vector<string> vector;

			while(ss.good()){
		        string substr;
		        getline(ss, substr, ',');
		        vector.push_back(substr);
			}
			string apartment = vect[i][1];
			string string_flat_id_before = vect[i][2];
			stringstream converter1(string_flat_id_before);
			int flat_id_before;
			converter1 >> flat_id_before;
			relocate_flats_to_same_apartment(head, apartment, flat_id_before, vector);



		}
		if(vect[i][0] == "list_apartments"){
			list_apartments(head,myFile);
		}


	}
	myFile<<"ending"<<endl;
	myFile.close();
	return 0;
}

//*********************************************************************************************************

void add_apartment(string name, string position, int max_bandwidth,struct apartment* &head){



	apartment* new_apartment = new apartment();
	new_apartment->name = name;
	new_apartment->max_bandwidth = max_bandwidth;
	new_apartment->headFlat = NULL;
	new_apartment->next = NULL;
	new_apartment->number_of_flats = 0;


	if(position == "head"){
		head = new_apartment;
		new_apartment->next = new_apartment;
		return;
	}
	apartment* ptr = head;
	if(position[0] == 'a' ){
		char c = position[6];
		string s(1,c);
		while(ptr->name != s){
			ptr = ptr->next; // ptr = kendisinden sonra eklenecek olan apartman
		}
		if(ptr->next == head){
			ptr->next = new_apartment;
			new_apartment -> next = head;
		}else{
			apartment* ptr2 = ptr->next;
			ptr->next = new_apartment;
			new_apartment->next = ptr2;
		}
	}


	if(position[0] == 'b'){
		char c = position[7];
		string s(1,c);

		if(ptr->name == s){ //head should be updated
			apartment* lastPtr = head;
			while(lastPtr->next != head){
				lastPtr = lastPtr->next;
			}
			new_apartment->next = ptr;
			lastPtr->next = new_apartment;
			head = new_apartment;
		}else{
			while(ptr->next->name != s){
				ptr = ptr->next;
			}
			apartment* ptr2 = ptr->next;
			ptr->next = new_apartment;
			new_apartment->next = ptr2;
		}


	}

}

vector<vector<string> > read_input_file(string s){
	fstream MyReadFile;
	string lines;
	vector<vector<string> > mainvect;
	vector<string> vect;
	MyReadFile.open(s.c_str(), ios::in);
	if(MyReadFile.is_open()){
		while(getline(MyReadFile,lines)){
			vector<string> vect;
			stringstream iss(lines);
			string columns;
			while(getline(iss,columns,'\t')){
				vect.push_back(columns);
			}
		mainvect.push_back(vect);

	}
	}

	return mainvect;

}

void write_apartments(struct apartment* &head){
	apartment* ptr = head;

	if(ptr->next != head){
		while(ptr->next != head){
		cout<<ptr->name<<"("<<ptr->max_bandwidth<<")"<<" -> ";
		ptr = ptr->next;
		}
		cout<<ptr->name<<"("<<ptr->max_bandwidth<<")";
	}else{
		cout<<ptr->name<<"("<<ptr->max_bandwidth<<")";
	}

	cout<<endl;
}

void list_apartments(apartment* &head,fstream &outputfile){
	apartment* ptr = head;


	if (head != NULL) {
		do {
			flat* flat_ptr = ptr->headFlat;
			outputfile << ptr->name<<"("<<ptr->max_bandwidth<<")"<<"->";
			if(ptr->number_of_flats > 0){
				while(flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
					flat_ptr = flat_ptr->prev;
				}
				for(int i = 0; i < ptr->number_of_flats; i++){
					outputfile<<"Flat"<<flat_ptr->id<<"("<<flat_ptr->bandwidth<<")";

					if(flat_ptr->next != NULL){
						outputfile<<"\t";

						flat_ptr = flat_ptr->next;
					}
				}
				outputfile<<endl;

			}else{
				outputfile<<endl;

			}

	         ptr = ptr->next;
	        } while (ptr != head);
	    }
	else{
		outputfile<<"There is no apartment"<<endl;
	}
	outputfile<<endl;


}

void add_flat(apartment* &head,string name, int position, int initial_bandwith, int flat_id){
	int current_position = 0;

	apartment* ptr = head;

	while(ptr->name != name){
		ptr = ptr->next;
	}
	flat* flat_ptr= ptr->headFlat;

	flat* new_flat = new flat();
	new_flat->id = flat_id;
	new_flat->is_empty = false;
	new_flat->next = NULL;
	new_flat->prev = NULL;

	int current_bandwidth = 0;
	if(ptr->headFlat != NULL){ // İlk flatten sonrakiler için
		while(flat_ptr != NULL){
			current_bandwidth += flat_ptr->bandwidth; // mevcut flatlerin bandwithleri toplanıyor
			flat_ptr = flat_ptr->next;
		}
		if(initial_bandwith + current_bandwidth <= ptr->max_bandwidth){  // MAX bandwithi aşmıyorsa
			new_flat->bandwidth = initial_bandwith;
		}else{
			new_flat->bandwidth = (ptr->max_bandwidth - current_bandwidth); // Max bandwithi aşıyorsa flate alabileceği max bandwith atanıyor
			if(new_flat->bandwidth == 0){ // Flatin bandwithi 0 ise is_empty = true yapılıyor
				new_flat->is_empty = true;
			}
		}
	}else{  // İlk flat için
		if(initial_bandwith <= ptr->max_bandwidth){ //Eklenecek ilk flatin initial bandwidthi ile apartmanın max bandwithi karşılaştırılıyor.
			new_flat->bandwidth = initial_bandwith;
		}else{ // Eğer flatin initial bandwidthi apartmanın max bandwithinden fazla ise
			new_flat->bandwidth = ptr->max_bandwidth;
		}
	}


	if(position == 0 && ptr->headFlat == NULL){
			ptr->headFlat = new_flat;
			ptr->number_of_flats ++;

	}else if(position == 0  && ptr->headFlat != NULL){
		new_flat->next = ptr->headFlat;
		ptr->headFlat->prev = new_flat;
		ptr->headFlat = new_flat;
		ptr->number_of_flats ++;
	}
	else{
		while(current_position != position-1){
			ptr->headFlat = ptr->headFlat->next;
			current_position ++;
		}

		if(ptr->headFlat->next == NULL){
			ptr->headFlat->next = new_flat;
			new_flat->prev = ptr->headFlat;
			if(ptr->headFlat != NULL){
				while(ptr->headFlat->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
								ptr->headFlat = ptr->headFlat->prev;
							}
			}

			ptr->number_of_flats ++;

		}else{
			flat* flatpointer = ptr->headFlat->next;
			ptr->headFlat->next = new_flat;
			new_flat->prev = ptr->headFlat;
			new_flat->next = flatpointer;
			flatpointer->prev = new_flat;
			ptr->number_of_flats ++;
		}

	}


}

apartment* remove_apartment(apartment* &head, string name){
	apartment* ptr = head;
	apartment* ptr2;



	if(head == NULL){ // ıf there is no apartment
		return NULL;
	}

	if(ptr->next == ptr && ptr->name == name){ // ıf there is 1 apartment and that is the deleted one
		delete ptr;
		ptr = NULL;
		head = NULL;
		return NULL;
	}

	if(ptr->name == name){
		while(ptr->next != head){
			ptr = ptr->next;
		}
		ptr->next = head->next;
		delete head;
		head = ptr->next;
		return head;
	}

	while(ptr->next != head && ptr->next->name != name){
		ptr = ptr->next;
	}

	if(ptr->next->name == name){
		if(ptr->next != NULL){
			ptr2 = ptr->next; //ptr2 is the deleted node
		}
		if(ptr2->next != NULL){
			ptr->next = ptr2->next;
		}
		delete ptr2;
		ptr2 == NULL;
	}else{
		cout<<"The apartment who is going to delete was not found.";
	}
	return head;

}

void make_flat_empty(apartment* &head, string name, int flat_id){
	apartment* ptr = head;


	while(ptr->name != name){
			ptr = ptr->next;
		}
	flat* flat_ptr = ptr->headFlat;
	if(flat_ptr != NULL){
		while(flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
			flat_ptr = flat_ptr->prev;
		}
	}

	if(ptr->name == name){
		while(flat_ptr->id != flat_id ){
			flat_ptr = flat_ptr->next;
		}
	}
	
	if(flat_ptr->id == flat_id){
		flat_ptr->is_empty = true;
		flat_ptr->bandwidth = 0;
	}
}

int find_sum_of_max_bandwidth(apartment* &head,fstream &outputfile){
	apartment* ptr = head;

	int sum_of_bandwidth = 0;

	if (head != NULL) {
	        do {
	    		sum_of_bandwidth += ptr->max_bandwidth;
	    		ptr = ptr->next;
	        } while (ptr != head);
	    }
	outputfile<<"Sum of bandwidth :"<<sum_of_bandwidth<<endl;
	outputfile<<endl;
	
	return sum_of_bandwidth;


}

apartment* merge_two_apartments(apartment* &head, string to_be_added, string to_be_deleted){
	apartment* first_pointer = head;
	apartment* second_pointer = head;

	while(first_pointer->name != to_be_added){
		first_pointer = first_pointer->next;
	}
	while(second_pointer->name != to_be_deleted){
		second_pointer = second_pointer->next;
	}

	flat* to_be_added_flat_ptr = first_pointer->headFlat;

	if(to_be_added_flat_ptr != NULL){
		while(to_be_added_flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde sağa kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
			to_be_added_flat_ptr = to_be_added_flat_ptr->prev;
		}
	}

	flat* to_be_deleted_flat_ptr = second_pointer->headFlat;

	if(to_be_deleted_flat_ptr != NULL){
		while(to_be_deleted_flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
			to_be_deleted_flat_ptr = to_be_deleted_flat_ptr->prev;
		}
	}

	if(to_be_added_flat_ptr != NULL){
		while(to_be_added_flat_ptr->next != NULL){
			to_be_added_flat_ptr = to_be_added_flat_ptr->next;
		}
	}
	if(to_be_deleted_flat_ptr != NULL && to_be_added_flat_ptr != NULL){
		to_be_added_flat_ptr->next = to_be_deleted_flat_ptr;
		to_be_deleted_flat_ptr->prev = to_be_added_flat_ptr;
		first_pointer->number_of_flats += second_pointer->number_of_flats;
		second_pointer->number_of_flats = 0;
		first_pointer->max_bandwidth += second_pointer->max_bandwidth;
	}else if(to_be_deleted_flat_ptr != NULL && to_be_added_flat_ptr == NULL){
		first_pointer->number_of_flats += second_pointer->number_of_flats;
		second_pointer->number_of_flats = 0;
		first_pointer->max_bandwidth += second_pointer->max_bandwidth;
		first_pointer->headFlat = to_be_deleted_flat_ptr;


	}else if(to_be_deleted_flat_ptr == NULL && to_be_added_flat_ptr != NULL){
		first_pointer->number_of_flats += second_pointer->number_of_flats;
		second_pointer->number_of_flats = 0;
		first_pointer->max_bandwidth += second_pointer->max_bandwidth;




	}else if(to_be_deleted_flat_ptr == NULL && to_be_added_flat_ptr == NULL){
		first_pointer->number_of_flats += second_pointer->number_of_flats;
		second_pointer->number_of_flats = 0;
		first_pointer->max_bandwidth += second_pointer->max_bandwidth;


	}

	remove_apartment(head, to_be_deleted);
	return head;



}

void relocate_flats_to_same_apartment(apartment* &head, string apartment_name, int before_flat_id, vector<string> flats){

	apartment* ptr_same = head;
	apartment* ptr_app_flat = head;

	int size_of_vector = flats.size();
	vector<int> int_vector;
	for(int i = 0; i < size_of_vector; i++){
		string string_flat_id = flats.at(i);
		stringstream converter1(string_flat_id);
		int flat_id;
		converter1 >> flat_id;
		int_vector.push_back(flat_id);
	}
	while(ptr_same->name != apartment_name){
		ptr_same = ptr_same->next; //ptr_same flatlarin taşınacağı apartmanı gösteriyor.
	}


	flat* ptr_same_flat = ptr_same->headFlat;
	if(ptr_same_flat != NULL){
		while(ptr_same_flat->prev != NULL){	// headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
		ptr_same_flat = ptr_same_flat->prev;
		}
	}




	if(ptr_same_flat->id == before_flat_id){
	}else{
		while(ptr_same_flat->id != before_flat_id){
				ptr_same_flat = ptr_same_flat->next; // eklenecek apartmanda, kendisinden önce eklenecek olan flat bulundu. bunun previne eklenecek
			}
	}



	for(int j = 0; j < size_of_vector; j++){
		ptr_app_flat = head;
		while(ptr_app_flat->next != head){
			flat* flat_ptr = ptr_app_flat->headFlat;
			if(flat_ptr != NULL){
				while(flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
					flat_ptr = flat_ptr->prev;
						}
			}

			if(flat_ptr->id == int_vector.at(j)){

														if(flat_ptr->next != NULL && flat_ptr->prev == NULL){  // İLKİ ama devamı var

															flat* flat_ptr_next = flat_ptr->next;

															flat_ptr->next = NULL;
															flat_ptr->prev = NULL;
															flat_ptr_next->prev = NULL;

															ptr_app_flat->headFlat = flat_ptr_next;

															ptr_app_flat->number_of_flats --;
															ptr_same->number_of_flats ++;
															ptr_same->max_bandwidth += flat_ptr->bandwidth;
															ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

															if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
																flat* ptr_same_flat_prev = ptr_same_flat->prev;
																ptr_same_flat_prev->next = flat_ptr;
																flat_ptr->prev = ptr_same_flat_prev;
																ptr_same_flat->prev = flat_ptr;
																flat_ptr->next = ptr_same_flat;
															}else{ //apartmanın head flatinden önce eklenecekse
																ptr_same_flat->prev = flat_ptr;
																flat_ptr->next = ptr_same_flat;
																ptr_same->headFlat = flat_ptr;
															}

														}

														else if(flat_ptr->next == NULL && flat_ptr->prev == NULL){ //ilk ve tek

															ptr_app_flat->number_of_flats --;
															ptr_same->number_of_flats ++;
															ptr_same->max_bandwidth += flat_ptr->bandwidth;
															ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

															if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
																flat* ptr_same_flat_prev = ptr_same_flat->prev;
																ptr_same_flat_prev->next = flat_ptr;
																flat_ptr->prev = ptr_same_flat_prev;
																ptr_same_flat->prev = flat_ptr;
																flat_ptr->next = ptr_same_flat;
															}else{ //apartmanın head flatinden önce eklenecekse
																ptr_same_flat->prev = flat_ptr;
																flat_ptr->next = ptr_same_flat;
																ptr_same->headFlat = flat_ptr;
															}
															ptr_app_flat->headFlat = NULL;
														}
			}else{



			for(int x = 0; x < ptr_app_flat->number_of_flats-1; x++){
				flat_ptr = flat_ptr->next;
				if(flat_ptr->id == int_vector.at(j)){
										if(flat_ptr->prev != NULL && flat_ptr->next != NULL){ // ortada
											flat* flat_ptr_prev = flat_ptr->prev;
											flat* flat_ptr_next = flat_ptr->next;


											flat_ptr_prev->next = flat_ptr_next;
											flat_ptr_next->prev = flat_ptr_prev;

											flat_ptr->next = NULL;
											flat_ptr->prev = NULL;

											ptr_app_flat->number_of_flats --;
											ptr_same->number_of_flats ++;
											ptr_same->max_bandwidth += flat_ptr->bandwidth;
											ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;
											if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
												flat* ptr_same_flat_prev = ptr_same_flat->prev;
												ptr_same_flat_prev->next = flat_ptr;
												flat_ptr->prev = ptr_same_flat_prev;
												ptr_same_flat->prev = flat_ptr;
												flat_ptr->next = ptr_same_flat;

											}else{ //apartmanın head flatinden önce eklenecekse

												ptr_same_flat->prev = flat_ptr;
												flat_ptr->next = ptr_same_flat;
												ptr_same->headFlat = flat_ptr;

											}


										}

										else if(flat_ptr->next == NULL && flat_ptr->prev != NULL){ //sonda
											flat* flat_ptr_prev = flat_ptr->prev;
											flat_ptr_prev->next = NULL;

											flat_ptr->next = NULL;
											flat_ptr->prev = NULL;

											ptr_app_flat->number_of_flats --;
											ptr_same->number_of_flats ++;
											ptr_same->max_bandwidth += flat_ptr->bandwidth;
											ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

											if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
												flat* ptr_same_flat_prev = ptr_same_flat->prev;
												ptr_same_flat_prev->next = flat_ptr;
												flat_ptr->prev = ptr_same_flat_prev;
												ptr_same_flat->prev = flat_ptr;
												flat_ptr->next = ptr_same_flat;
											}else{ //apartmanın head flatinden önce eklenecekse
												ptr_same_flat->prev = flat_ptr;
												flat_ptr->next = ptr_same_flat;
												ptr_same->headFlat = flat_ptr;
											}

										}

				}

			}
		}
			ptr_app_flat = ptr_app_flat->next;
			if(ptr_app_flat->headFlat == NULL && ptr_app_flat->next != head){
				ptr_app_flat = ptr_app_flat->next;
				if(ptr_app_flat->headFlat == NULL && ptr_app_flat->next != head){
								ptr_app_flat = ptr_app_flat->next;
								if(ptr_app_flat->headFlat == NULL && ptr_app_flat->next != head){
												ptr_app_flat = ptr_app_flat->next;
												if(ptr_app_flat->headFlat == NULL && ptr_app_flat->next != head){
													ptr_app_flat = ptr_app_flat->next;
												}

											}
							}
			}

		}


		if(ptr_app_flat->headFlat != NULL){
		flat* flat_ptr = ptr_app_flat->headFlat;
		flat_ptr = ptr_app_flat->headFlat;

					if(flat_ptr != NULL){
						while(flat_ptr->prev != NULL){ // headflat pointeri nedenini anlamadığım bi şekilde  kaydığı için en başa çektirdim. sonra çözümüne bir daha bak
							flat_ptr = flat_ptr->prev;
								}
					}

					if(flat_ptr->id == int_vector.at(j)){

																if(flat_ptr->next != NULL && flat_ptr->prev == NULL){  // İLKİ ama devamı var

																	flat* flat_ptr_next = flat_ptr->next;

																	flat_ptr->next = NULL;
																	flat_ptr->prev = NULL;
																	flat_ptr_next->prev = NULL;

																	ptr_app_flat->headFlat = flat_ptr_next;

																	ptr_app_flat->number_of_flats --;
																	ptr_same->number_of_flats ++;
																	ptr_same->max_bandwidth += flat_ptr->bandwidth;
																	ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

																	if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
																		flat* ptr_same_flat_prev = ptr_same_flat->prev;
																		ptr_same_flat_prev->next = flat_ptr;
																		flat_ptr->prev = ptr_same_flat_prev;
																		ptr_same_flat->prev = flat_ptr;
																		flat_ptr->next = ptr_same_flat;
																	}else{ //apartmanın head flatinden önce eklenecekse
																		ptr_same_flat->prev = flat_ptr;
																		flat_ptr->next = ptr_same_flat;
																		ptr_same->headFlat = flat_ptr;
																	}

																}

																else if(flat_ptr->next == NULL && flat_ptr->prev == NULL){ //ilk ve tek

																	ptr_app_flat->number_of_flats --;
																	ptr_same->number_of_flats ++;
																	ptr_same->max_bandwidth += flat_ptr->bandwidth;
																	ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

																	if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
																		flat* ptr_same_flat_prev = ptr_same_flat->prev;
																		ptr_same_flat_prev->next = flat_ptr;
																		flat_ptr->prev = ptr_same_flat_prev;
																		ptr_same_flat->prev = flat_ptr;
																		flat_ptr->next = ptr_same_flat;
																	}else{ //apartmanın head flatinden önce eklenecekse
																		ptr_same_flat->prev = flat_ptr;
																		flat_ptr->next = ptr_same_flat;
																		ptr_same->headFlat = flat_ptr;
																	}
																	ptr_app_flat->headFlat = NULL;
																}
					}else{



					for(int x = 0; x < ptr_app_flat->number_of_flats-1; x++){
						flat_ptr = flat_ptr->next;
						if(flat_ptr->id == int_vector.at(j)){
												if(flat_ptr->prev != NULL && flat_ptr->next != NULL){ // ortada
													flat* flat_ptr_prev = flat_ptr->prev;
													flat* flat_ptr_next = flat_ptr->next;


													flat_ptr_prev->next = flat_ptr_next;
													flat_ptr_next->prev = flat_ptr_prev;

													flat_ptr->next = NULL;
													flat_ptr->prev = NULL;

													ptr_app_flat->number_of_flats --;
													ptr_same->number_of_flats ++;
													ptr_same->max_bandwidth += flat_ptr->bandwidth;
													ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;
													if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
														flat* ptr_same_flat_prev = ptr_same_flat->prev;
														ptr_same_flat_prev->next = flat_ptr;
														flat_ptr->prev = ptr_same_flat_prev;
														ptr_same_flat->prev = flat_ptr;
														flat_ptr->next = ptr_same_flat;

													}else{ //apartmanın head flatinden önce eklenecekse

														ptr_same_flat->prev = flat_ptr;
														flat_ptr->next = ptr_same_flat;
														ptr_same->headFlat = flat_ptr;

													}


												}

												else if(flat_ptr->next == NULL && flat_ptr->prev != NULL){ //sonda
													flat* flat_ptr_prev = flat_ptr->prev;
													flat_ptr_prev->next = NULL;

													flat_ptr->next = NULL;
													flat_ptr->prev = NULL;

													ptr_app_flat->number_of_flats --;
													ptr_same->number_of_flats ++;
													ptr_same->max_bandwidth += flat_ptr->bandwidth;
													ptr_app_flat->max_bandwidth -= flat_ptr->bandwidth;

													if(ptr_same_flat->prev != NULL){ //kendisinden önce eklenecek olan flat ilk flat değilse
														flat* ptr_same_flat_prev = ptr_same_flat->prev;
														ptr_same_flat_prev->next = flat_ptr;
														flat_ptr->prev = ptr_same_flat_prev;
														ptr_same_flat->prev = flat_ptr;
														flat_ptr->next = ptr_same_flat;
													}else{ //apartmanın head flatinden önce eklenecekse
														ptr_same_flat->prev = flat_ptr;
														flat_ptr->next = ptr_same_flat;
														ptr_same->headFlat = flat_ptr;
													}

												}

						}

					}
				}
				}
	}//for loop






















}


