#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <filesystem>
#include <fstream>

using namespace std;

string metaline, s1, s2, s3, path, cmd, metaline2;
char c1, c2, c3;
long long int pos = -1, temp;
float f3, f2, f1, secdur /* total length of the video in seconds */;
bool b1, b2, b3;
unsigned long long int i1, i2, i3;

void pcheck(FILE * &f, bool &pexit) { // check if child process exited
	if (f != NULL) /* if (FILE * ) makes the thread wait for child process to finish */ {pexit = true;} // pexit = true after child process has exited
}

inline string replace(string sprstr, string substr, string repstr) {
	int pos = -1;
	pos = sprstr.find(substr);
	if (pos < 0) {
		cout << "Fatal error: Substring to replace not found. (" << substr << ")" << endl;
		exit(2);
	}
	sprstr.erase(pos, substr.size());
	sprstr.insert(pos, repstr);
	return sprstr;
}

inline string remquo(string s) {
		if (s.at(0) == '\'') {s.erase(0, 1);} // remove quotation marks sorrounding string, if any
		else if (s.at(0) == '\"') {s.erase(0, 1);}
		if (s.at(s.size() - 1) == '\'') {s.erase(s.size() - 2, s.size() - 1);}
		else if (s.at(s.size() - 1) == '\"') {s.erase(s.size() - 2, s.size() - 1);}
		return s;
}

inline string chfsfx(string s, string newsfx) { // change file suffix
	s = remquo(s);
	reverse(s.begin(), s.end());
	if (s.find(".") >= 0) {s.erase(0, s.find("."));} // if there is no suffix, append new suffix and . instead of replace suffix
	else {s.append(".");}
	reverse(s.begin(), s.end());
	s.append(newsfx);
	return s;
}

inline filesystem::path chfsfx(filesystem::path p, string newsfx) { // change file suffix
	string s = p.generic_string();
	s = remquo(s);
	reverse(s.begin(), s.end());
	if (s.find(".") >= 0) {s.erase(0, s.find("."));} // if there is no suffix, append new suffix and . instead of replace suffix
	else {s.append(".");}
	reverse(s.begin(), s.end());
	s.append(newsfx);
	p = s;
	return p;
}

inline string ftog(float f) { // float to guint (or something)
	string dotstr = ".", s; // pitivi wants these in some weird format. guint or something i think. this is the only way i can think of to convert
	stringstream getsrt;
	getsrt << fixed << setprecision(9) << f;
	getline(getsrt, s);
	pos = s.find(dotstr);
	s.erase(pos, dotstr.size());
	return s;
}

inline bool isnum(char c) {
	if (c == '0') {return true;}
	else if (c == '1') {return true;}
	else if (c == '2') {return true;}
	else if (c == '3') {return true;}
	else if (c == '4') {return true;}
	else if (c == '5') {return true;}
	else if (c == '6') {return true;}
	else if (c == '7') {return true;}
	else if (c == '8') {return true;}
	else if (c == '9') {return true;}
	else {return false;}
}

class clip {
	public:
		string extra;
		float start;
		float end;
		float dur;
		void sd() {dur = end - start;}
		void merge(const clip &c) {
			if (start > c.start) {start = c.start;} // if this clips start is further into the video than c.start
			if (end < c.end) {end = c.end;} // if the clips end is not as far along into the video as c.end
		}
		float operator + (const float &f) {return f + dur;}
		float operator + (const clip &c) {return dur + c.dur;}
		float operator - (const float &f) {return dur - f;}
		float operator - (const clip &c) {return dur - c.dur;}
		float operator += (const float &f) {return dur + f;}
		float operator -= (const float &f) {return dur - f;}
		bool operator == (const clip &c) {
			if (start == c.start and end == c.end) {return true;}
			else {return false;}
		}
		bool operator != (const clip &c) {
			if (start == c.start and end == c.end) {return false;}
			else {return true;}
		}
};

inline void log(string s) {
	string lp = chfsfx(path, "log");
	if (not (filesystem::exists(lp))) {
	 ofstream wrtlog(lp);
	 wrtlog << lp << endl;
	 wrtlog.close();
	} else {
		ofstream wrtlog(lp, ofstream::app);
		wrtlog << s << endl;
		wrtlog.close();
	}
}

inline void log(string s, string lp) {
	if (not (filesystem::exists(lp))) {
	 ofstream wrtlog(lp);
	 wrtlog << lp << endl;
	 wrtlog.close();
	} else {
		ofstream wrtlog(lp, ofstream::app);
		wrtlog << s << endl;
		wrtlog.close();
	}
}
