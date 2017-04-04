#include<iostream>
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

int main(){
    clock_t time1, time2, time3; //for measure run time
    time1 = clock();
	int j, k, strLength;
	long i, sum;
    double bWidth;
	unordered_map<string, int> log_UMap; //use map to get data from log.txt
    log_UMap.reserve(2000000);
    string str, strLine, strBW;
    ifstream in_Stream;
    in_Stream.open("log.txt"); //log file
    ofstream out,out2;
	out.open("out.txt");
	out2.open("resources.txt");  //output file for the top-10 visitor
	stringstream line_Stream;

	i = 0; //start to process the log.txt
//need to manually delete \n at the end of the file
//(line that contain nothing).
//otherwise, there will be an error when try to read the empty line
//add the check if line is empty or not decrease the speed, so not implemented
    while(!in_Stream.eof()){
        //start extracting the bandwidth and address, line-by-line
        line_Stream.str("");
		getline(in_Stream, strLine);
        line_Stream << strLine;
		line_Stream.ignore(1000, '/'); //skip IP
        line_Stream.ignore(1000, '/'); //skip month
        line_Stream.ignore(1000, '/'); //skip year + "GET
    	line_Stream >> str;
        //get bandwidth from back
        strLength = strLine.length();
        if(isdigit(strLine[strLength-1])){
            //search for space right before the bandwidth
            for(j = strLength; j >= 0 ; j--){ 
			    if(strLine[j] == ' ') break;
		    } //j = "space" (before bandwidth) location
            strBW = ""; // k = 0, get the first letter of bWidth
            //k = length - j - 2, get at length - 1 (last digit)
            for(k = 0; k < strLength - j - 1; k++){
			    strBW.insert(strBW.end(), strLine[j+1+k]);
		    }
            bWidth = stod(strBW);
		}
		else{
            bWidth = 0;
		}
        //finish extracting the bandwidth and address, line-by-line
		//put the data into an unordered_map for counting the frequency (hit)
		if(log_UMap.count(str) == 0){  //if new key, add to the map
            log_UMap.insert(make_pair(str,bWidth));
	    }
        else{
            log_UMap[str] += bWidth; //if old key, increase the count on value
	    }
        i++;
   		if(i%10000 == 0){ //show the work progress every 100000 line
			time3 = clock();
           	cout << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	cout << " complete " << i << " lines\n";
			out << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	out << " i = " << i << "\n";
		}
	}

    //copy map to vector and then sort
    cout << "start to build vector\n";
    vector<pair<string, double>> log_Vec(log_UMap.begin(), log_UMap.end());
    cout << "start to sort\n";
    std::sort(log_Vec.begin(), log_Vec.end(), descendingMode<string, double>());
    cout << "finish sorting\n";
    out << "\n\n";

	//correct the output
	//HTTP/1.0" is actually for the URL without anything "/ "
	for(int j = 0; j< 10; j++){
		if(log_Vec[j].first == "HTTP/1.0\"")
			log_Vec[j].first = ' ';
	}

	for(int j = 0; j< 10; j++){
        out2 << '/' << log_Vec[j].first  <<'\n';
        cout << log_Vec[j].second << "\t" << '/' << log_Vec[j].first << '\n';
        out << log_Vec[j].second << "\t" << '/' << log_Vec[j].first << '\n';
	}

    time2 = clock();    //print the result to file and screen
    std::cout << "total line number = " << i << '\n';
	std::cout << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	std::cout << "\n\nDONE";
    out << "total line number = " << i << '\n';
	out << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	out << "\n\nDONE";

    out.close();
    out2.close();
}