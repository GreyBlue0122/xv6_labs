// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
#ifdef LAB_LOCK
  int nts;
  int n;
#endif
};

#ifdef LAB_LOCK
// Reader-writer lock.
struct rwspinlock {
  struct spinlock l;          // 内部锁，保护所有字段
  int readers;                // 当前读者数量
  int waiting_writers;        // 等待中的写者数量（包括正在尝试的）
  int writer;                 // 是否有写者持有写锁（0/1）
};

#endif
