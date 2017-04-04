#include<iostream> //feature 4
#include<fstream>
#include<ctime>
#include<unordered_map>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<ctype.h> //for isdigit
using namespace std;

template <typename T1, typename T2>
struct descendingMode{      //comparison method for std::sort
    typedef pair<T1, T2> type;
    bool operator() (type const & a, type const & b) const{
        return a.second > b.second;
	}
};

template <typename T1, typename T2>
struct keySort{      //comparison method for std::sort
    typedef pair<T1, T2> type;
    bool operator() (type const & a, type const & b) const{
        return a.first < b.first;
	}
};

class timeClass{
    public:
	    int DD, HH, MM, SS;
        long absTime;
        string timeStamp; //format = start without '['
    long clockToAbsTime(){
        return ((DD-1) * (24*3600) + HH * 3600 + MM * 60 + SS);
    }
    void set_Time(string str){
    //input is the timestamp
    //output = set all member according to the input
        DD = stoi(str.substr(0,2));
    	HH = stoi(str.substr(12,2));
    	MM = stoi(str.substr(15,2));
        SS = stoi(str.substr(18,2));
        absTime = clockToAbsTime();
        timeStamp = str;
	}
    void set_Time(long second){ 
	//input is the absolute time in second
    //output = set all member according to the input
		DD = second / 86400 + 1;
        HH = second % 86400 / 3600;
        MM = second % 3600 / 60;
        SS = second % 60;
        string d_field, h_field, m_field, s_field;
        if(DD < 10){
            d_field = "0" + to_string(DD);
		}
		else{
            d_field = to_string(DD);
		}
        if(HH < 10){
            h_field = "0" + to_string(HH);
		}
		else{
            h_field = to_string(HH);
		}
        if(MM < 10){
            m_field = "0" + to_string(MM);
		}
		else{
            m_field = to_string(MM);
		}
        if(SS < 10){
            s_field = "0" + to_string(SS);
		}
		else{
            s_field = to_string(SS);
		}
        timeStamp = d_field + "/Jul/1995:" + h_field + ":" + m_field + ":" + s_field;
        absTime = second;
	}
};

int main(){
    clock_t time1, time2, time3; //for measure program time
    time1 = clock();
	long i, ii, j, freqCount, sum;
    long timeDiff, blockListSize, trackListSize;
	unordered_map<string, long> log_UMap; //use map to get data from log.txt
    log_UMap.reserve(2000000);
    string str, IPName, loginStatus;
    string timeStr;
	stringstream strStream;
    ifstream in_Stream;
    in_Stream.open("log.txt"); //log file
    ofstream out,out2;
	out.open("out.txt");
	out2.open("blocked.txt");  //output file for the top-10 visitor
    timeClass scan_tClass, ref_tClass;
	//reference at 01/Jul 00:00:00
    //additional codes are needed if data expand beyond 1 month period

//**************** Calculation part 1, combine the same timestamp together
//the output will be unordered_map, which will be transferred to vector for sorting

	i = 0; //creating a fail attempt map
    while(!in_Stream.eof()){
	    in_Stream >> IPName; //get IP name
	    in_Stream.ignore(1000,'\"');//move to HTTP reply part
        std::getline(in_Stream,str); //POST /login HTTP/1.0" 401  == fail login
        str = str.substr(0, 26);
        if(str == "POST /login HTTP/1.0\" 401 "){
            if(log_UMap.count(IPName) == 0){
			    log_UMap.insert(make_pair(IPName,1));
			}
			else{
			    log_UMap[IPName] += 1;
			}
		}
        i++;
  		if(i%10000 == 0){ //show the work progress every 100000 line
			time3 = clock();
           	std::cout << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	std::cout << " complete " << i << " lines\n";
			out << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	out << " i = " << i << "\n";
		}
	}
    //copy map to vector, then sort
    vector<pair<string, long>> log_Vec(log_UMap.begin(), log_UMap.end());
    sort(log_Vec.begin(), log_Vec.end(), descendingMode<string, long>());

    for(auto& it:log_Vec){
        std::cout << it.second << "\t" << it.first <<'\n';
	}

    long vecSize;
    vecSize = log_Vec.size();

    for(ii = 0; ii < vecSize; ii++){
        if(log_Vec[ii].second < 3)
            break; //ii = the starting point to be < 3
	}
    log_UMap.clear();
    log_UMap.reserve(10000);
    for(i = 0; i < ii; i++){
        log_UMap.insert(make_pair(log_Vec[i].first,log_Vec[i].second));
	}
	unordered_multimap<string, string> track_UMM; // ip : timestamp
    unordered_multimap<string, string> toBeBlocked_UMM; // ip : timestamp
    track_UMM.reserve(100000);
    toBeBlocked_UMM.reserve(100000);
    in_Stream.close();
    in_Stream.open("log.txt");

    i = 0;
    while(!in_Stream.eof()){
	    in_Stream >> IPName;
        if(log_UMap.count(IPName) == 0){
            std::getline(in_Stream,str);
            i++;
            continue; //skip if the name is not in the list
		}
		else{
            //processing the input line to get timestamp, login status
			std::getline(in_Stream, str);
            strStream.str(str);
            timeStr = str.substr(6,20);
			//update toBeBlocked_UMap
            scan_tClass.set_Time(timeStr);
            if(!toBeBlocked_UMM.empty()){
            	unordered_multimap<string,string>::iterator it;
				for(j = 0;;){
				    it = toBeBlocked_UMM.begin();
                    advance(it,j);
					if(it == toBeBlocked_UMM.end()) break;
					ref_tClass.set_Time((*it).second);
                    timeDiff = scan_tClass.absTime - ref_tClass.absTime;
                    if(timeDiff > 300){
                        toBeBlocked_UMM.erase(it);
					}
					else{
                        j++;
					}
				}
			}
            //check if IPName is still in the updated to be blocked list
			if(toBeBlocked_UMM.count(IPName) > 0){
				out2 << IPName << str << '\n';
                std::cout << IPName << str << '\n';
                i++;
                continue;
			}
            loginStatus = str.substr(35, 26);            
            //update track_UMap to remove any expire record
            if(!track_UMM.empty()){
            	unordered_multimap<string,string>::iterator it;
				for(j = 0;;){
				    it = track_UMM.begin();
                    advance(it,j);
					if(it == track_UMM.end()) break;
					ref_tClass.set_Time((*it).second);
                    timeDiff = scan_tClass.absTime - ref_tClass.absTime;
                    if(timeDiff > 20){
                        track_UMM.erase(it);
					}
					else{
                        j++;
					}
				}
			}
            //update track+ to be blocked list
            if(loginStatus == "POST /login HTTP/1.0\" 401 "){
                track_UMM.insert(make_pair(IPName,timeStr));
                if(track_UMM.count(IPName) >= 3){
                    //add into tobeblocked list
                    toBeBlocked_UMM.insert(make_pair(IPName,timeStr));
				}
			}
            if(loginStatus == "POST /login HTTP/1.0\" 200 "){
                track_UMM.erase(IPName);
			}
            //otherwise it is just "GET
			i++;
		}
	}

    time2 = clock();    //print the result to file and screen
    std::cout << "total line number = " << i << '\n';
	std::cout << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	std::cout << "\n\nDONE";
    out << "total line number = " << i << '\n';
	out << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	out << "\n\nDONE";

    str = str;
}
