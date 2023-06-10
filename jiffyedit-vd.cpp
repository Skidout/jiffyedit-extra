#include "common.hpp" // includes inside common.hpp
#include <filesystem>
#include <algorithm>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

string bfflt, in, totlen, times = "", args = ""; // additional arguments to pass to whisper.cpp
float buff = 0.2;
long long int len;
unsigned int thrnum = thread::hardware_concurrency(); // fetch number of active threads
unsigned short int lngnum = 0;
vector <clip> clparr;
vector <vector <string>> langs = {
	{"en", "zh", "de", "es", "ru", "ko", "fr", "ja", "pt", "tr", "pl", "ca", "nl", "ar", "sv", "it", "id", "hi", "fi", "vi", "iw", "uk", "el", "ms", "cs", "ro", "da", "hu", "ta", "no", "th", "ur", "hr", "bg", "lt", "la", "mi", "ml", "cy", "sk", "te", "fa", "lv", "bn", "sr", "az", "sl", "kn", "et", "mk", "br", "eu", "is", "hy", "ne", "mn", "bs", "kk", "sq", "sw", "gl", "mr", "pa", "si", "km", "sn", "yo", "so", "af", "oc", "ka", "be", "tg", "sd", "gu", "am", "yi", "lo", "uz", "fo", "ht", "ps", "tk", "nn", "mt", "sa", "lb", "my", "bo", "tl", "mg", "as", "tt", "haw", "ln", "ha", "ba", "jw", "su"},
	{"english", "chinese", "german", "spanish", "russian", "korean", "french", "japanese", "portuguse", "turkish", "polish", "catalan", "dutch", "arabic", "swedish", "italian", "indonesian", "hindi", "finnish", "vietnamese", "hebrew", "ukrainian", "greek", "malay", "czech", "romanian", "danish", "hungarian", "tamil", "norwegian", "thai", "urdu", "croatian", "bulgarian", "lithuanian", "latin", "maori", "malayam", "welsh", "slovak", "telugu", "persian", "latvian", "bengali", "serbian", "azerbaijani", "slovenian", "kannada", "estonian", "macedonian", "breton", "basque", "icelandic", "armenian", "nepali", "mongolian", "bosnian", "kazakh", "albanian", "swahili", "galician", "marathi", "punjabi", "sinhala", "khmer", "shona", "yoruba", "somali", "afrikaans", "occitan", "georgian", "belarusian", "tajik", "sindhi", "gujarati", "amharic", "yiddish", "lao", "uzbek", "faroese", "haitian creole", "pashto", "turkmen", "nynorsk", "maltese", "sanskrit", "luxembourgish", "myanmar", "tibetan", "tagalog", "malagassy", "assamese", "tatar", "hawaiian", "lingala", "hausa", "bashkir", "javanese", "sundanese"}
};
bool logging = false;

#ifdef __linux__
	fs::path base = "/usr/share/whisper.cpp-model-base/base.bin";
	fs::path base_en = "/usr/share/whisper.cpp-model-base.en/base.en.bin";
	fs::path large = "/usr/share/whisper.cpp-model-large/large.bin";
	fs::path large_v1 = "/usr/share/whisper.cpp-model-large-v1/large-v1.bin";
	fs::path medium = "/usr/share/whisper.cpp-model-medium/medium.bin";
	fs::path medium_en = "/usr/share/whisper.cpp-model-medium.en/medium.en.bin";
	fs::path small= "/usr/share/whisper.cpp-model-small/small.bin";
	fs::path small_en = "/usr/share/whisper.cpp-model-small.en/small.en.bin";
	fs::path tiny = "/usr/share/whisper.cpp-model-tiny/tiny.bin";
	fs::path tiny_en = "/usr/share/whisper.cpp-model-tiny.en/tiny.en.bin";
	fs::path font = "/usr/share/fonts";
#endif

fs::path model;

void sorter();

