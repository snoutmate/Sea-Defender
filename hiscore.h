#pragma once

#include <string>
using namespace std;

#include "snoutlib/misc.h"

#define NUMSCORES 10

#define XORVALUE 16

struct HSitem {
	HSitem(const string& name, int score, bool builtin=false);
	HSitem();

	string m_name;
	int m_score;
  bool m_builtin;

	bool operator<(const HSitem& rhs) const { return m_score > rhs.m_score; }
};

//typedef vector<HSitem> hsitem_ary_t;

class HiScore
{
	HSitem m_scores[NUMSCORES];
  string m_fname;

public:
	HiScore(const string& fname);
	void draw(void);
  void read_from_file(void);
  void save_to_file(void);
  bool is_highscore(int score);
  void add_highscore(const string& name, int score, bool builtin=false);

  string first_nonbuiltin_name(void);
};
