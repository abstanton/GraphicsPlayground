#pragma once

class UUIDGenerator {
 public:
  static UUIDGenerator& get() {
    static UUIDGenerator generator;
    return generator;
  }

  unsigned int getUUID() {
    counter_++;
    return counter_;
  }

 private:
  UUIDGenerator() {}
  unsigned int counter_;
};