void reader() { // make sure to remux to .wav before here ffmpeg -i 23-37-44.mkv -ar 16000 23-37-44-2.wav
	char buf[1025];
	bool tmpwav = false;
	if (not (fs::exists(fs::temp_directory_path()/"jiffyedit-vd"))) {fs::create_directory(fs::temp_directory_path()/"jiffyedit-vd");} // create the jiffyedit-vd directory in temp directory if it does not exist, since whipser does not seem to be able to write otherwise

	string wavpth = chfsfx(path, "wav");
	if (logging) {log("wavpth: " + wavpth);}
	if (not (fs::exists(wavpth))) {
		string concmd = "ffmpeg -i " + path + " -ar 16000 " + wavpth + " 2>&1";
		if (logging) {log("concmd: " + concmd);}
		system(concmd.c_str());
	} else {
		s1 = "ffprobe -v error -select_streams a -of default=noprint_wrappers=1:nokey=1 -show_entries stream=sample_rate \"" + path + "\" 2>&1";
		FILE * inasr;
		inasr = popen(s1.c_str(), "r");
		if (inasr != NULL) {
			fgets(buf, 1024, inasr);
			s1 = buf;
			if (s1.size() == 5 and s1.find("16000") == 0) {}
			else {
				wavpth = remquo(fs::temp_directory_path().generic_string()) + "/jiffyedit-vd/temp.wav";
				string concmd = "ffmpeg -i \"" + path + '\"' + " -ar 16000 " + wavpth;
				if (logging) {log("concmd: " + concmd);}
				system(concmd.c_str());
				tmpwav = true;
			}
		}
	}

	string wtspth = fs::temp_directory_path().generic_string().append("/jiffyedit-vd/clips");
	if (thrnum > 12) {thrnum = 12;}
	else if (thrnum == 0) {thrnum = 4;}
	string cmd = "whisper.cpp -l LANG ARGS TIME -pp -owts -fp FONT -of WTSP -t THREADS -m MODEL \"PATH\" 2>&1";
	if (args.size() > 0) {cmd = replace(cmd, "ARGS", langs.at(0).at(lngnum));} // if additional arguments specified, insert them
	else {cmd = replace(cmd, "ARGS", " ");}
	if (times.size() > 0) {cmd = replace(cmd, "TIME", times);}
	else {cmd = replace(cmd, "TIME", " ");}
	cmd = replace(cmd, "FONT", font.generic_string());
	cmd = replace(cmd, "WTSP", wtspth);
	cmd = replace(cmd, "LANG", langs.at(0).at(lngnum));
	cmd = replace(cmd, "THREADS", to_string(thrnum));
	cmd = replace(cmd, "MODEL", model.generic_string());
	cmd = replace(cmd, "PATH", wavpth);
	FILE * inmeta = NULL;
	b1 = false;
	if (logging) {log("cmd: " + cmd);}
	stringstream f; // stringstream for being able to do things by line
	inmeta = popen(cmd.c_str(), "r");
	thread pctrl(pcheck, ref(inmeta), ref(b1));
	while (not b1) {
		while (fgets(buf, 1024, inmeta) != NULL) {
			s1 = buf;
			if (s1.size() < 5) {}
			else if (s1.at(s1.size() - 1) == '%' or s1.at(s1.size() - 2) == '%' or s1.at(s1.size() - 3) == '%' or s1.at(s1.size() - 4) == '%') {s1.erase(0, s1.size() - 4); cout << "reset: clipping: " << s1;} // progress
		}
		pclose(inmeta); // closes the pipe after the end of the datastream
		inmeta = NULL; // clears the datastream from memory (i think)
	}

	if (tmpwav) {fs::remove(wavpth);}

	wtspth.append(".wts");
	if (logging) {log("wtspth: " + wtspth);}
	ifstream getwts(wtspth);
	i2 = 0;
	while (getwts) {
		getwts.get(c1);
		b1 = false;
		s1 = "2:y=h/2:text='";
		if (c1 == s1.at(0)) {
			for (i1 = 1; i1 < s1.size(); i1++) {
				getwts.get(c1);
				if (c1 == s1.at(i1) and i1 == s1.size() - 1) {b1 = true; break;}
				else if (c1 == s1.at(i1)) {}
				else {break;}
			}
		}
		if (b1) {
			clip tmpclp;
			tmpclp.extra = "";
			getwts.get(c1);
			while (c1 != '\'') {
				tmpclp.extra.push_back(c1);
				getwts.get(c1);
			} // clip.extra used for subtitles
			while (c1 != '(') {getwts.get(c1);}
			s2 = "";
			while (not (isnum(c1))) {getwts.get(c1);}
			while (c1 != ')') {
				if (c1 == ',') {c1 = ' ';}
				s2.push_back(c1);
				getwts.get(c1);
			}
			stringstream stamps(s2);
			stamps >> tmpclp.start >> tmpclp.end;
			tmpclp.sd();
			if (tmpclp.extra.size() >= 6 and (tmpclp.extra.at(0) == '[' or tmpclp.extra.at(1) == '[' or tmpclp.extra.at(2) == '[' or tmpclp.extra.at(3) == '[' or tmpclp.extra.at(4) == '[' or tmpclp.extra.at(5) == '[')) {} // strings are not okay, idk
			else if (tmpclp.dur > 0) {clparr.push_back(tmpclp);}
		}
	}
	getwts.close();
	if (clparr.size() == 0) {cout << "Fatal error: No clips found in video." << endl; exit(4);}

	fs::remove(wtspth); // delete temporary .wts file
// on windows it is up to programs to manage their temp files. on linux, they are typically wiped after a restart. on macos, they are deleted three days after not being accessed.
// the size of the log files here is likely to be only a few KB, so we dont have to be too concerned with managing them
// and a future filterer will need it later
	sorter();
}

