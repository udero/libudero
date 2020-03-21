#pragma once

#include <string>
#include <iostream>
#include <ios>
#include <fstream>
#include <sstream>
#include <map>


inline std::string trim(const std::string& string, const char* removedChara = " \t\v\r\n")
{
  std::string res;
  size_t left = string.find_first_not_of(removedChara);

  if (left != std::string::npos) {
    size_t right = string.find_last_not_of(removedChara);
    res = string.substr(left, right - left + 1);
  }

  return res;
}


class ParamLoader {
  std::map<std::string, std::string> m_map;

 public:
  ParamLoader(std::string filename) {
    std::ifstream fin;//
    fin.open(filename.c_str());

    while (true) {
      if (!fin.good()) break;
      std::string line;
      std::getline(fin, line);
      std::stringstream ss;
      ss << line;

      std::string key;
      std::getline(ss, key, ':');

      if (!ss.good()) continue;

      std::string value;
      std::getline(ss, value);

      if (trim(key).find("#") == std::string::npos) {
	value = trim(value);
	if (value.find("#") != std::string::npos) {
	  int c = value.find("#");
	  value = trim(value.substr(0, value.find("#")));
	}
	m_map[trim(key)] = trim(value);
      }

      if (!fin.good()) break;

    }
  }


  std::string get(const std::string& key, const double defaultValue) {
    if (m_map.find(key) != m_map.end()) {
      return m_map[key];
    }
    std::stringstream ss;
    ss << defaultValue;
    return ss.str();

  }

  std::string get(const std::string& key, const std::string& defaultValue) {
    if (m_map.find(key) != m_map.end()) {
      return m_map[key];
    }
    return defaultValue;
  }

  std::string toString() {
    std::stringstream ss;
    for (std::map<std::string, std::string>::const_iterator p = m_map.begin(); p != m_map.end(); ++p) {
      ss << p->first << ": " << p->second << std::endl;
    }
    return ss.str();
  }
};
