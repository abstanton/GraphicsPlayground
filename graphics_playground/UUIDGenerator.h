#pragma once

class UUIDGenerator {
 public:
  static unsigned int getUUID() {
    auto& inst = UUIDGenerator::get();
    inst.counter_++;
    return inst.counter_;
  }

 private:
  static UUIDGenerator& get() {
    static UUIDGenerator generator;
    return generator;
  }

  UUIDGenerator() {}
  unsigned int counter_;
};