int main(int argc, char * arga[]) {
	model = base;
	fs::current_path(font);
	for (auto &p: fs::recursive_directory_iterator(fs::current_path())) { // get a list of every file in the current directory, then sort them to .dat files only
		stringstream getfnt;
		getfnt << p;
		getline(getfnt, s1);
		reverse(s1.begin(), s1.end());
		if (s1.at(0) == '\'') {s1.erase(0, 1);} // remove quotation marks sorrounding path, if any
		else if (s1.at(0) == '\"') {s1.erase(0, 1);}
		reverse(s1.begin(), s1.end());
		if (s1.at(0) == '\'') {s1.erase(0, 1);}
		else if (s1.at(0) == '\"') {s1.erase(0, 1);}
		reverse(s1.begin(), s1.end());
		if(s1.find("ftt.") == 0) {reverse(s1.begin(), s1.end()); font = s1; break;}
	}

	vector <string> argv;
	for (i1 = 0; i1 < argc; i1++) { argv.push_back(arga[i1]); }
	path = argv.at(1);
	if (argv.size() > 2) {
		for (i1 = 2; i1 < argv.size(); i1++) {
			if (argv.at(i1).find("th") == 0 and argv.at(i1).size() == 2) {thrnum = stoi(argv.at(++i1));} // threads
			else if (argv.at(i1).find("md") == 0 and argv.at(i1).size() == 2) { // model
				i1++;
				if (argv.at(i1).find("base") == 0 and argv.at(i1).size() == 4) {model = base;}
				else if (argv.at(i1).find("base_en") == 0) {model = base_en;}
				else if (argv.at(i1).find("large") == 0 and argv.at(i1).size() == 5) {model = large;}
				else if (argv.at(i1).find("large_v1") == 0) {model = large_v1;}
				else if (argv.at(i1).find("medium") == 0 and argv.at(i1).size() == 6) {model = medium;}
				else if (argv.at(i1).find("medium_en") == 0) {model = medium_en;}
				else if (argv.at(i1).find("small") == 0 and argv.at(i1).size() == 5) {model = small;}
				else if (argv.at(i1).find("small_en") == 0) {model = small_en;}
				else if (argv.at(i1).find("tiny") == 0 and argv.at(i1).size() == 4) {model = tiny;}
				else if (argv.at(i1).find("tiny_en") == 0) {model = tiny_en;}

			}
			else if (argv.at(i1).find("aa") == 0 and argv.at(i1).size() == 2) {args = argv.at(++i1);} // additional arguments to pass to whisper.cpp
			else if (argv.at(i1).find("ln") == 0 and argv.at(i1).size () == 2) {
				i1++;
				if (isnum(argv.at(i1).at(0))) {lngnum = stoi(argv.at(i1));} // direct reference by number
				else if (argv.at(i1).size() == 2 or (argv.at(i1).size() == 3 and argv.at(i1).at(0) == 'h' /* for hawaiian */)) { // short language code
					for (i2 = 0; i2 < langs.at(0).size(); i2++) {
						if (langs.at(0).at(i2).find(argv.at(i1)) == 0) {lngnum = i2; break;}
					}
				} else { // full language name
					for (i2 = 0; i2 < langs.at(1).size(); i2++) {
						if (langs.at(1).at(i2).find(argv.at(i1)) == 0) {lngnum = i2; break;}
					}
				}
			} else if (argv.at(i1).find("bt") == 0 and argv.at(i1).size() == 2) {
				f1 = stof(argv.at(++i1)); // start of section
				f2 = stof(argv.at(++i1)); // end of section
				times = "-ot ";
				times.append(to_string(f1));
				times.append(" -d ");
				times.append(to_string(f2 - f1)); // dur of section
			} else if (argv.at(i1).find("cm") == 0 and argv.at(i1).size() == 2) {model = argv.at(i1);}
			else if (argv.at(i1).find("fp") == 0 and argv.at(i1).size() == 2) {font = argv.at(++i1);}
			else if (argv.at(i1).find("bf") == 0 and argv.at(i1).size() == 2) {buff = stof(argv.at(++i1));}
			else if (argv.at(i1).find("-log") == 0) {logging = true;}
		}
	}
	if (not fs::exists(model)) {cout << "Fatal error: model does not exist. If you specified a custom model, make sure it is the full path. Otherwise, make sure you have the models installed." << endl; exit(4);}

	if (logging) {
		log("args:");
		for (i1 = 0; i1 < argv.size(); i1++) {log("	" + argv.at(i1));}
	}

	reader();
}

void sorter() {
	i1 = 1;
	while (i1 < clparr.size()) {
		if (clparr.at(i1).start == clparr.at(i1 - 1).end) {
			clparr.at(i1 - 1).extra.append(clparr.at(i1).extra);
			clparr.at(i1 - 1).end = clparr.at(i1).end;
			auto iter = clparr.begin();
			for (i2 = 0; i2 < i1; i2++) {iter++;}
			clparr.erase(iter, iter + 1);
		} else {i1++;}
	}
	//cout << langs.at(0).at(lngnum) << endl; // keeping this here to maybe write subtitle logs later
	//for (i1 = 0; i1 < clparr.size(); i1++) {cout << clparr.at(i1).start << " " << clparr.at(i1).end << ": " << clparr.at(i1).extra << endl;}
	if (logging) {log("clips:");}
	for (i1 = 0; i1 < clparr.size(); i1++) {
		if (logging) {log("	" + to_string(clparr.at(i1).start) + "-" + to_string(clparr.at(i1).end));}
		cout << endl << "clipstart: " << clparr.at(i1).start << endl << "clipend: " << clparr.at(i1).end;
	}
	exit(0);
}
