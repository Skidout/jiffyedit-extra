#include "common.hpp"

using namespace std;

int main(int argc, char * arga[]) {
	vector <string> argv;
	bool logging = false;
	for (i1 = 0; i1 < argc; i1++) {
		argv.push_back(arga[i1]);
	}

	path = argv.at(1);
	string editor = argv.at(2);
	long long unsigned int fltnum = stoi(argv.at(3));
	float prolen = stof(argv.at(4));
	float prolenm1 = stof(argv.at(5));

	cout << 1 << endl;

	float volume = -15.0;
	int window = 600;
	float maxgain = 15.0;
	float mingain = -15.0;
	float maxrate = 3.0;
	bool reond = false; // reset on discontinuity
	for (i1 = 6; i1 < argv.size(); i1++) {
		if (argv.at(i1).find("-log") == 0) {logging = true;}
		else if (argv.at(i1).find("vol") == 0) {
			i1++;
			volume = stof(argv.at(i1));
			if (volume > 0.0) {volume = volume * -1;}
		} else if (argv.at(i1).find("win") == 0) {
			i1++;
			window = stoi(argv.at(i1));
			if (window < 2 or window > 600) {
				cout << "Fatal error: window cannot be less than two or more than 600";
				exit(4);
			}
		} else if (argv.at(i1).find("mxg") == 0) {
			i1++;
			maxgain = stof(argv.at(i1));
			if (maxgain < 0.0) {maxgain = maxgain * -1;}
		} else if (argv.at(i1).find("mng") == 0) {
			i1++;
			mingain = stof(argv.at(i1));
			if (mingain > 0.0) {mingain = mingain * -1;}
		} else if (argv.at(i1).find("mxr") == 0) {
			i1++;
			maxrate = stof(argv.at(i1));
			if (maxrate < 0.0) {maxrate = maxrate * -1;}
		} else if (argv.at(i1).find("rod") == 0) {reond = true;}
	}

	if (logging) {
		log("start of [nz]");
		for (i1 = 0; i1 < argv.size(); i1++) {
			log("args:");
			log(argv.at(i1));
		}
	}

	if (editor.find("shotcut") == 0) {
	s1 = "    <filter id=\"FLTNUM\" out=\"PROLENM1\">";
	s1 = replace(s1, "FLTNUM", to_string(fltnum));
	s1 = replace(s1, "PROLENM1", toanatim(prolenm1, 3));
	cout << s1 << endl;
	s1 = "      <property name=\"target_loudness\">VOLUME</property>";
	stringstream getvol;
	getvol << fixed << setprecision(1) << volume;
	getline(getvol, s2);
	s1 = replace(s1, "VOLUME", s2);
	cout << s1 << endl;
	s1 = "      <property name=\"window\">WINDOW</property>";
	s1 = replace(s1, "WINDOW", to_string(window));
	cout << s1 << endl;
	s1 = "      <property name=\"max_gain\">MAXGAIN</property>";
	stringstream getmxg;
	getmxg << fixed << setprecision(1) << maxgain;
	getline(getvol, s2);
	if (s2.at(0) == '-') {s2.erase(0, 1);}
	s1 = replace(s1, "MAXGAIN", s2);
	cout << s1 << endl;
	s1 = "      <property name=\"min_gain\">MINGAIN</property>";
	stringstream getmng;
	getmng << fixed << setprecision(1) << mingain;
	getline(getmng, s2);
	s1 = replace(s1, "MINGAIN", s2);
	cout << s1 << endl;
	s1 = "      <property name=\"max_rate\">MAXRATE</property>";
	stringstream getmxr;
	getmxr << fixed << setprecision(1) << maxrate;
	getline(getmxr, s2);
	s1 = replace(s1, "MAXRATE", s2);
	cout << s1 << endl;
	s1 = "      <property name=\"discontinuity_reset\">0</property>";
	if (reond) {s1 = replace(s1, "0", "1");}
	cout << s1 << endl;
	cout << "      <property name=\"in_loudness\">-nan</property>\n      <property name=\"out_gain\">-nan</property>\n      <property name=\"reset_count\">0</property>\n      <property name=\"mlt_service\">dynamic_loudness</property>\n    </filter>\n";
	}

	if (logging) {log("end of [nz]");}

	exit(0);
}
