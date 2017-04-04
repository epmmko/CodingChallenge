#include<iostream>
#include<fstream>
#include<ctime>
#include<unordered_map>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
using namespace std;

template <typename T1, typename T2>
struct descendingMode{      //comparison method for std::sort
    typedef pair<T1, T2> type;
    bool operator() (type const & a, type const & b) const{
        return a.second > b.second;
	}
};

int main(){
    clock_t time1, time2, time3; //for measure program time
    time1 = clock();
	long i, sum;
	unordered_map<string, int> log_UMap; //use map to get data from log.txt
    log_UMap.reserve(2000000);
    string str;
    ifstream in_Stream;
    in_Stream.open("log.txt"); //log file
    ofstream out,out2;
	out.open("out.txt");
	out2.open("hosts.txt");  //output file for the top-10 visitor
	stringstream line_Stream;

    i = 0;
    while(!in_Stream.eof()){
        in_Stream >> str;   //start reading log.txt
        in_Stream.ignore(1000,'\n');//take just the IP/web part
		if(log_UMap.count(str) == 0){  //if new key, add to the map
            log_UMap.insert(make_pair(str,1));
	    }
        else{
            log_UMap[str] += 1; //if old key, increase the count on value
	    }
        i++;
   		if(i%100000 == 0){ //show the work progress every 100000 line
			time3 = clock();
           	cout << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	cout << " complete " << i << " lines\n";
			out << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	out << " i = " << i << "\n";
		}

	}
    //copy map to vector and then sort
    vector<pair<string, int>> log_Vec(log_UMap.begin(), log_UMap.end());
    std::sort(log_Vec.begin(), log_Vec.end(), descendingMode<string, int>());

    for(int j = 0; j< 10; j++){
        out2 << log_Vec[j].first  <<',' << log_Vec[j].second << endl;
        cout << log_Vec[j].second << "\t" << log_Vec[j].first << '\n';
	}
    sum = 0; //recheck the cumulative frequency
    for(auto &it: log_Vec){
        sum += it.second;
	}

    time2 = clock();    //print the result to file and screen
    std::cout << "total line number = " << i << '\n';
    std::cout << "total count = " << sum << '\n';
	std::cout << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	std::cout << "\n\nDONE";
    out << "total line number = " << i << '\n';
    out << "total count = " << sum << '\n';
	out << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	out << "\n\nDONE";

    out.close();
    out2.close();
}