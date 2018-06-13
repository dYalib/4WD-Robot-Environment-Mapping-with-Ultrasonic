#ifndef __AVR__

class ServoSim{

private:
  int curpos;

public:
  void write(int pos);
  int read();
};

#endif
