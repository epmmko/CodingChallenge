#include<iostream> //feature 3, top 10 busiest period
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
	int j;
	long i, freqCount, sum;
	unordered_map<string, long> log_UMap; //use map to get data from log.txt
    log_UMap.reserve(2000000);
    string str;
    ifstream in_Stream;
    in_Stream.open("log.txt"); //log file
    ofstream out,out2;
	out.open("out.txt");
	out2.open("hours.txt");  //output file for the top-10 visitor
    timeClass scanStart_tClass, scanEnd_tClass, scanning_tClass;
	//reference at 01/Jul 00:00:00
    //additional codes are needed if data expand beyond 1 month period


//**************** Calculation part 1, combine the same timestamp together
//the output will be unordered_map, which will be transferred to vector for sorting

	i = 0;
    while(!in_Stream.eof()){
        in_Stream.ignore(1000,'[');//take just the IP/web part
        in_Stream >> str;
        //ignore the rest, to start @ next line, next time
        in_Stream.ignore(1000,'\n');
		if(log_UMap.count(str) == 0){  //if new key, add to the map
            log_UMap.insert(make_pair(str,1));
	    }
        else{
            log_UMap[str] += 1; //if old key, increase the count on value
	    }
        i++;
   		if(i%100000 == 0){ //show the work progress every 100000 line
			time3 = clock();
           	std::cout << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	std::cout << " complete " << i << " lines\n";
			out << "time(s)= " << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC);
        	out << " i = " << i << "\n";
		}

	}
    time2 = clock();
    //copy map to vector and then sort
    vector<pair<string, long>> log_Vec(log_UMap.begin(), log_UMap.end());
    std::sort(log_Vec.begin(), log_Vec.end(), keySort<string, long>());

    time3 = clock();
	std::cout << "time(s) required to build vector from UMap = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';
    out << "time(s) required to build vector from UMap = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';


    time2 = clock();    //print the result to file and screen
    std::cout << "total line number = " << i << '\n';
	std::cout << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	std::cout << "\n\nDONE";
    out << "total line number = " << i << '\n';
	out << "run_time_(s)= " << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC);
	out << "\n\nDONE first part\n\n";

    i = i;
    time2 = clock();
    //start from the begin() of vector to create another vector<pair<string, long>>
    //string = time stamp, long = cumulative frequency
    vector<pair<string, long>> freq_Vec(log_Vec.begin(), log_Vec.end());
    //log_Vec.size(); = 1109780
    time3 = clock();
	std::cout << "time(s) required to duplicate vector size n = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';
    out << "time(s) required to duplicate vector size n = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';
    
//************************** Calculation Part 2: count # of visit in 1 hr window
//summation is done from t to t+3599 sec, to get the number visit in 1 hr
//The next 60 minute window will overlap with the current window
//the average is calculated by exclude the old non-overlap part and-
//-add the new non-overlap part
    time2 = clock();    
	long iOld;
    long vecSize;
    long oldSum=0, oldCumuSum, newCumuSum;
    long acStop, acOStop=0;
    bool done = false, overlap = false;
	i = 0;
    
    vecSize = freq_Vec.size(); //unique timestamp = 1109780
    std::cout << "vector size after frequencies are counted\n = " << vecSize;
    while(i < vecSize){
        scanStart_tClass.set_Time(freq_Vec[i].first);
        scanEnd_tClass.set_Time(scanStart_tClass.absTime + 3599);
        if (overlap == false){
    	    freqCount = 0;
		    for(j = 0; j < vecSize-i; j++){  //First time counting
		        //keep scaning until break
                //the upper limit is for the case of error,-
                //-no vector out of range problem
		        str = freq_Vec[i+j].first;
                scanning_tClass.set_Time(str);
                if(scanning_tClass.absTime > scanEnd_tClass.absTime){
				    break;
		        }
		        else{
                    freqCount = freqCount + freq_Vec[i+j].second;
                    acStop = i+j;
			    }
	        }
            freq_Vec[i].second = freqCount;
	    }
	    else{
        //Calculation formula newSum = oldSum - oldCumulativeSum + newCumulativeSum
            //oldCumuSum = sum from freq_Vec[iOld]  to freq_Vec[i-1]
            oldCumuSum = log_Vec[i-1].second;
            //get the new actual-stop index
		    for(j = 1; j < vecSize - acOStop; j++){
                if(freq_Vec[acOStop+j].first > scanEnd_tClass.timeStamp){
                    break;
			    }
                acStop = acOStop + j;
		    }
            newCumuSum = 0; //from actualOStop to actualStop
            for(j = acOStop + 1; j <= acStop; j++){
                newCumuSum += log_Vec[j].second;
		    }
            freqCount = oldSum - oldCumuSum + newCumuSum;
            freq_Vec[i].second = freqCount;
	    }
        //get next starting time
        //chekc if done or not
        if(i + 1 == vecSize){
            break;
	    }
        //check if the new time window overlap with the current one or not
        if(freq_Vec[i+1].first > scanEnd_tClass.timeStamp){
            overlap = false;
	    }
	    else{
            overlap = true;
	    }
        iOld = i;
        acOStop = acStop;
        oldSum = freqCount;
	    i = i + 1;

        if(i%10000 == 0){
		    time3 = clock();
            std::cout << "i = " << i << " time(s), in counting = " 
				    << 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC)
                    << " program run time(s) = "
                    << 1.0 * (time3 - time1) / (1.0 * CLOCKS_PER_SEC) << '\n';
		}
    }

    time3 = clock();
	out << "time(s) required to count the frequency in 60-min windows = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';
	std::cout << "time(s) required to count the frequency in 60-min windows = " 
		<< 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';

	time2 = clock();
    std::sort(freq_Vec.begin(), freq_Vec.end(), descendingMode<string, long>());
    time3 = clock();
    out << "time(s) required to sort the most busy time = " 
	    << 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';
    std::cout << "time(s) required to sort the most busy time = " 
	    << 1.0 * (time3 - time2) / (1.0 * CLOCKS_PER_SEC) << '\n';

	for(i = 0; i <= 9; i++){
        out2 << freq_Vec[i].first << " -0400," << freq_Vec[i].second << '\n';
	    std::cout << freq_Vec[i].first << " -0400," << freq_Vec[i].second << '\n';
	}
    time2 = clock();
    out << "\n\ntotal run time (second) = " 
	     << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC) << '\n';
	out << "done";
    std::cout << "\n\ntotal run time (second) = " 
	     << 1.0 * (time2 - time1) / (1.0 * CLOCKS_PER_SEC) << '\n';
	std::cout << "done";
    out.close();
    out2.close();
}