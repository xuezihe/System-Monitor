
#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  long PrevTotal();
  long PrevIdle();
  void Update(long idle,long total);

 private:
  long m_idle;
  long m_total;
};

#endif