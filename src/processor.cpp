#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  // Return the aggregate CPU utilization
  long total_old, total_new, idle_old, idle_new;
  total_new = LinuxParser::Jiffies();
  idle_new = LinuxParser::IdleJiffies();

  total_old = PrevTotal();
  idle_old = PrevIdle();

  Update(idle_new,total_new);

  float totalDelta = float(total_new) - float(total_old);
  float idleDetla = float(idle_new) - float(idle_old);

  float utilization = (totalDelta - idleDetla) / totalDelta;
  return utilization;
}

long Processor::PrevTotal() { return m_total; }
long Processor::PrevIdle() { return m_idle; }
void Processor::Update(long idle,long total) {
  m_idle = idle;
  m_total = total;
}
