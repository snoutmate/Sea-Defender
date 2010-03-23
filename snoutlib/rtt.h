#pragma once

class RTT
{
public:
  virtual void start_render(unsigned int num=0) = 0;
  virtual void end_render(void) = 0;
  virtual unsigned int get_texid(unsigned int num=0) = 0;
  virtual ~RTT() = 0;
};
